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




/*
 * First iteration of target.c for Hi-Five target.
 * Will need to configure this for Hi-Five target once we start 
 * building for hardware.
 *
 * Example code below from DOGE target's target.c
 */

#include <stdint.h>

#include "platform.h"
#include "drivers/io.h"

#include "drivers/timer.h"
#include "drivers/dma.h"
#include "drivers/timer_def.h"

//I think we only need timers for PWM outputs to 4x motors
const timerHardware_t timerHardware[USABLE_TIMER_CHANNEL_COUNT] = {
    DEF_TIM(TIM1,  CH1, PB11, TIM_USE_MOTOR, 0, 0 ), // PWM1 - OUT1 (Motor 1)
    DEF_TIM(TIM1,  CH2, PA1,  TIM_USE_MOTOR, 0, 0 ), // PWM2 - OUT2 (Motor 2)
    DEF_TIM(TIM1,  CH3, PA15, TIM_USE_MOTOR, 0, 0 ), // PWM3 - OUT3 (Motor 3)
    DEF_TIM(TIM1,  CH4, PB9,  TIM_USE_MOTOR, 0, 0 ), // PWM4 - OUT4 (Motor 4)
};
