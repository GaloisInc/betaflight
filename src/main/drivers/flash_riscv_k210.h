/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#define USE_FLASH_RISCV_K210

#pragma message ( "inside flash_riscv_k210.h RUBEN" )
#include <stdint.h>
/* clang-format off */
#define DATALENGTH                          8
#define SPI_SLAVE_SELECT                    (0x01)
#define FLASH_PAGE_SIZE              256
#define FLASH_SECTOR_SIZE            4096
#define FLASH_PAGE_NUM_PER_SECTOR    16
#define FLASH_CHIP_SIZE              (16777216 UL)
#define WRITE_ENABLE                        0x06
#define WRITE_DISABLE                       0x04
#define READ_REG1                           0x05
#define READ_REG2                           0x35
#define READ_REG3                           0x15
#define WRITE_REG1                          0x01
#define WRITE_REG2                          0x31
#define WRITE_REG3                          0x11
#define READ_DATA                           0x03
#define FAST_READ                           0x0B
#define FAST_READ_DUAL_OUTPUT               0x3B
#define FAST_READ_QUAL_OUTPUT               0x6B
#define FAST_READ_DUAL_IO                   0xBB
#define FAST_READ_QUAL_IO                   0xEB
#define DUAL_READ_RESET                     0xFFFF
#define QUAL_READ_RESET                     0xFF
#define PAGE_PROGRAM                        0x02
#define QUAD_PAGE_PROGRAM                   0x32
#define SECTOR_ERASE                        0x20
#define BLOCK_32K_ERASE                     0x52
#define BLOCK_64K_ERASE                     0xD8
#define CHIP_ERASE                          0x60
#define READ_ID                             0x90
#define ENABLE_QPI                          0x38
#define EXIT_QPI                            0xFF
#define ENABLE_RESET                        0x66
#define RESET_DEVICE                        0x99
#define REG1_BUSY_MASK                      0x01
#define REG2_QUAL_MASK                      0x02
#define LETOBE(x)     ((x >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | (x << 24))
/* clang-format on */

/**
 * @brief      w25qxx operating status enumerate
 */
typedef enum _flash_status {
	FLASH_OK = 0, FLASH_BUSY, FLASH_ERROR,
} riscv_k210_w25q_t;
/**
 * @brief      w25qxx read operating enumerate
 */
typedef enum _flash_read {
	FLASH_STANDARD = 0, FLASH_STANDARD_FAST, FLASH_DUAL, FLASH_DUAL_FAST, FLASH_QUAD, FLASH_QUAD_FAST,
} flash_read_t;

riscv_k210_w25q_t flash_init ( uint8_t spi_index, uint8_t spi_ss );
riscv_k210_w25q_t flash_is_busy ( void );
riscv_k210_w25q_t flash_chip_erase ( void );
riscv_k210_w25q_t flash_enable_quad_mode ( void );
riscv_k210_w25q_t flash_disable_quad_mode ( void );
riscv_k210_w25q_t flash_sector_erase ( uint32_t addr );
riscv_k210_w25q_t flash_32k_block_erase ( uint32_t addr );
riscv_k210_w25q_t flash_64k_block_erase ( uint32_t addr );
riscv_k210_w25q_t flash_read_status_reg1 ( uint8_t*reg_data );
riscv_k210_w25q_t flash_read_status_reg2 ( uint8_t*reg_data );
riscv_k210_w25q_t flash_write_status_reg ( uint8_t reg1_data, uint8_t reg2_data );
riscv_k210_w25q_t flash_read_id ( uint8_t*manuf_id, uint8_t*device_id );
riscv_k210_w25q_t flash_write_data ( uint32_t addr, uint8_t*data_buf, uint32_t length );
riscv_k210_w25q_t flash_write_data_direct ( uint32_t addr, uint8_t*data_buf, uint32_t length );
riscv_k210_w25q_t flash_read_data ( uint32_t addr, uint8_t*data_buf, uint32_t length, flash_read_t mode );
riscv_k210_w25q_t flash_enable_xip_mode ( void );
riscv_k210_w25q_t flash_disable_xip_mode ( void );
riscv_k210_w25q_t flash_read_id_dma ( uint8_t*manuf_id, uint8_t*device_id );
riscv_k210_w25q_t flash_sector_erase_dma ( uint32_t addr );
riscv_k210_w25q_t flash_init_dma ( uint8_t spi_index, uint8_t spi_ss );
riscv_k210_w25q_t flash_write_data_dma ( uint32_t addr, uint8_t*data_buf, uint32_t length );
riscv_k210_w25q_t flash_write_data_direct_dma ( uint32_t addr, uint8_t*data_buf, uint32_t length );
riscv_k210_w25q_t flash_read_data_dma ( uint32_t addr, uint8_t*data_buf, uint32_t length, flash_read_t mode );
riscv_k210_w25q_t flash_is_busy_dma ( void );
riscv_k210_w25q_t flash_enable_quad_mode_dma ( void );


void ruben_flash();

