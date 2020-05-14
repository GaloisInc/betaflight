/* Unittests for configs
 * Author :nikolay nikolov
 */
#include <stddef.h>
#include "../../../src/main/config/config_eeprom.h"
#include "../../Unity/src/unity.h"
#include "../../../src/main/pg/pg.h"
#include "../../../src/main/pg/pg_ids.h"



/* Variables ---------------------------*/
//configRecord_t = {1024, 2042, 1, 0, PG_RISCV_K210};
/* -------------------------------------*/

void setUp ( void ) {
	// set stuff up here
}

void tearDown ( void ) {
	// clean stuff up here
}

void test_isEEPROMVersionValid (void) {
	TEST_ASSERT_EQUAL(0, isEEPROMVersionValid());
}
/*
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_isEEPROMVersionValid);
    return UNITY_END();
}
*/
