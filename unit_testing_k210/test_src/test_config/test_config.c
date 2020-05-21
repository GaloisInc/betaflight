/* Unittests for configs
 * Author :nikolay nikolov
 */
#include <stddef.h>
#include "../../Unity/src/unity.h"
//#include "../../../src/main/config/config_eeprom.h"

#include "../../../src/main/drivers/flash_riscv_k210.h"
#include "../../../src/main/config/config_eeprom.h"

//#include "../../../src/main/config/config.h"
//#include "../../../src/main/rx/rx.h"
//#include "../../../src/main/pg/pg.h"
//#include "../../../src/main/config/feature.h"
//#include "../../../src/main/pg/pg_ids.h"



/* Variables ---------------------------*/
//configRecord_t = {1024, 2042, 1, 0, PG_RISCV_K210};
/* -------------------------------------*/

void setUp ( void ) {
	// set stuff up here
}

void tearDown ( void ) {
	// clean stuff up here
}
/*
// Example test
void test_isEEPROMVersionValid (void) {
	TEST_ASSERT_EQUAL(0, isEEPROMVersionValid());
}
*/

// Check if K210 flash gets iniialized - init.c 454: flash_init(3,0)
void test_flash_init (void) {
	TEST_ASSERT_EQUAL_MESSAGE(0, flash_init( 3, 0 ), "Flash init failed");
}
/*
//Check if configs get written to EEPROM
void test_writeConfigToEEPROM (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, writeConfigToEEPROM(), "Flash write config to eeprom failed");
}
*/

//Check if load EEPROM from external flash
void test_loadEEPROMFromExternalFlash (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, loadEEPROMFromExternalFlash(), "Load EEPROM failed");
}
/*
//Check if 
void test_readEEPROM (void) {
	TEST_ASSERT_EQUAL_MESSAGE(1, readEEPROM(), "Read from EEPROM failed");
}
*/
/*
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_isEEPROMVersionValid);
    return UNITY_END();
}
*/