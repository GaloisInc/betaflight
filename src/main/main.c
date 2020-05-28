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

#include "platform.h"

#include "fc/init.h"

//#include "scheduler/scheduler.h"

void run(void);

#include "drivers/flash_riscv_k210.h"
#define MY_ADDR 0x80200000
uint8_t my_data_buf[0];

int main(void)
{
    // this is temp..to allow minicom to boot up
    sleep(2);

    init();

    // for demo purpose - erase flash every other board reset
    // need to erase previous config or else it
    // will read it again so you won't see it write again --- for dev
    uint32_t my_index = 0x1;
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

    //run();

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
