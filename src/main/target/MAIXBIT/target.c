/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "drivers/system.h"
#include "riscv_k210_uart.h"
#include "riscv_k210_gpiohs.h"
#include "riscv_k210_sysctl.h"
#include <unistd.h>
#include "riscv_k210_platform.h"


#define CMD_LENTH  4
#define UART_NUM    UART_DEVICE_1


#include "riscv_k210_fpioa.h"
#include "riscv_k210_gpio.h"


uint32_t recv_buf[48];
#define RECV_DMA_LENTH  6

gpio_pin_value_t value = GPIO_PV_HIGH;
volatile uint32_t recv_flag = 0;
char g_cmd[4];
volatile uint8_t g_cmd_cnt = 0;


void led();
volatile uint32_t g_uart_send_flag = 0;

void failureMode(failureMode_e mode) {
	printf("[failureMode]!!! %d\n", mode);
	while (1);
}

// Only for testing and demo
void io_mux_init(void)
{

	gpio_init();
	fpioa_set_function( 24, FUNC_GPIO3 );
	fpioa_set_function( 25, FUNC_GPIO4 );
	fpioa_set_function( 26, FUNC_GPIO5 );
	gpio_set_drive_mode( 3, GPIO_DM_OUTPUT );
	gpio_set_drive_mode( 4, GPIO_DM_OUTPUT );
	gpio_set_drive_mode( 5, GPIO_DM_OUTPUT );
	gpio_set_pin( 3, value );
	gpio_set_pin( 4, !value );
	gpio_set_pin( 5, value );
	fpioa_set_function(4, FUNC_UART1_RX + UART_NUM * 2);
	fpioa_set_function(5, FUNC_UART1_TX + UART_NUM * 2);
}

int uart_send_done(void *ctx)
{
	g_uart_send_flag = 1;
	return 0;
}

int uart_recv_done(void *ctx)
{
	uint32_t *v_dest = ((uint32_t *)ctx) + RECV_DMA_LENTH;
	if(v_dest >= recv_buf + 48)
		v_dest = recv_buf;

	uart_data_t data = (uart_data_t)
	{
		.rx_channel = DMAC_CHANNEL1,
			.rx_buf = v_dest,
			.rx_len = RECV_DMA_LENTH,
			.transfer_mode = UART_RECEIVE,
	};

	plic_interrupt_t irq = (plic_interrupt_t)
	{
		.callback = uart_recv_done,
			.ctx = v_dest,
			.priority = 2,
	};

	uart_handle_data_dma(UART_NUM, data, &irq);
	uint32_t *v_buf = (uint32_t *)ctx;
	return 0;
}
void led(){

	gpio_set_pin( 3, value = !value );
	sleep(1);
	gpio_set_pin( 4, value = !value );
	sleep( 1);
	gpio_set_pin( 5, value = !value );
}

int blink_uart(void)
{


	uart_init(UART_NUM);
	uart_configure(UART_NUM, 115200, 8, UART_STOP_1, UART_PARITY_NONE);

	uint8_t *hel = {"UART \n"};

	uint32_t *v_tx_buf = malloc(sizeof(hel) * sizeof(uint32_t));
	for(uint32_t i = 0; i < strlen(hel); i++)
	{
		v_tx_buf[i] = hel[i];
	}

	uart_data_t data = (uart_data_t)
	{
		.tx_channel = DMAC_CHANNEL0,
			.tx_buf = v_tx_buf,
			.tx_len = strlen(hel),
			.transfer_mode = UART_SEND,
	};

	plic_interrupt_t irq = (plic_interrupt_t)
	{
		.callback = uart_send_done,
			.ctx = NULL,
			.priority = 1,
	};

	uart_handle_data_dma(UART_NUM, data, &irq);

	uart_data_t v_rx_data = (uart_data_t)
	{
		.rx_channel = DMAC_CHANNEL1,
			.rx_buf = recv_buf,
			.rx_len = RECV_DMA_LENTH,
			.transfer_mode = UART_RECEIVE,
	};

	plic_interrupt_t v_rx_irq = (plic_interrupt_t)
	{
		.callback = uart_recv_done,
			.ctx = recv_buf,
			.priority = 2,
	};
	uart_handle_data_dma(UART_NUM, v_rx_data, &v_rx_irq);
	while(1)
	{
		sleep(1);
		led();
		uart_handle_data_dma(UART_NUM, data, &irq);
		g_uart_send_flag = 1;
	}
}

