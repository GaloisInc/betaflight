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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "riscv_k210_fpioa.h"
#include "riscv_k210_gpio.h"

#include "platform.h"

#include "fc/init.h"
void blink();
//#include "scheduler/scheduler.h"

void run(void);

/**** capstone - for demo ****/
#include "drivers/flash_riscv_k210.h"
#define MY_ADDR 0x80200000
/**** capstone - end of demo ****/

int main(void)
{
    // this is temp..to allow minicom to boot up
    sleep(2);

    init();

    /**** capstone - for demo ****/
    // for demo purpose - erase flash every other board reset
    // need to erase previous config or else it
    // will read it again so you won't see it write again --- for dev
    uint32_t my_index = 0x1;
    uint8_t my_data_buf[0];
    flash_read_data(MY_ADDR, &my_data_buf[0], 1, FLASH_STANDARD);
    if ((int)my_data_buf[0] == 1) {
        my_index = 0x2;
        my_data_buf[0] = (uint8_t)(my_index);
        flash_write_data(MY_ADDR, (uint8_t *)my_data_buf, 8);
        flash_sector_erase(FLASH_START_ADDR);
    } else {
        my_index = 0x1;
        my_data_buf[0] = (uint8_t)(my_index);
        flash_write_data(MY_ADDR, (uint8_t *)my_data_buf, 8);
    }
    printf("Board Initialized: OK\n\n");
    /**** capstone - end of demo ****/
	blink();
    //run();

    return 0;
}
void blink() {
	gpio_init();
	gpio_pin_value_t value = GPIO_PV_HIGH;
	fpioa_set_function( 24, FUNC_GPIO3 );
	fpioa_set_function( 25, FUNC_GPIO4 );
	fpioa_set_function( 26, FUNC_GPIO5 );
	gpio_set_drive_mode( 3, GPIO_DM_OUTPUT );
	gpio_set_drive_mode( 4, GPIO_DM_OUTPUT );
	gpio_set_drive_mode( 5, GPIO_DM_OUTPUT );
	gpio_set_pin( 3, value );
	gpio_set_pin( 4, !value );
	gpio_set_pin( 5, value );
	while( 1 ) {
		sleep( 1 );
		gpio_set_pin( 3, value = !value );
		printf("LED 1 is %d\n", value);
		sleep( 1 );
		gpio_set_pin( 4, value = !value );
		printf("LED 2 is %d\n", value);
		sleep( 1 );
		gpio_set_pin( 5, value = !value );
		printf("LED 3 is %d\n",value);
	}
	return 0;
}
/*
void FAST_CODE FAST_CODE_NOINLINE run(void) {
    while (true) {
        scheduler();
        processLoopback();
#ifdef SIMULATOR_BUILD
        delayMicroseconds_real(50); // max rate 20kHz
#endif
    }
}
*/
