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

/* *********** start of temp test *********** */
#include "drivers/flash_riscv_k210.h"
#define TEST_NUMBER (256 + 128)
#define DATA_ADDRESS 0x130000
uint8_t data_buf[TEST_NUMBER];
/* *********** end of temp test *********** */

int main(void)
{
    /* *********** start of temp test *********** */
    printf("========= Pinche Corona-19 ====\n\n");
    uint32_t index;
    flash_init(3, 0);

    flash_enable_quad_mode();

    /*write data*/
    for (index = 0; index < TEST_NUMBER; index++)
        data_buf[index] = (uint8_t)(index);
    printf("Erase Sector\n");
    flash_sector_erase(DATA_ADDRESS);
    while (flash_is_busy() == FLASH_BUSY)
        ;
    printf("Write Data\n");
    flash_write_data_direct(DATA_ADDRESS, data_buf, TEST_NUMBER);

    /* standard read test*/
    for (index = 0; index < TEST_NUMBER; index++)
        data_buf[index] = 0;
    printf("Standard Read Test Start\n");
    flash_read_data(DATA_ADDRESS, data_buf, TEST_NUMBER, FLASH_STANDARD);
    for (index = 0; index < TEST_NUMBER; index++)
    {
        if (data_buf[index] != (uint8_t)(index))
        {
            printf("Standard Read Test Error\n");
            return 0;
        }
    }

    printf("SPI3 Master Test OK\n");
    printf("\n=============\n\n\n");
    /* *********** end of temp test *********** */

    init();
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