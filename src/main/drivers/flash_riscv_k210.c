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
#include "flash_riscv_k210.h"
#include "riscv_k210_dmac.h"
#include "riscv_k210_fpioa.h"
#include "riscv_k210_spi.h"
#include "riscv_k210_sysctl.h"
#include <stdio.h>
uint32_t spi_bus_no = 0;
uint32_t spi_chip_select = 0;
riscv_k210_w25q_t ( *flash_page_program_fun )( uint32_t addr, uint8_t* data_buf, uint32_t length );
riscv_k210_w25q_t ( *flash_read_fun )( uint32_t addr, uint8_t* data_buf, uint32_t length );
static riscv_k210_w25q_t flash_stand_read_data( uint32_t addr, uint8_t* data_buf, uint32_t length );
static riscv_k210_w25q_t flash_quad_read_data( uint32_t addr, uint8_t* data_buf, uint32_t length );
static riscv_k210_w25q_t flash_page_program( uint32_t addr, uint8_t* data_buf, uint32_t length );
static riscv_k210_w25q_t flash_quad_page_program( uint32_t addr, uint8_t* data_buf, uint32_t length );
//----------------------------------------------------------------------------------------------------------- //
static riscv_k210_w25q_t flash_receive_data( uint8_t* cmd_buff, uint8_t cmd_len, uint8_t* rx_buff, uint32_t rx_len ) {
	spi_init( spi_bus_no, SPI_WORK_MODE_0, SPI_FF_STANDARD, DATALENGTH, 0 );
	spi_receive_data_standard( spi_bus_no, spi_chip_select, cmd_buff, cmd_len, rx_buff, rx_len );
	return FLASH_OK;
}
static riscv_k210_w25q_t flash_send_data( uint8_t* cmd_buff, uint8_t cmd_len, uint8_t* tx_buff, uint32_t tx_len ) {
	spi_init( spi_bus_no, SPI_WORK_MODE_0, SPI_FF_STANDARD, DATALENGTH, 0 );
	spi_send_data_standard( spi_bus_no, spi_chip_select, cmd_buff, cmd_len, tx_buff, tx_len );
	return FLASH_OK;
}
static riscv_k210_w25q_t flash_receive_data_enhanced( uint32_t* cmd_buff, uint8_t cmd_len, uint8_t* rx_buff,
		uint32_t rx_len ) {
	spi_receive_data_multiple( spi_bus_no, spi_chip_select, cmd_buff, cmd_len, rx_buff, rx_len );
	return FLASH_OK;
}
static riscv_k210_w25q_t flash_send_data_enhanced( uint32_t* cmd_buff, uint8_t cmd_len, uint8_t* tx_buff,
		uint32_t tx_len ) {
	spi_send_data_multiple( spi_bus_no, spi_chip_select, cmd_buff, cmd_len, tx_buff, tx_len );
	return FLASH_OK;
}
riscv_k210_w25q_t flash_init( uint8_t spi_index, uint8_t spi_ss ) {
	spi_bus_no = spi_index;
	spi_chip_select = spi_ss;
	spi_init( spi_bus_no, SPI_WORK_MODE_0, SPI_FF_STANDARD, DATALENGTH, 0 );
	uint32_t ret = spi_set_clk_rate( spi_bus_no, 10000000 );
	//printf ("SPI_%d CLK is %d\n", spi_bus_no, ret);
	flash_page_program_fun = flash_page_program;
	flash_read_fun = flash_stand_read_data;
	return FLASH_OK;
}
riscv_k210_w25q_t flash_read_id( uint8_t* manuf_id, uint8_t* device_id ) {
	uint8_t cmd[ 4 ] = { READ_ID, 0x00, 0x00, 0x00 };
	uint8_t data[ 2 ] = { 0 };
	flash_receive_data( cmd, 4, data, 2 );
	*manuf_id = data[ 0 ];
	*device_id = data[ 1 ];
	return FLASH_OK;
}
static riscv_k210_w25q_t flash_write_enable( void ) {
	uint8_t cmd[ 1 ] = { WRITE_ENABLE };
	flash_send_data( cmd, 1, 0, 0 );
	return FLASH_OK;
}
riscv_k210_w25q_t flash_write_status_reg( uint8_t reg1_data, uint8_t reg2_data ) {
	uint8_t cmd[ 3 ] = { WRITE_REG1, reg1_data, reg2_data };
	flash_write_enable();
	flash_send_data( cmd, 3, 0, 0 );
	return FLASH_OK;
}
riscv_k210_w25q_t flash_read_status_reg1( uint8_t* reg_data ) {
	uint8_t cmd[ 1 ] = { READ_REG1 };
	uint8_t data[ 1 ] = { 0 };
	flash_receive_data( cmd, 1, data, 1 );
	*reg_data = data[ 0 ];
	return FLASH_OK;
}
riscv_k210_w25q_t flash_read_status_reg2( uint8_t* reg_data ) {
	uint8_t cmd[ 1 ] = { READ_REG2 };
	uint8_t data[ 1 ] = { 0 };
	flash_receive_data( cmd, 1, data, 1 );
	*reg_data = data[ 0 ];
	return FLASH_OK;
}
riscv_k210_w25q_t flash_is_busy( void ) {
	uint8_t status = 0;
	flash_read_status_reg1( &status );
	if( status & REG1_BUSY_MASK ) { return FLASH_BUSY; }
	return FLASH_OK;
}
riscv_k210_w25q_t flash_sector_erase( uint32_t addr ) {
	uint8_t cmd[ 4 ] = { SECTOR_ERASE };
	cmd[ 1 ] = ( uint8_t )( addr >> 16 );
	cmd[ 2 ] = ( uint8_t )( addr >> 8 );
	cmd[ 3 ] = ( uint8_t )( addr );
	flash_write_enable();
	flash_send_data( cmd, 4, 0, 0 );
	return FLASH_OK;
}
riscv_k210_w25q_t flash_32k_block_erase( uint32_t addr ) {
	uint8_t cmd[ 4 ] = { BLOCK_32K_ERASE };
	cmd[ 1 ] = ( uint8_t )( addr >> 16 );
	cmd[ 2 ] = ( uint8_t )( addr >> 8 );
	cmd[ 3 ] = ( uint8_t )( addr );
	flash_write_enable();
	flash_send_data( cmd, 4, 0, 0 );
	return FLASH_OK;
}
riscv_k210_w25q_t flash_64k_block_erase( uint32_t addr ) {
	uint8_t cmd[ 4 ] = { BLOCK_64K_ERASE };
	cmd[ 1 ] = ( uint8_t )( addr >> 16 );
	cmd[ 2 ] = ( uint8_t )( addr >> 8 );
	cmd[ 3 ] = ( uint8_t )( addr );
	flash_write_enable();
	flash_send_data( cmd, 4, 0, 0 );
	return FLASH_OK;
}
riscv_k210_w25q_t flash_chip_erase( void ) {
	uint8_t cmd[ 1 ] = { CHIP_ERASE };
	flash_write_enable();
	flash_send_data( cmd, 1, 0, 0 );
	return FLASH_OK;
}
riscv_k210_w25q_t flash_enable_quad_mode( void ) {
	uint8_t reg_data = 0;
	flash_read_status_reg2( &reg_data );
	if( !( reg_data & REG2_QUAL_MASK ) ) {
		reg_data |= REG2_QUAL_MASK;
		flash_write_status_reg( 0x00, reg_data );
	}
	flash_page_program_fun = flash_quad_page_program;
	flash_read_fun = flash_quad_read_data;
	return FLASH_OK;
}
riscv_k210_w25q_t flash_disable_quad_mode( void ) {
	uint8_t reg_data = 0;
	flash_read_status_reg2( &reg_data );
	if( reg_data & REG2_QUAL_MASK ) {
		reg_data &= ( ~REG2_QUAL_MASK );
		flash_write_status_reg( 0x00, reg_data );
	}
	flash_page_program_fun = flash_page_program;
	flash_read_fun = flash_stand_read_data;
	return FLASH_OK;
}
riscv_k210_w25q_t flash_page_program( uint32_t addr, uint8_t* data_buf, uint32_t length ) {
	uint8_t cmd[ 4 ] = { PAGE_PROGRAM };
	cmd[ 1 ] = ( uint8_t )( addr >> 16 );
	cmd[ 2 ] = ( uint8_t )( addr >> 8 );
	cmd[ 3 ] = ( uint8_t )( addr );
	flash_write_enable();
	flash_send_data( cmd, 4, data_buf, length );
	while( flash_is_busy() == FLASH_BUSY ) {}
	return FLASH_OK;
}
static riscv_k210_w25q_t flash_quad_page_program( uint32_t addr, uint8_t* data_buf, uint32_t length ) {
	uint32_t cmd[ 2 ] = { 0 };
	cmd[ 0 ] = QUAD_PAGE_PROGRAM;
	cmd[ 1 ] = addr;
	flash_write_enable();
	spi_init( spi_bus_no, SPI_WORK_MODE_0, SPI_FF_QUAD, DATALENGTH, 0 );
	spi_init_non_standard( spi_bus_no, 8 /*instrction length*/, 24 /*address length*/, 0 /*wait cycles*/,
			SPI_AITM_STANDARD /*spi address trans mode*/ );
	flash_send_data_enhanced( cmd, 2, data_buf, length );
	while( flash_is_busy() == FLASH_BUSY ) {}
	return FLASH_OK;
}
static riscv_k210_w25q_t flash_sector_program( uint32_t addr, uint8_t* data_buf ) {
	uint8_t index = 0;
	for( index = 0; index < FLASH_PAGE_NUM_PER_SECTOR; index++ ) {
		flash_page_program_fun( addr, data_buf, FLASH_PAGE_SIZE );
		addr += FLASH_PAGE_SIZE;
		data_buf += FLASH_PAGE_SIZE;
	}
	return FLASH_OK;
}
riscv_k210_w25q_t flash_write_data( uint32_t addr, uint8_t* data_buf, uint32_t length ) {
	uint32_t sector_addr = 0;
	uint32_t sector_offset = 0;
	uint32_t sector_remain = 0;
	uint32_t write_len = 0;
	uint32_t index = 0;
	uint8_t* pread = NULL;
	uint8_t* pwrite = NULL;
	uint8_t swap_buf[ FLASH_SECTOR_SIZE ] = { 0 };
	while( length ) {
		sector_addr = addr & ( ~( FLASH_SECTOR_SIZE - 1 ) );
		sector_offset = addr & ( FLASH_SECTOR_SIZE - 1 );
		sector_remain = FLASH_SECTOR_SIZE - sector_offset;
		write_len = ( ( length < sector_remain ) ? length : sector_remain );
		flash_read_fun( sector_addr, swap_buf, FLASH_SECTOR_SIZE );
		pread = swap_buf + sector_offset;
		pwrite = data_buf;
		for( index = 0; index < write_len; index++ ) {
			if( ( *pwrite ) != ( ( *pwrite ) & ( *pread ) ) ) {
				flash_sector_erase( sector_addr );
				while( flash_is_busy() == FLASH_BUSY ) {}
				break;
			}
			pwrite++;
			pread++;
		}
		if( write_len == FLASH_SECTOR_SIZE ) {
			flash_sector_program( sector_addr, data_buf );
		} else {
			pread = swap_buf + sector_offset;
			pwrite = data_buf;
			for( index = 0; index < write_len; index++ ) { *pread++ = *pwrite++; }
			flash_sector_program( sector_addr, swap_buf );
		}
		length -= write_len;
		addr += write_len;
		data_buf += write_len;
	}
	return FLASH_OK;
}
riscv_k210_w25q_t flash_write_data_direct( uint32_t addr, uint8_t* data_buf, uint32_t length ) {
	uint32_t page_remain = 0;
	uint32_t write_len = 0;
	while( length ) {
		page_remain = FLASH_PAGE_SIZE - ( addr & ( FLASH_PAGE_SIZE - 1 ) );
		write_len = ( ( length < page_remain ) ? length : page_remain );
		flash_page_program_fun( addr, data_buf, write_len );
		length -= write_len;
		addr += write_len;
		data_buf += write_len;
	}
	return FLASH_OK;
}
riscv_k210_w25q_t _flash_read_data( uint32_t addr, uint8_t* data_buf, uint32_t length, flash_read_t mode ) {
	uint32_t cmd[ 2 ] = { 0 };
	switch( mode ) {
		case FLASH_STANDARD:
			*( ( ( uint8_t* )cmd ) + 0 ) = READ_DATA;
			*( ( ( uint8_t* )cmd ) + 1 ) = ( uint8_t )( addr >> 16 );
			*( ( ( uint8_t* )cmd ) + 2 ) = ( uint8_t )( addr >> 8 );
			*( ( ( uint8_t* )cmd ) + 3 ) = ( uint8_t )( addr >> 0 );
			flash_receive_data( ( uint8_t* )cmd, 4, data_buf, length );
			break;
		case FLASH_STANDARD_FAST:
			*( ( ( uint8_t* )cmd ) + 0 ) = FAST_READ;
			*( ( ( uint8_t* )cmd ) + 1 ) = ( uint8_t )( addr >> 16 );
			*( ( ( uint8_t* )cmd ) + 2 ) = ( uint8_t )( addr >> 8 );
			*( ( ( uint8_t* )cmd ) + 3 ) = ( uint8_t )( addr >> 0 );
			*( ( ( uint8_t* )cmd ) + 4 ) = 0xFF;
			flash_receive_data( ( uint8_t* )cmd, 5, data_buf, length );
			break;
		case FLASH_DUAL:
			cmd[ 0 ] = FAST_READ_DUAL_OUTPUT;
			cmd[ 1 ] = addr;
			spi_init( spi_bus_no, SPI_WORK_MODE_0, SPI_FF_DUAL, DATALENGTH, 0 );
			spi_init_non_standard( spi_bus_no, 8 /*instrction length*/, 24 /*address length*/,
					8 /*wait cycles*/, SPI_AITM_STANDARD /*spi address trans mode*/ );
			flash_receive_data_enhanced( cmd, 2, data_buf, length );
			break;
		case FLASH_DUAL_FAST:
			cmd[ 0 ] = FAST_READ_DUAL_IO;
			cmd[ 1 ] = addr << 8;
			spi_init( spi_bus_no, SPI_WORK_MODE_0, SPI_FF_DUAL, DATALENGTH, 0 );
			spi_init_non_standard( spi_bus_no, 8 /*instrction length*/, 32 /*address length*/,
					0 /*wait cycles*/, SPI_AITM_ADDR_STANDARD /*spi address trans mode*/ );
			flash_receive_data_enhanced( cmd, 2, data_buf, length );
			break;
		case FLASH_QUAD:
			cmd[ 0 ] = FAST_READ_QUAL_OUTPUT;
			cmd[ 1 ] = addr;
			spi_init( spi_bus_no, SPI_WORK_MODE_0, SPI_FF_QUAD, DATALENGTH, 0 );
			spi_init_non_standard( spi_bus_no, 8 /*instrction length*/, 24 /*address length*/,
					8 /*wait cycles*/, SPI_AITM_STANDARD /*spi address trans mode*/ );
			flash_receive_data_enhanced( cmd, 2, data_buf, length );
			break;
		case FLASH_QUAD_FAST:
			cmd[ 0 ] = FAST_READ_QUAL_IO;
			cmd[ 1 ] = addr << 8;
			spi_init( spi_bus_no, SPI_WORK_MODE_0, SPI_FF_QUAD, DATALENGTH, 0 );
			spi_init_non_standard( spi_bus_no, 8 /*instrction length*/, 32 /*address length*/,
					4 /*wait cycles*/, SPI_AITM_ADDR_STANDARD /*spi address trans mode*/ );
			flash_receive_data_enhanced( cmd, 2, data_buf, length );
			break;
	}
	return FLASH_OK;
}
riscv_k210_w25q_t flash_read_data( uint32_t addr, uint8_t* data_buf, uint32_t length, flash_read_t mode ) {
	uint32_t len = 0;
	while( length ) {
		len = ( ( length >= 0x010000 ) ? 0x010000 : length );
		_flash_read_data( addr, data_buf, len, mode );
		addr += len;
		data_buf += len;
		length -= len;
	}
	return FLASH_OK;
}
static riscv_k210_w25q_t flash_stand_read_data( uint32_t addr, uint8_t* data_buf, uint32_t length ) {
	return flash_read_data( addr, data_buf, length, FLASH_STANDARD_FAST );
}
static riscv_k210_w25q_t flash_quad_read_data( uint32_t addr, uint8_t* data_buf, uint32_t length ) {
	return flash_read_data( addr, data_buf, length, FLASH_QUAD_FAST );
}
riscv_k210_w25q_t flash_write_disable(void){
	uint8_t reg_data = 0;
	flash_read_status_reg2( &reg_data );
	if( reg_data & WRITE_DISABLE ) {
		reg_data &= ( ~WRITE_DISABLE );
		flash_write_status_reg( 0x00, reg_data );
	}
	flash_page_program_fun = flash_page_program;
	flash_read_fun = flash_stand_read_data;
	return FLASH_OK;
}
