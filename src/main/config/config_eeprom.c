/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <stdio.h>

#include "platform.h"

#include "build/build_config.h"

#include "common/crc.h"
#include "common/utils.h"

#include "config/config_eeprom.h"
#include "config/config_streamer.h"
#include "pg/pg.h"
#include "config/config.h"

// temp for debugging
#include "capstone_print.h"

#ifdef CONFIG_IN_SDCARD
#include "io/asyncfatfs/asyncfatfs.h"
#endif

#include "drivers/flash.h"
#include "drivers/system.h"

// riscv files
#include "drivers/flash_riscv_k210.h"

static uint16_t eepromConfigSize;

typedef enum {
    CR_CLASSICATION_SYSTEM   = 0,
    CR_CLASSICATION_PROFILE_LAST = CR_CLASSICATION_SYSTEM,
} configRecordFlags_e;

#define CR_CLASSIFICATION_MASK  (0x3)
#define CRC_START_VALUE         0xFFFF

#ifdef RISCV_K210
#define CRC_CHECK_VALUE         0xD5E2  // pre-calculated value of CRC that includes the CRC itself
#else
#define CRC_CHECK_VALUE         0x1D0F  // pre-calculated value of CRC that includes the CRC itself
#endif

// Header for the saved copy.
typedef struct {
    uint8_t eepromConfigVersion;
    uint8_t magic_be;           // magic number, should be 0xBE
} PG_PACKED configHeader_t;

// Header for each stored PG.
typedef struct {
    // split up.
    uint16_t size;
    pgn_t pgn;
    uint8_t version;

    // lower 2 bits used to indicate system or profile number, see CR_CLASSIFICATION_MASK
    uint8_t flags;

    uint8_t pg[];
} PG_PACKED configRecord_t;

// Footer for the saved copy.
typedef struct {
    uint16_t terminator;
} PG_PACKED configFooter_t;
// checksum is appended just after footer. It is not included in footer to make checksum calculation consistent

// Used to check the compiler packing at build time.
typedef struct {
    uint8_t byte;
    uint32_t word;
} PG_PACKED packingTest_t;

#if defined(CONFIG_IN_EXTERNAL_FLASH)
#ifdef RISCV_K210
bool loadEEPROMFromExternalFlash(void)
{
    // got this address from one of the example from k210...not sure if this is right
    uint32_t flashStartAddress = FLASH_START_ADDR;
    uint32_t totalBytesRead = 0;
    int success = 0;

    // flash_read_data will return FLASH_OK = 0 if successful
    success = flash_read_data ( flashStartAddress, &eepromData[totalBytesRead], EEPROM_SIZE, FLASH_STANDARD );

    return !success;
}
#else
bool loadEEPROMFromExternalFlash(void)
{
    const flashPartition_t *flashPartition = flashPartitionFindByType(FLASH_PARTITION_TYPE_CONFIG);
    const flashGeometry_t *flashGeometry = flashGetGeometry();

    uint32_t totalBytesRead = 0;
    int bytesRead = 0;

    bool success = false;

    do {
        bytesRead = flashReadBytes(flashStartAddress + totalBytesRead, &eepromData[totalBytesRead], EEPROM_SIZE - totalBytesRead);

        if (bytesRead > 0) {
            totalBytesRead += bytesRead;
            success = (totalBytesRead == EEPROM_SIZE);
        }
    } while (!success && bytesRead > 0);

    return success;
}
#endif
#elif defined(CONFIG_IN_SDCARD)

enum {
    FILE_STATE_NONE = 0,
    FILE_STATE_BUSY = 1,
    FILE_STATE_FAILED,
    FILE_STATE_COMPLETE,
};

uint8_t fileState = FILE_STATE_NONE;

const char *defaultSDCardConfigFilename = "CONFIG.BIN";

void saveEEPROMToSDCardCloseContinue(void)
{
    if (fileState != FILE_STATE_FAILED) {
        fileState = FILE_STATE_COMPLETE;
    }
}

void saveEEPROMToSDCardWriteContinue(afatfsFilePtr_t file)
{
    if (!file) {
        fileState = FILE_STATE_FAILED;
        return;
    }

    uint32_t totalBytesWritten = 0;
    uint32_t bytesWritten = 0;
    bool success;

    do {
        bytesWritten = afatfs_fwrite(file, &eepromData[totalBytesWritten], EEPROM_SIZE - totalBytesWritten);
        totalBytesWritten += bytesWritten;
        success = (totalBytesWritten == EEPROM_SIZE);

        afatfs_poll();
    } while (!success && afatfs_getLastError() == AFATFS_ERROR_NONE);

    if (!success) {
        fileState = FILE_STATE_FAILED;
    }

    while (!afatfs_fclose(file, saveEEPROMToSDCardCloseContinue)) {
        afatfs_poll();
    }
}

bool saveEEPROMToSDCard(void)
{
    fileState = FILE_STATE_BUSY;
    bool result = afatfs_fopen(defaultSDCardConfigFilename, "w+", saveEEPROMToSDCardWriteContinue);
    if (!result) {
        return false;
    }

    while (fileState == FILE_STATE_BUSY) {
        afatfs_poll();
    }

    while (!afatfs_flush()) {
        afatfs_poll();
    };

    return (fileState == FILE_STATE_COMPLETE);
}

void loadEEPROMFromSDCardCloseContinue(void)
{
    if (fileState != FILE_STATE_FAILED) {
        fileState = FILE_STATE_COMPLETE;
    }
}

void loadEEPROMFromSDCardReadContinue(afatfsFilePtr_t file)
{
    if (!file) {
        fileState = FILE_STATE_FAILED;
        return;
    }

    fileState = FILE_STATE_BUSY;

    uint32_t totalBytesRead = 0;
    uint32_t bytesRead = 0;
    bool success;

    if (afatfs_feof(file)) {
        // empty file, nothing to load.
        memset(eepromData, 0x00, EEPROM_SIZE);
        success = true;
    } else {

        do {
            bytesRead = afatfs_fread(file, &eepromData[totalBytesRead], EEPROM_SIZE - totalBytesRead);
            totalBytesRead += bytesRead;
            success = (totalBytesRead == EEPROM_SIZE);

            afatfs_poll();
        } while (!success && afatfs_getLastError() == AFATFS_ERROR_NONE);
    }

    if (!success) {
        fileState = FILE_STATE_FAILED;
    }

    while (!afatfs_fclose(file, loadEEPROMFromSDCardCloseContinue)) {
        afatfs_poll();
    }

    return;
}

bool loadEEPROMFromSDCard(void)
{
    fileState = FILE_STATE_BUSY;
    // use "w+" mode here to ensure the file is created now - in w+ mode we can read and write and the seek position is 0 on existing files, ready for reading.
    bool result = afatfs_fopen(defaultSDCardConfigFilename, "w+", loadEEPROMFromSDCardReadContinue);
    if (!result) {
        return false;
    }

    while (fileState == FILE_STATE_BUSY) {
        afatfs_poll();
    }

    return (fileState == FILE_STATE_COMPLETE);
}
#endif

#ifdef CONFIG_IN_FILE
void loadEEPROMFromFile(void) {
    FLASH_Unlock(); // load existing config file into eepromData
}
#endif

void initEEPROM(void)
{
    // Verify that this architecture packs as expected.
    STATIC_ASSERT(offsetof(packingTest_t, byte) == 0, byte_packing_test_failed);
    STATIC_ASSERT(offsetof(packingTest_t, word) == 1, word_packing_test_failed);
    STATIC_ASSERT(sizeof(packingTest_t) == 5, overall_packing_test_failed);

    STATIC_ASSERT(sizeof(configFooter_t) == 2, footer_size_failed);
    STATIC_ASSERT(sizeof(configRecord_t) == 6, record_size_failed);

#if defined(CONFIG_IN_FILE)
    loadEEPROMFromFile();
#elif defined(CONFIG_IN_EXTERNAL_FLASH)
    bool eepromLoaded = loadEEPROMFromExternalFlash();
    char buffer[200];
    sprintf(buffer, "Loaded EEPROM from flash - %s", eepromLoaded ? "successful" : "unsuccessful");
    print_my_msg(buffer, __FUNCTION__, __FILE__, __LINE__);
    if (!eepromLoaded) {
        // Flash read failed - just die now
        printf("%s:%s:%d - Flash read failed \n\n", __FUNCTION__,__FILE__,__LINE__);
        failureMode(FAILURE_FLASH_READ_FAILED);
    }
#elif defined(CONFIG_IN_SDCARD)
    bool eepromLoaded = loadEEPROMFromSDCard();
    if (!eepromLoaded) {
        // SDCard read failed - just die now
        failureMode(FAILURE_SDCARD_READ_FAILED);
    }
#endif
}

bool isEEPROMVersionValid(void)
{
    const uint8_t *p = &__config_start;
    const configHeader_t *header = (const configHeader_t *)p;

    if (header->eepromConfigVersion != EEPROM_CONF_VERSION) {
        print_my_msg("EEPROM version valid - unsuccessful", __FUNCTION__, __FILE__, __LINE__);
        return false;
    }

    print_my_msg("EEPROM version valid - successful", __FUNCTION__, __FILE__, __LINE__);
    return true;
}

// Scan the EEPROM config. Returns true if the config is valid.
bool isEEPROMStructureValid(void)
{
    const uint8_t *p = &__config_start;
    const configHeader_t *header = (const configHeader_t *)p;

    if (header->magic_be != 0xBE) {
        print_my_msg("No magic_be (0xBE) in header file", __FUNCTION__, __FILE__, __LINE__);
        return false;
    }

    uint16_t crc = CRC_START_VALUE;
    crc = crc16_ccitt_update(crc, header, sizeof(*header));
    p += sizeof(*header);

    for (;;) {
        const configRecord_t *record = (const configRecord_t *)p;

        if (record->size == 0) {
            // Found the end.  Stop scanning.
            break;
        }

        if (p + record->size >= &__config_end
            || record->size < sizeof(*record)) {
            // Too big or too small.
            return false;
        }

        crc = crc16_ccitt_update(crc, p, record->size);

        p += record->size;
    }

    const configFooter_t *footer = (const configFooter_t *)p;
    crc = crc16_ccitt_update(crc, footer, sizeof(*footer));
    p += sizeof(*footer);

    // include stored CRC in the CRC calculation
    const uint16_t *storedCrc = (const uint16_t *)p;
    crc = crc16_ccitt_update(crc, storedCrc, sizeof(*storedCrc));
    p += sizeof(storedCrc);

    eepromConfigSize = p - &__config_start;

    // CRC has the property that if the CRC itself is included in the calculation the resulting CRC will have constant value
    return crc == CRC_CHECK_VALUE;
}

uint16_t getEEPROMConfigSize(void)
{
    return eepromConfigSize;
}

size_t getEEPROMStorageSize(void)
{
#if defined(CONFIG_IN_EXTERNAL_FLASH)
    const flashPartition_t *flashPartition = flashPartitionFindByType(FLASH_PARTITION_TYPE_CONFIG);
    return FLASH_PARTITION_SECTOR_COUNT(flashPartition) * flashGetGeometry()->sectorSize;
#endif
#ifdef CONFIG_IN_RAM
    return EEPROM_SIZE;
#else
    return &__config_end - &__config_start;
#endif
}

// find config record for reg + classification (profile info) in EEPROM
// return NULL when record is not found
// this function assumes that EEPROM content is valid
static const configRecord_t *findEEPROM(const pgRegistry_t *reg, configRecordFlags_e classification)
{
    const uint8_t *p = &__config_start;
    p += sizeof(configHeader_t);             // skip header
    while (true) {
        const configRecord_t *record = (const configRecord_t *)p;
        if (record->size == 0) {
            //print_my_msg("NOTHING", __FUNCTION__, __FILE__, __LINE__);
            break;
        }
        if( p + record->size >= &__config_end) {
            //print_my_msg("too BIG", __FUNCTION__, __FILE__, __LINE__);
            break;
        }

        if(record->size < sizeof(*record)) {
            //print_my_msg("too small", __FUNCTION__, __FILE__, __LINE__);
            break;
        }

        if (record->size == 0
            || p + record->size >= &__config_end
            || record->size < sizeof(*record))
            break;

        if (pgN(reg) == record->pgn
            && (record->flags & CR_CLASSIFICATION_MASK) == classification) {
            //print_my_msg("inside right", __FUNCTION__, __FILE__, __LINE__);
            return record;
        }
        p += record->size;
    }
    // record not found
    return NULL;
}

// Initialize all PG records from EEPROM.
// This functions processes all PGs sequentially, scanning EEPROM for each one. This is suboptimal,
//   but each PG is loaded/initialized exactly once and in defined order.
bool loadEEPROM(void)
{
    bool success = true;

    PG_FOREACH(reg) {
        const configRecord_t *rec = findEEPROM(reg, CR_CLASSICATION_SYSTEM);
        if (rec) {
            // config from EEPROM is available, use it to initialize PG. pgLoad will handle version mismatch
            if (!pgLoad(reg, rec->pg, rec->size - offsetof(configRecord_t, pg), rec->version)) {

                print_my_msg("Unable to load pgs from EEPROM", __FUNCTION__, __FILE__, __LINE__);
                success = false;
            }
        } else {
            pgReset(reg);
            printf("Config unavailable from EEPROM - pgn %d initialized\n", (reg->pgn));
            //printf("Writing to Flash - PG - pgn %d | size %d | version %d | addr of group in RAM %x\n", record.pgn, record.size, record.version, *(reg->address));
            success = false;
        }
    }

    char buffer[200];
    sprintf(buffer, "Loading EEPROM complete - %s", success ? "successful" : "unsuccessful");
    print_my_msg(buffer, __FUNCTION__, __FILE__, __LINE__);
    return success;
}

static bool writeSettingsToEEPROM(void)
{
    config_streamer_t streamer;
    config_streamer_init(&streamer);

    config_streamer_start(&streamer, (uintptr_t)&__config_start, &__config_end - &__config_start);

    configHeader_t header = {
        .eepromConfigVersion =  EEPROM_CONF_VERSION,
        .magic_be =             0xBE,
    };

    printf("Config Header includes EEPROM_CONF_VERSION = %d and magic_be = %x \n", header.eepromConfigVersion, header.magic_be);
    config_streamer_write(&streamer, (uint8_t *)&header, sizeof(header));
    uint16_t crc = CRC_START_VALUE;
    crc = crc16_ccitt_update(crc, (uint8_t *)&header, sizeof(header));
    PG_FOREACH(reg) {
        const uint16_t regSize = pgSize(reg);
        configRecord_t record = {
            .size = sizeof(configRecord_t) + regSize,
            .pgn = pgN(reg),
            .version = pgVersion(reg),
            .flags = 0
        };

        record.flags |= CR_CLASSICATION_SYSTEM;
        config_streamer_write(&streamer, (uint8_t *)&record, sizeof(record));
        crc = crc16_ccitt_update(crc, (uint8_t *)&record, sizeof(record));
        config_streamer_write(&streamer, reg->address, regSize);
        crc = crc16_ccitt_update(crc, reg->address, regSize);
        printf("Writing to Flash - PG - pgn %d | size %d | version %d | addr of group in RAM %x\n", record.pgn, record.size, record.version, *(reg->address));
    }

    configFooter_t footer = {
        .terminator = 0,
    };

    config_streamer_write(&streamer, (uint8_t *)&footer, sizeof(footer));
    crc = crc16_ccitt_update(crc, (uint8_t *)&footer, sizeof(footer));

    // include inverted CRC in big endian format in the CRC
    const uint16_t invertedBigEndianCrc = ~(((crc & 0xFF) << 8) | (crc >> 8));
    config_streamer_write(&streamer, (uint8_t *)&invertedBigEndianCrc, sizeof(crc));

    config_streamer_flush(&streamer);

    const bool success = config_streamer_finish(&streamer) == 0;

    return success;
}

void writeConfigToEEPROM(void)
{
    bool success = false;
    char buffer_k210[200];
    sprintf(buffer_k210, "Writing to flash at starting address %x", FLASH_START_ADDR);
    print_my_msg(buffer_k210, __FUNCTION__, __FILE__, __LINE__);
    // write it
    for (int attempt = 0; attempt < 3 && !success; attempt++) {
        if (writeSettingsToEEPROM()) {
            success = true;

    sprintf(buffer_k210, "Flash write complete - %s", success ? "successful" : "unsuccessful");
    print_my_msg(buffer_k210, __FUNCTION__, __FILE__, __LINE__);

#ifdef CONFIG_IN_EXTERNAL_FLASH
            // copy it back from flash to the in-memory buffer.
            success = loadEEPROMFromExternalFlash();
            char buffer[200];
            sprintf(buffer, "Loaded EEPROM from flash - %s", success ? "successful" : "unsuccessful");
            print_my_msg(buffer, __FUNCTION__, __FILE__, __LINE__);
#endif
#ifdef CONFIG_IN_SDCARD
            // copy it back from flash to the in-memory buffer.
            success = loadEEPROMFromSDCard();
#endif
        }
    }

    if (success && isEEPROMVersionValid() && isEEPROMStructureValid()) {
        return;
    }

    // Flash write failed - just die now
    failureMode(FAILURE_CONFIG_STORE_FAILURE);
}
