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

#include <stdint.h>

#include "drivers/flash_riscv_k210.h"
#include "pg/flash_riscv_k210.h"
#include "pg/pg.h"

typedef struct flashK210Config_s {
uint8_t spi_index;
uint32_t index;
uint8_t work_mode;
uint8_t data_bit_length;
uint8_t *manuf_id;
uint8_t *device_id;
uint8_t cs;
uint32_t addr;
uint8_t *reg_data;
uint8_t *data_buf;
uint32_t length;
} flashK210Config_t;
PG_DECLARE(flashK210Config_t, flashK210Config);