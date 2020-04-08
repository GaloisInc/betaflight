# flash size is 128kb = 16MB
TARGET_FLASH_SIZE 	:= 16384
LINKER_DIR      	:= $(ROOT)/src/link
SRC_DIR         	:= $(ROOT)/lib/main/RISCV_K210
# Object file--------------------------------------------------#
OPBL = yes
#--------------------------------------------------------------#
# EXST = yes
# this flag is used for eeprom mem config
#MAIXBIT linker
LD_SCRIPT       	= $(LINKER_DIR)/riscv_flash_k210_128k.ld
#---------------------------------------------------------------#
TARGET_MAP  		= $(OBJECT_DIR)/$(FORKNAME)_$(TARGET).map
DEVICE_FLAGS  		+= -DRISCV_K210
TARGET_FLAGS  		:= -D$(TARGET)
STARTUP_SRC    		= $(ROOT)/src/main/startup/crt.S
#---------------------------------------------------------------#
#Source------------------------------------------------------#
DRIVERS_SRC 		= $(notdir $(wildcard $(SRC_DIR)/drivers/*.c))
BSP_SRC 			= $(notdir $(wildcard $(SRC_DIR)/bsp/*.c))
STDPERIPH_DIR 		:= 	$(DRIVERS_SRC) \
						$(BSP_SRC)
#-----------------------------------------------------------------#
#Includes
DRIVER_INCLUDES 	= $(SRC_DIR)/drivers/include
BSP_INCLUDES 		= $(SRC_DIR)/bsp/include
UTILS_INCLUDES 		= $(SRC_DIR)/utils/include
THIRD_PARTY 		= $(SRC_DIR)/xtl/include
NNCASE				= $(SRC_DIR)/nncase/include
#-----------------------------------------------------------------#
INCLUDE_DIRS    	:= \
					$(INCLUDE_DIRS) \
					$(SRC_DIR) \
					$(DRIVER_INCLUDES) \
					$(BSP_INCLUDES) \
					$(UTILS_INCLUDES) \
					$(THIRD_PARTY)\
					$(NNCASE)\
					$(ROOT)/src/main/target/$(TARGET) \


#-------------------------------------------------------------------#
# ARCH_FLAGS 														#
ARCH_FLAGS      	= -march=rv64imafc -mabi=lp64f -mcmodel=medany	#
#-------------------------------------------------------------------#
# Assembly FLAGS-----------------------------------------------------
ASFLAGS   			= $(ARCH_FLAGS) \
					-x assembler-with-cpp -D __riscv64 \
					$(addprefix -I,$(INCLUDE_DIRS)) 
#-------------------------------------------------------------------#
# Linker FLAGS
LD_FLAGS       		= \
					-nostartfiles \
					-mno-relax \
					-T $(LD_SCRIPT) \
					-static \
					-Wl,--gc-sections \
					-Wl,-static \
					-Wl,--start-group \
					-Wl,--whole-archive \
					-Wl,--no-whole-archive \
					-Wl,--end-group \
					-Wl,-EL \
					-Wl,--no-relax \
					-g \
					-Os \
					-O3 \
					-O2 \
					-lm \
					-lc \
					-Wl,-gc-sections,-Map,$(TARGET_MAP) \
					-Wl,--cref \
					-Wl,--print-memory-usage \
					--specs=nano.specs \
					-lnosys \
					$(LTO_FLAGS) 
#--------------------------------------------------------------------#
# GCC FLAGS 
CFLAGS			 = \
					-mcmodel=medany \
					-mabi=lp64f \
					-march=rv64imafc \
					-std=gnu11 \
					-Wno-pointer-to-int-cast \
					-Wno-old-style-declaration \
					-fno-common \
					-ffunction-sections \
					-fdata-sections \
					-fstrict-volatile-bitfields \
					-fno-zero-initialized-in-bss \
					-ffast-math \
					-fno-math-errno \
					-fsingle-precision-constant \
					-Os\
					-ggdb \
					-Wall \
					-Werror=all \
					-Wno-error=unused-function \
					-Wno-error=unused-but-set-variable \
					-Wno-error=unused-variable \
					-Wno-error=deprecated-declarations \
					-Wno-multichar \
					-Wextra \
					-Werror=frame-larger-than=32768 \
					-Wno-unused-parameter \
					-Wno-sign-compare \
					-Wno-error=missing-braces \
					-Wno-error=return-type \
					-Wno-error=pointer-sign \
					-Wno-missing-braces \
					-Wno-strict-aliasing \
					-Wno-implicit-fallthrough \
					-Wno-missing-field-initializers \
					-Wno-int-to-pointer-cast \
					-Wno-error=comment \
					-Wno-error=logical-not-parentheses \
					-Wno-error=duplicate-decl-specifier \
					-Wno-error=parentheses\
					-std=gnu11 \
					-Wno-pointer-to-int-c \
					-Wno-old-style-declaration $(addprefix -I,$(INCLUDE_DIRS)) \
					$(TARGET_FLAGS) \
					$(DEVICE_FLAGS) \
					-D$(TARGET) \
					-D'__FORKNAME__="$(FORKNAME)"' \
					-D'__TARGET__="$(TARGET)"' \
					-D'__REVISION__="$(REVISION)"' \
					-DUSE_DEVICE_STDPERIPH_SRC \
					-g
#-------------------------------------------------------------
#Debug
ifneq ($(DEBUG),GDB)
OPTIMISE_DEFAULT    := -Os
OPTIMISE_SPEED      :=
OPTIMISE_SIZE       :=
LTO_FLAGS           := $(OPTIMISATION_BASE) $(OPTIMISE_DEFAULT)
endif
#---------------------------------------------------------------