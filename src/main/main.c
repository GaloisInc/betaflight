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

//#include <stdio.h>
#include <unistd.h>
#include "fpioa.h"
#include "gpio.h"

<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 29eb4da4e634b1ec552fb8a0755d6d1867b139ed
int main(void)
{
    fpioa_set_function(24, FUNC_GPIO3);

    gpio_init();
    gpio_set_drive_mode(3, GPIO_DM_OUTPUT);
    gpio_pin_value_t value = GPIO_PV_HIGH;
    gpio_set_pin(3, value);
    while (1)
    {
        sleep(1);
        gpio_set_pin(3, value = !value);
    }
    return 0;
}
<<<<<<< HEAD

//#include <stdbool.h>
//#include <stdint.h>

/*
#include "platform.h"

#include "fc/init.h"

=======

//#include <stdbool.h>
//#include <stdint.h>

/*
#include "platform.h"

#include "fc/init.h"

=======
#include "platform.h"

#include "fc/init.h"

>>>>>>> 62eb123b451bf7115453fb13863d4befb70db427
>>>>>>> 29eb4da4e634b1ec552fb8a0755d6d1867b139ed
#include "scheduler/scheduler.h"

void run(void);

int main(void)
{
<<<<<<< HEAD
    //init();
=======
<<<<<<< HEAD
    //init();
=======
    init();
>>>>>>> 62eb123b451bf7115453fb13863d4befb70db427
>>>>>>> 29eb4da4e634b1ec552fb8a0755d6d1867b139ed

    run();

    return 0;
}

void FAST_CODE FAST_CODE_NOINLINE run(void)
{
    while (true) {
        scheduler();
        processLoopback();
#ifdef SIMULATOR_BUILD
        delayMicroseconds_real(50); // max rate 20kHz
#endif
    }
}
<<<<<<< HEAD
*/
=======
<<<<<<< HEAD
*/
=======
>>>>>>> 62eb123b451bf7115453fb13863d4befb70db427
>>>>>>> 29eb4da4e634b1ec552fb8a0755d6d1867b139ed
