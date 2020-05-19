
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
#include "../../../src/main/config/config_eeprom.h"

/*=======External Functions This Runner Calls=====*/
extern void setUp(void);
extern void tearDown(void);
extern void test_isEEPROMVersionValid(void);

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
  RUN_TEST(test_isEEPROMVersionValid);

	return (UnityEnd());
}
