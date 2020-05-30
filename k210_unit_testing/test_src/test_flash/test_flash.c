/* Unittests for build
 * Authors :nikolay nikolov
 * 			eschulte@pdx.edu
 */
#include <stddef.h>
#include "riscv_k210_sysctl.h"
#include "test_flash.h"
#define TEST_NUMBER ( 256 + 128 )
#define DATA_ADDRESS 0x130000
#define TARGET_BOARD_IDENTIFIER "MAIX"
uint8_t data_buf[ TEST_NUMBER ];
uint8_t manuf_id, device_id;
uint8_t spi_index;

TEST_GROUP(test_flash);
//sometimes you may want to get at local data in a module.
//for example: If you plan to pass by reference, this could be useful
//however, it should often be avoided
extern int Counter;

TEST_SETUP(test_flash){
	//This is run before EACH TEST
	Counter = 0x5a5a;
}

TEST_TEAR_DOWN(test_flash ) {}

TEST( test_flash, testing_all_flash_modules ) {
	char boardIdentifier[sizeof(TARGET_BOARD_IDENTIFIER) + 1];
	TEST_ASSERT_EQUAL( FLASH_OK, flash_init(3,0) );
}

