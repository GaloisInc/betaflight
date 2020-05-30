# ---------------------------------- //
# Unit Tests for the K210
#----------------------------------- //
# The target to build, see VALID_TARGETS below
TARGET   	 	:= MAIXBIT
FORKNAME  		:= BETAFLIGHT
REVISION 		:= RISCVK210

TEST ?= yes
# -----------------------------------//
ROOT 			= ../
TEST_DIR 		= unit_testing_k210
TEST_SRC		= $(notdir $(wildcard $(TEST_DIR)/test_src/*.c))
OBJECT_DIR 		= ./obj
TARGET_DIR     	= $(ROOT)/src/main/target/$(BASE_TARGET)
TARGET_DIR_SRC 	= $(notdir $(wildcard $(TARGET_DIR)/*.c))
MCU_COMMON_SRC  = $(notdir $(wildcard $(TEST_SRC)/test_runners/*.c))
#------------------------------------//
# Search path for sources
LD_FLAGS        :=
EXTRA_LD_FLAGS  :=
#------------------------------------//
# start specific includes
include $(ROOT)/make/targets.mk
include $(ROOT)/make/system-id.mk
include $(ROOT)/make/targets_list.mk
include $(ROOT)/make/mcu/$(TARGET_MCU).mk
MCU_FLASH_SIZE = $(TARGET_FLASH_SIZE)
VPATH 			:= $(VPATH):$(ROOT)/make/mcu
VPATH 			:= $(VPATH):$(ROOT)/make
# ---------------------------------//
VPATH 			:= 	$(VPATH):$(TARGET_DIR) \
					$(SRC_DIR):$(SRC_DIR)/startup \
					$(VPATH):$(ROOT)/make/mcu \
					$(VPATH):$(ROOT)/make \
					$(VPATH):$(USER_DIR):$(TEST_DIR) \
					$(VPATH):$(STDPERIPH_DIR) \
					$(VPATH):$(TEST_SRC) \
# ------------------------------------//
INCLUDE_DIRS    	:= 	$(INCLUDE_DIRS) \
					$(TARGET_DIR) \
					$(ROOT)/lib \
					$(ROOT)/src \
					$(ROOT)/src/main/$(wildcard *.h)
# ------------------------------------//
TARGET_FLASH_SIZE := $(MCU_FLASH_SIZE)
# ------------------------------------//
DEVICE_FLAGS  	:= $(DEVICE_FLAGS) -DTARGET_FLASH_SIZE=$(TARGET_FLASH_SIZE)
# ------------------------------------------------ //
.DEFAULT_GOAL 	:= binary
# -------------------------------------------------//
# RISCV Tools
RISCV64_SDK_PREFIX = /opt/kendryte-toolchain/bin/riscv64-unknown-elf-
CROSS_CC    	:= $(RISCV64_SDK_PREFIX)gcc
CROSS_CXX   	:= $(RISCV64_SDK_PREFIX)g++
CROSS_GDB   	:= $(RISCV64_SDK_PREFIX)-gdb
OBJCOPY    		:= $(RISCV64_SDK_PREFIX)objcopy
OBJDUMP     	:= $(RISCV64_SDK_PREFIX)objdump
SIZE        	:= $(RISCV64_SDK_PREFIX)size
# ------------------------------------------------ //
TARGET_BASENAME = $(OBJECT_DIR)/$(FORKNAME)_$(FC_VER)_$(TARGET)_$(REVISION)
TARGET_BIN      = $(TARGET_BASENAME).bin
TARGET_HEX      = $(TARGET_BASENAME).hex
TARGET_ELF      = $(OBJECT_DIR)/$(FORKNAME)_$(TARGET).elf
TARGET_EXST_ELF = $(OBJECT_DIR)/$(FORKNAME)_$(TARGET)_EXST.elf
TARGET_OBJS     = $(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(SRC))))
TARGET_DEPS     = $(addsuffix .d,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(SRC))))
TARGET_MAP      = $(OBJECT_DIR)/$(FORKNAME)_$(TARGET).map
# ------------------------------------------------- //
TARGET_EXST_HASH_SECTION_FILE = $(OBJECT_DIR)/$(TARGET)/exst_hash_section.bin
##------------------------------------------------- //
CLEAN_ARTIFACTS := $(TARGET_BIN)
CLEAN_ARTIFACTS += $(TARGET_HEX)
CLEAN_ARTIFACTS += $(TARGET_ELF) $(TARGET_OBJS) $(TARGET_MAP)
CLEAN_ARTIFACTS += $(TARGET_LST)
CLEAN_ARTIFACTS += $(TARGET_DFU)
# --------------------------------------------------//
FC_VER_MAJOR := $(shell grep " FC_VERSION_MAJOR" ../src/main/build/version.h | awk '{print $$3}' )
FC_VER_MINOR := $(shell grep " FC_VERSION_MINOR" ../src/main/build/version.h | awk '{print $$3}' )
FC_VER_PATCH := $(shell grep " FC_VERSION_PATCH" ../src/main/build/version.h | awk '{print $$3}' )
FC_VER := $(FC_VER_MAJOR).$(FC_VER_MINOR).$(FC_VER_PATCH)
# ------------------------------------------------- //
include $(ROOT)/make/source.mk
include test_src/test_source.mk
vpath %.c test_src
vpath %.h test_src
#------------------------------------------------- //
$(TARGET_LST): $(TARGET_ELF)
	$(V0) $(OBJDUMP) -S --disassemble $< > $@
$(TARGET_BIN): $(TARGET_ELF)
	@echo "Creating BIN $(TARGET_BIN)" "$(STDOUT)"
	$(V1) $(OBJCOPY) -O binary $< $@
$(TARGET_ELF): $(TARGET_OBJS) $(LD_SCRIPT)
	@echo "Linking $(TARGET)  " "$(STDOUT)"
	$(V1) $(CROSS_CC) -o $@ $(filter-out %.ld,$^) $(LD_FLAGS)
# Compile
## compile_file takes two arguments: (1) optimisation description string and (2) optimisation compiler flag
define compile_file
	echo "%% ($(1)) $<" "$(STDOUT)" && \
	$(CROSS_CC) -c -o $@ $(CFLAGS) $(2) $<
endef
ifeq ($(DEBUG),GDB)
$(OBJECT_DIR)/$(TARGET)/%.o: %.c
	$(V1) mkdir -p $(dir $@)
	$(V1) $(if $(findstring $<,$(NOT_OPTIMISED_SRC)), \
		$(call compile_file,not optimised, $(CC_NO_OPTIMISATION)) \
	, \
		$(call compile_file,debug,$(CC_DEBUG_OPTIMISATION)) \
	)
else
$(OBJECT_DIR)/$(TARGET)/%.o: %.c
	$(V1) mkdir -p $(dir $@)
	$(V1) $(if $(findstring $<,$(NOT_OPTIMISED_SRC)), \
		$(call compile_file,not optimised,$(CC_NO_OPTIMISATION)) \
	, \
		$(if $(findstring $(subst ./src/main/,,$<),$(SPEED_OPTIMISED_SRC)), \
			$(call compile_file,speed optimised,$(CC_SPEED_OPTIMISATION)) \
		, \
			$(if $(findstring $(subst ./src/main/,,$<),$(SIZE_OPTIMISED_SRC)), \
				$(call compile_file,size optimised,$(CC_SIZE_OPTIMISATION)) \
			, \
				$(call compile_file,optimised,$(CC_DEFAULT_OPTIMISATION)) \
			) \
		) \
	)
endif
# --------------------------------------------------//
# Assemble
$(OBJECT_DIR)/$(TARGET)/%.o: %.s
	$(V1) mkdir -p $(dir $@)
	@echo "%% $(notdir $<)" "$(STDOUT)"
	$(V1) $(CROSS_CC) -c -o $@ $(ASFLAGS) $<
$(OBJECT_DIR)/$(TARGET)/%.o: %.S
	$(V1) mkdir -p $(dir $@)
	@echo "%% $(notdir $<)" "$(STDOUT)"
	$(V1) $(CROSS_CC) -c -o $@ $(ASFLAGS) $<
TARGETS_CLEAN = $(addsuffix _clean,$(VALID_TARGETS))
# Make the binary ------------------------------- //
binary:
	$(V0) $(MAKE) -j8 $(TARGET_BIN)
## clean             : clean up temporary / machine-generated files
clean:
	@echo "Cleaning $(TARGET_BASENAME)"
	#@echo "$(SRC)"
	@echo "$(TARGET_DIR_SRC)"
	$(V0) rm -f $(OBJECT_DIR)/BETAFLIGHT_4.2.0_MAIXBIT_RISCVK210.bin
	$(V0) rm -f $(OBJECT_DIR)/BETAFLIGHT_MAIXBIT.elf
	$(V0) rm -rf $(OBJECT_DIR)/$(TARGET)
	@echo "Cleaning $(TARGET) succeeded."
## test_clean        : clean up temporary / machine-generated files (tests)
test-%_clean:
	$(MAKE) test_clean
test_clean:
	$(V0) $(MAKE) clean || true
$(TARGETS_CLEAN):
	$(V0) $(MAKE) -j8 TARGET=$(subst _clean,,$@) clean
## clean_all         : clean all valid targets
clean_all: $(TARGETS_CLEAN) test_clean
version:
	@echo $(FC_VER)
target-mcu:
	@echo $(TARGET_MCU)
# rebuild everything when makefile changes
$(TARGET_OBJS): Makefile $(TARGET_DIR)/target.mk $(wildcard make/*)
# include auto-generated dependencies
-include $(TARGET_DEPS)
# -----------------------------------------//
