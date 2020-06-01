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

#include "riscv_k210_fpioa.h"
#include "riscv_k210_gpio.h"
void failureMode(failureMode_e mode) {
    printf("[failureMode]!!! %d\n", mode);
    while (1);
}

// Only for testing and demo
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
