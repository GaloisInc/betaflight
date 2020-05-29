
#include <setjmp.h>
#include <stdio.h>
#include "../../Unity/src/unity.h"
#include "../../../src/main/config/config_eeprom.h"
#include "../../../src/main/drivers/flash_riscv_k210.h"
#include "../../../src/main/target/MAIXBIT/target.h"
#include "../../../src/main/config/config.h"
#include "../../../src/main/rx/rx.h"
#include "../../../src/main/pg/pg.h"
#include "../../../src/main/config/feature.h"
#include "../../../src/main/pg/pg_ids.h"
#include "../../../src/main/config/config_streamer.h"
#include "../../../src/main/build/build_config.h"
#include "../../Unity/examples/unity_config.h"
#include "../../../src/main/common/crc.h"
#define RUN_TEST(TestFunc)                                             \
