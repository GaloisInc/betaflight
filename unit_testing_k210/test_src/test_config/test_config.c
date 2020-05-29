/* Unittests for configs
 * Authors :nikolay nikolov
 * 			eschulte@pdx.edu
 */
#include <stddef.h>
#include "test_config.h"

uint32_t flashAddress = 0x00100000 + 0x8;

// flash_init - Pass: 0; Fail: 1
void test_flash_init (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, flash_init( 3, 0 ), "Function flash_init failed");
}
//loadEEPROMFromExternalFlash - Pass: 1; Fail: 0
void test_loadEEPROMFromExternalFlash (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, loadEEPROMFromExternalFlash(), "Function loadEEPROMFromExternalFlash failed");
}
//readEEPROM - Pass: 1; Fail: 0
void test_readEEPROM (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, readEEPROM(), "Function readEEPROM failed");
}
//loadEEPROM  - Pass: 1; Fail: 0
void test_loadEEPROM (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, loadEEPROM(), "Function loadEEPROM failed");
}
//isEEPROMStructureValid  - Pass: 1; Fail: 0
void test_isEEPROMStructureValid (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, isEEPROMStructureValid(), "Function isEEPROMStructureValid failed");
}
//isEEPROMVersionValid  - Pass: 1; Fail: 0
void test_isEEPROMVersionValid (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, isEEPROMVersionValid(), "Function isEEPROMVersionValid failed");
}
//writeSettingsToEEPROM - Pass: 0; Fail: !0
void test_writeSettingsToEEPROM (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, writeSettingsToEEPROM(), "Function writeSettingsToEEPROM failed");
}
//config_streamer_write - Pass: 0; Fail: !0
void test_config_streamer_write (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, config_streamer_write(flashAddress, buffer, 4096), "Function config_streamer_write failed");
}
//config_streamer_flush - Pass: 0; Fail: !0
void test_config_streamer_flush (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, config_streamer_flush(flashAddress), "Function config_streamer_flush failed");
}
//config_streamer_finish - Pass: 0; Fail: !0
void test_config_streamer_finish (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, config_streamer_finish(flashAddress), "Function config_streamer_finish failed");
}
void test_flash_write_data (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, flash_write_data(flashAddress, 8,4096), "Function flash_write_data failed");
}
