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

// these flags are used for config mem
#define CONFIG_IN_EXTERNAL_FLASH
#define USE_FLASH_CHIP

#define SERIAL_PORT_COUNT       1

#undef TARGET_BUS_INIT
#undef USE_VCP
#undef USE_BRUSHED_ESC_AUTODETECT  // Detect if brushed motors are connected and set defaults appropriately to avoid motors spinning on boot
#undef USE_GYRO_REGISTER_DUMP  // Adds gyroregisters command to cli to dump configured register values
#undef USE_TIMER
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

//RISCV flags
#define __riscv64
#define TCB_SPAN_NO_EXCEPTIONS
#define TCB_SPAN_NO_CONTRACT_CHECKING

#define NNCASE_TARGET k210
#define KENDRYTE_SDK_TYPE 1
#define KENDRYTE_SDK_TYPE_STANDALONE 1
#define KENDRYTE_SDK_TYPE_FREERTOS 2
#define KENDRYTE_SDK_RELEASE_DATE 0
#define DEBUG 1
#define LOG_KERNEL
#define LV_CONF_INCLUDE_SIMPLE
#define LINKER_LANGUAGE C


// Declare typedefs needed for config storage compilation
typedef enum
{
    Mode_TEST = 0x0,
    Mode_Out_PP = 0x10
} GPIO_Mode;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {DIS = 0, EN = !DIS} FunctionalState;
typedef enum {TEST_IRQ = 0 } IRQn_Type;
typedef enum {
    EXTI_Trigger_Rising = 0x08,
    EXTI_Trigger_Falling = 0x0C,
    EXTI_Trigger_Rising_Falling = 0x10
} EXTITrigger_TypeDef;

typedef struct
{
  uint32_t IDR;
  uint32_t ODR;
  uint32_t BSRR;
  uint32_t BRR;
} GPIO_TypeDef;

#define GPIOA_BASE ((intptr_t)0x0001)

typedef struct
{
    void* test;
} TIM_TypeDef;

typedef struct
{
    void* test;
} TIM_OCInitTypeDef;

typedef struct {
    void* test;
} DMA_TypeDef;

typedef struct {
    void* test;
} DMA_Channel_TypeDef;

uint8_t DMA_GetFlagStatus(void *);
void DMA_Cmd(DMA_Channel_TypeDef*, FunctionalState );
void DMA_ClearFlag(uint32_t);

typedef struct
{
    void* test;
} SPI_TypeDef;

typedef struct
{
    void* test;
} USART_TypeDef;

#define USART1 ((USART_TypeDef *)0x0001)
#define USART2 ((USART_TypeDef *)0x0002)
#define USART3 ((USART_TypeDef *)0x0003)
#define USART4 ((USART_TypeDef *)0x0004)
#define USART5 ((USART_TypeDef *)0x0005)
#define USART6 ((USART_TypeDef *)0x0006)
#define USART7 ((USART_TypeDef *)0x0007)
#define USART8 ((USART_TypeDef *)0x0008)

#define UART4 ((USART_TypeDef *)0x0004)
#define UART5 ((USART_TypeDef *)0x0005)
#define UART7 ((USART_TypeDef *)0x0007)
#define UART8 ((USART_TypeDef *)0x0008)

typedef struct
{
    void* test;
} I2C_TypeDef;

typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
} FLASH_Status;

typedef struct {
    double timestamp;                   // in seconds
    double imu_angular_velocity_rpy[3]; // rad/s -> range: +/- 8192; +/- 2000 deg/se
    double imu_linear_acceleration_xyz[3];    // m/s/s NED, body frame -> sim 1G = 9.80665, FC 1G = 256
    double imu_orientation_quat[4];     //w, x, y, z
    double velocity_xyz[3];             // m/s, earth frame
    double position_xyz[3];             // meters, NED from origin
} fdm_packet;
typedef struct {
    float motor_speed[4];   // normal: [0.0, 1.0], 3D: [-1.0, 1.0]
} servo_packet;

typedef struct
{
    void* test;
} ADC_TypeDef;

