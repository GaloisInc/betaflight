 /* Unittests for configs
  * Author :nikolay nikolov
  */
 #include <stddef.h>
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
 #include "../../../src/main/drivers/flash_riscv_k210.h"
 #include "../../Unity/src/unity.h"

 // Test Fash hanshake
 void test_flash_read_id () {
	 uint8_t* manuf_id;
	 uint8_t* device_id ;
	 TEST_ASSERT_EQUAL_MESSAGE(0, flash_read_id(&manuf_id, &device_id ),"Testing handshake");
 }
 void test_flash_is_busy (void) {
	 TEST_ASSERT_EQUAL_MESSAGE(0, flash_is_busy(), "Testing  if flash is busy");
 }
 void test_flash_chip_erase( void ) {
	 TEST_ASSERT_EQUAL_MESSAGE(0, flash_chip_erase(), "Testing erasing the entire chip");
 }
 void test_flash_sector_erase(void){
	 uint32_t addr = 0x00150000;
	 TEST_ASSERT_EQUAL_MESSAGE(0, flash_sector_erase(addr), "Testing erasing a sector");
}
void test_flash_32k_block_erase(void){
	uint32_t addr = 0x00150000;
	TEST_ASSERT_EQUAL_MESSAGE(0, flash_32k_block_erase(addr), "Testing erasing a sector of 32k");
}
void test_flash_write_data(void){
	int32_t addr = 0x00150000;
	uint32_t DATA=(1024+1024);
	uint32_t length =1024;
	TEST_ASSERT_EQUAL_MESSAGE(0, flash_write_data(addr,DATA,length), "Testing writing data to flash");
}
void test_flash_read_data (void ){
	uint32_t addr = 0x00150000;
	uint32_t length =1024;
	uint8_t* data_buf[length];
	TEST_ASSERT_EQUAL_MESSAGE(0, flash_read_data(addr,data_buf,length,FLASH_STANDARD_FAST), "Testing reading ");
}
