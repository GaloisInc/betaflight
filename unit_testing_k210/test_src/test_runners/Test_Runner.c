#include "Test_Runner.h"


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
#include "Test_Runner.h"
#define TEST
/*=======External Functions This Runner Calls=====*/
extern void setUp(void);
extern void tearDown(void);
extern void test_flash_init (void);
extern void test_loadEEPROMFromExternalFlash (void);
extern void test_readEEPROM (void);
extern void test_loadEEPROM (void);
extern void test_isEEPROMStructureValid (void);
extern void test_isEEPROMVersionValid (void);
extern void test_writeSettingsToEEPROM (void);
extern void test_write_word (void);
extern void test_config_streamer_write (void);
extern void test_config_streamer_flush (void);
extern void test_config_streamer_finish (void);
extern void test_flash_write_data (void);
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
  RUN_TEST(test_flash_init);
  RUN_TEST(test_loadEEPROMFromExternalFlash);
  RUN_TEST(test_readEEPROM);
  RUN_TEST(test_loadEEPROM);
  RUN_TEST(test_isEEPROMStructureValid);
  RUN_TEST(test_isEEPROMVersionValid);
  RUN_TEST(test_writeSettingsToEEPROM);
  RUN_TEST(test_config_streamer_write);
  RUN_TEST(test_config_streamer_flush);
  RUN_TEST(test_config_streamer_finish);
  RUN_TEST(test_flash_write_data);
  UnityBegin("test_src/test_build/test_build.c");
  return (UnityEnd());



}
