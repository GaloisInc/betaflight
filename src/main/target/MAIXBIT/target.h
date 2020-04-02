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

#pragma once

#define TARGET_BOARD_IDENTIFIER "MAIXBIT"
#define USBD_PRODUCT_STRING "Minimal MAIXBIT"

# this flag is used for config mem
#define CONFIG_IN_EXTERNAL_FLASH

#undef USE_BEEPER

#define USE_VCP

#define SERIAL_PORT_COUNT       1 // VCP

#define TARGET_IO_PORTA (0xffff & ~(BIT(14)|BIT(13)))
#define TARGET_IO_PORTB (0xffff & ~(BIT(2)))
#define TARGET_IO_PORTC (0xffff & ~(BIT(15)|BIT(14)|BIT(13)))
#define TARGET_IO_PORTD BIT(2)

#undef USE_BRUSHED_ESC_AUTODETECT  // Detect if brushed motors are connected and set defaults appropriately to avoid motors spinning on boot
#undef USE_GYRO_REGISTER_DUMP  // Adds gyroregisters command to cli to dump configured register values
//#undef USE_TIMER
//#undef USE_MOTOR

//#undef USE_PWM_OUTPUT
#undef USE_EXTI
#undef USE_ACC
#undef USE_PPM
#undef USE_PWM
#undef USE_UART
#undef USE_SERIAL_RX
#undef USE_SERIALRX_CRSF       // Team Black Sheep Crossfire protocol
#undef USE_SERIALRX_IBUS       // FlySky and Turnigy receivers
#undef USE_SERIALRX_SBUS       // Frsky and Futaba receivers
#undef USE_SERIALRX_SPEKTRUM   // SRXL, DSM2 and DSMX protocol
#undef USE_SERIALRX_SUMD       // Graupner Hott protocol
#undef USE_SERIALRX_SUMH       // Graupner legacy protocol
#undef USE_SERIALRX_XBUS       // JR
#undef USE_LED_STRIP

#undef USE_ACRO_TRAINER
#undef USE_BLACKBOX
#undef USE_RUNAWAY_TAKEOFF     // Runaway Takeoff Prevention (anti-taz)
#undef USE_SERVOS
#undef USE_TELEMETRY
#undef USE_TELEMETRY_FRSKY_HUB
#undef USE_TELEMETRY_HOTT
#undef USE_TELEMETRY_LTM
#undef USE_TELEMETRY_SMARTPORT

#undef USE_CMS
#undef USE_OSD
#undef USE_VTX_COMMON
#undef USE_VTX_CONTROL
#undef USE_VTX_TRAMP
#undef USE_VTX_SMARTAUDIO
#undef USE_CAMERA_CONTROL
#undef USE_GPS
#undef USE_GPS_RESCUE

#undef USE_I2C_OLED_DISPLAY
#undef USE_MSP_DISPLAYPORT
#undef USE_OSD_OVER_MSP_DISPLAYPORT

//#undef USE_DMA
#undef USE_ADC
#undef USE_DSHOT
#undef USE_GYRO_DATA_ANALYSE
#undef USE_ADC_INTERNAL
#undef USE_USB_CDC_HID
#undef USE_USB_MSC
#undef USE_OVERCLOCK
#undef USE_RTC_TIME
#undef USE_RCDEVICE

#define USABLE_TIMER_CHANNEL_COUNT 14
#define USED_TIMERS ( TIM_N(1) | TIM_N(2) | TIM_N(3) | TIM_N(4) | TIM_N(5) | TIM_N(10) | TIM_N(12) | TIM_N(8) | TIM_N(9))
