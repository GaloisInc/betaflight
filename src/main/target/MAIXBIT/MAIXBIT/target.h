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
 * First iteration of target.h for K210 target.
 * Features Acc/Gyro, UART are placeholders.
 * Use this version of target.h to compile initial build of main.c.
 *
 * Need to map FE310-G002 pinout to STM pinout sytem (PA1-16, PB1-16, PC1-16, etc.)
 */

#pragma once

#define TARGET_BOARD_IDENTIFIER "COLIBRI"

#define USBD_PRODUCT_STRING "COLIBRI"

#define USE_VCP
#define SERIAL_PORT_COUNT 1 // VCP

//TARGET_IO_PORT<port> is bitmask of used pins on target
//PORTA corresponds to pins PA1-16, PORTB to PB1-16, etc.
#define TARGET_IO_PORTA 0xffff
#define TARGET_IO_PORTB 0xffff
#define TARGET_IO_PORTC 0xffff


// *************** Gyro & ACC **********************
#undef USE_ACC
#undef USE_FAKE_ACC

#undef USE_GYRO
#undef USE_FAKE_GYRO


// *************** UART *****************************
#undef USE_UART1
#undef USE_UART2
#undef USE_UART3

#undef USE_SOFTSERIAL1
#undef USE_SOFTSERIAL2


// *************** DEFAULT FEATURES TO UNDEFINE **************
#undef USE_BRUSHED_ESC_AUTODETECT  // Detect if brushed motors are connected and set defaults appropriately to avoid motors spinning on boot
#undef USE_GYRO_REGISTER_DUMP  // Adds gyroregisters command to cli to dump configured register values
#undef USE_TIMER
#undef USE_MOTOR

#undef USE_PWM_OUTPUT
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

#undef USE_DMA
#undef USE_ADC
#undef USE_DSHOT
#undef USE_GYRO_DATA_ANALYSE
#undef USE_ADC_INTERNAL
#undef USE_USB_CDC_HID
#undef USE_USB_MSC
#undef USE_OVERCLOCK
#undef USE_RTC_TIME
#undef USE_RCDEVICE

// *************** TIMERS *****************

//Defines used timers and total timer channels (4 channels per timer
//See target.c for usage
#define USABLE_TIMER_CHANNEL_COUNT 4
#define USED_TIMERS (TIM_N(1))
