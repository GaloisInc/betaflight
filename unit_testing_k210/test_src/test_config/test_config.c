/* Unittests for configs
 * Author :nikolay nikolov
 */
#include <stddef.h>
#include "../../Unity/src/unity.h"
//#include "../../../src/main/config/config_eeprom.h"

#include "../../../src/main/drivers/flash_riscv_k210.h"
#include "../../../src/main/config/config_eeprom.h"

#include "../../../src/main/target/MAIXBIT/target.h"
#include "../../../src/main/config/config.h"
#include "../../../src/main/rx/rx.h"
#include "../../../src/main/pg/pg.h"
#include "../../../src/main/config/feature.h"
#include "../../../src/main/pg/pg_ids.h"

#include "../../../src/main/config/config_streamer.h"
#include "../../../src/main/pg/pg.h"
#include "../../../src/main/common/crc.h"

void setUp ( void ) {
	// optional set stuff up here
}

void tearDown ( void ) {
	// optional clean stuff up here
}

// flash_init - Pass: 0; Fail: 1
void test_flash_init (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, flash_init( 3, 0 ), "Function flash_init failed");
}

//Prototype function
bool loadEEPROMFromExternalFlash(void);
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

//Prototype function
/*static*/ bool writeSettingsToEEPROM(void);
//writeSettingsToEEPROM - Pass: 0; Fail: !0
void test_writeSettingsToEEPROM (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, writeSettingsToEEPROM(), "Function writeSettingsToEEPROM failed");
}

//write_word  - Pass: 0; Fail: -3
config_streamer_t *c;
/*static*/ int write_word(config_streamer_t *c, config_streamer_buffer_align_type_t *buffer);
/*
void test_write_word (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, write_word(c, &c->buffer.w), "Function write_word failed");
}
*/
//config_streamer_write - Pass: 0; Fail: !0
typedef struct {
    uint8_t eepromConfigVersion;
    uint8_t magic_be;           // magic number, should be 0xBE
} PG_PACKED configHeader_t;

config_streamer_t streamer;

configHeader_t header = {
        .eepromConfigVersion =  EEPROM_CONF_VERSION,
        .magic_be =             0xBE,
    };
void test_config_streamer_write (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, config_streamer_write(&streamer, (uint8_t *)&header, sizeof(header)), "Function config_streamer_write failed");
}

//config_streamer_flush - Pass: 0; Fail: !0
void test_config_streamer_flush (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, config_streamer_flush(&streamer), "Function config_streamer_flush failed");
}

//config_streamer_finish - Pass: 0; Fail: !0
void test_config_streamer_finish (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, config_streamer_finish(&streamer), "Function config_streamer_finish failed");
}
/*
//flash_write_data - Pass: 0; Fail: !0
//uint8_t eepromData[EEPROM_SIZE];

uint32_t dataOffset = (uint32_t)(c->address - (uintptr_t)&eepromData[0]);
    uint32_t flashStartAddress = FLASH_START_ADDR;

uint32_t flashAddress = flashStartAddress + dataOffset;

char buffer[200];
void test_flash_write_data (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, flash_write_data(flashAddress, (uint8_t *)buffer, CONFIG_STREAMER_BUFFER_SIZE), "Function flash_write_data failed");
}
*/

//Don't need if running from a test runner
/*
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_isEEPROMVersionValid);
    return UNITY_END();
}
*/