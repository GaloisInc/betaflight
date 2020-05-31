/* Unittests for configs
 * Authors :nikolay nikolov
 * 			eschulte@pdx.edu
 */
#include <stddef.h>
#include "test_config.h"

TEST_GROUP(test_config);
//sometimes you may want to get at local data in a module.
//for example: If you plan to pass by reference, this could be useful
//however, it should often be avoided
extern int Counter;

TEST_SETUP(test_config){

}

TEST_TEAR_DOWN(test_config ) {}
unsigned flashAddress = 0x8000ffff;
uint8_t buffer[8];

TEST(config,test1){
	// flash_init - Pass: 0; Fail: 1

	TEST_ASSERT_EQUAL_MESSAGE( 0, flash_init( 3, 0 ), "Function flash_init failed" );

	//loadEEPROMFromExternalFlash - Pass: 1; Fail: 0

// 	TEST_ASSERT_EQUAL_MESSAGE( 1, loadEEPROMFromExternalFlash(), "Function loadEEPROMFromExternalFlash failed" );

	//readEEPROM - Pass: 1; Fail: 0
	TEST_ASSERT_EQUAL_MESSAGE( 1, readEEPROM(), "Function readEEPROM failed" );

	//loadEEPROM  - Pass: 1; Fail: 0
	TEST_ASSERT_EQUAL_MESSAGE( 1, loadEEPROM(), "Function loadEEPROM failed" );

	//isEEPROMStructureValid  - Pass: 1; Fail: 0

	TEST_ASSERT_EQUAL_MESSAGE( 1, isEEPROMStructureValid(), "Function isEEPROMStructureValid failed" );
	//isEEPROMVersionValid  - Pass: 1; Fail: 0

	TEST_ASSERT_EQUAL_MESSAGE( 1, isEEPROMVersionValid(), "Function isEEPROMVersionValid failed" );

	//writeSettingsToEEPROM - Pass: 0; Fail: !0
	TEST_ASSERT_EQUAL_MESSAGE( 1, writeSettingsToEEPROM(), "Function writeSettingsToEEPROM failed" );

	//config_streamer_write - Pass: 0; Fail: !0
	TEST_ASSERT_EQUAL_MESSAGE( 0, config_streamer_write( flashAddress, buffer, 4096 ),
	                           "Function config_streamer_write failed" );

	//config_streamer_flush - Pass: 0; Fail: !0
	TEST_ASSERT_EQUAL_MESSAGE( 0, config_streamer_flush( flashAddress ), "Function config_streamer_flush failed" );

	//config_streamer_finish - Pass: 0; Fail: !0
	TEST_ASSERT_EQUAL_MESSAGE( 0, config_streamer_finish( flashAddress ),
							   "Function config_streamer_finish failed" );

	TEST_ASSERT_EQUAL_MESSAGE(0, flash_write_data(flashAddress, buffer,4096), "Function flash_write_data failed");
}
