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
// this flag is used for config mem


#define CONFIG_IN_EXTERNAL_FLASH
#define USE_FLASH_CHIP

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

