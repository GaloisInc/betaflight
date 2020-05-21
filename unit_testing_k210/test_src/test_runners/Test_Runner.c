
/*=======Test Runner Used To Run Each Test Below=====*/
#define RUN_TEST(TestFunc) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT()) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

/*=======Automagically Detected Files To Include=====*/
#include "../../Unity/src/unity.h"
#include "../../Unity/examples/unity_config.h"
#include <setjmp.h>
#include <stdio.h>
//#include "../../../src/main/config/config_eeprom.h"
#include "../../../src/main/drivers/flash_riscv_k210.h"
#include "../../../src/main/config/config_eeprom.h"

//#include "../../../src/main/config/config.h"
//#include "../../../src/main/rx/rx.h"
//#include "../../../src/main/pg/pg.h"
//#include "../../../src/main/config/feature.h"

/*=======External Functions This Runner Calls=====*/
extern void setUp(void);
extern void tearDown(void);

//extern void test_isEEPROMVersionValid(void);
extern void test_flash_init (void);
//extern void test_writeConfigToEEPROM (void);
extern void test_loadEEPROMFromExternalFlash (void);
//extern void test_readEEPROM (void);

/*=======Test Reset Option=====*/
void resetTest(void);
void resetTest(void)
{
	tearDown();
	setUp();
}


/*=======MAIN=====*/
int main(void)
{
	UnityBegin("test_src/test_config/test_config.c");
  //RUN_TEST(test_isEEPROMVersionValid);
  RUN_TEST(test_flash_init);
  //RUN_TEST(test_writeConfigToEEPROM);
  RUN_TEST(test_loadEEPROMFromExternalFlash);
  //RUN_TEST(test_readEEPROM);

	return (UnityEnd());
}
