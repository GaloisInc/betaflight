# flash size is 1024kB = 1MB
TARGET_FLASH_SIZE 	:= 1024
LINKER_DIR      	:= $(ROOT)/src/link
RISCV_SRC         	:= $(ROOT)/lib/main/RISCV_K210
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
STARTUP_SRC         = crt.S
#---------------------------------------------------------------#
#Source------------------------------------------------------#
STDPERIPH_DIR   = $(ROOT)/lib/main/RISCV_K210
STDPERIPH_SRC   = $(notdir $(wildcard $(STDPERIPH_DIR)/bsp/*.c))\
                    $(notdir $(wildcard $(STDPERIPH_DIR)/drivers/*.c))
EXCLUDES        = 
STDPERIPH_SRC   := $(filter-out ${EXCLUDES}, $(STDPERIPH_SRC))
DEVICE_STDPERIPH_SRC = $(STDPERIPH_SRC)
TARGET_SRC			= \
					drivers/flash_riscv_k210.c \
					capstone_print.c \
					pg/flash_riscv_k210.c
#-----------------------------------------------------------------#
#Includes
DRIVER_INCLUDES 	= $(RISCV_SRC)/drivers/include
BSP_INCLUDES 		= $(RISCV_SRC)/bsp/include
UTILS_INCLUDES 		= $(RISCV_SRC)/utils/include
THIRD_PARTY 		= $(RISCV_SRC)/xtl/include
NNCASE				= $(RISCV_SRC)/nncase/include

VPATH              += \
					$(ROOT)/lib/main/RISCV_K210/bsp \
                    $(ROOT)/lib/main/RISCV_K210/drivers

#-----------------------------------------------------------------#
INCLUDE_DIRS    	:= \
					$(INCLUDE_DIRS) \
					$(RISCV_SRC) \
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
					-T $(LD_SCRIPT) \
					-Wl,-static \
					-Wl,--start-group \
					-Wl,--whole-archive \
					-Wl,--no-whole-archive \
					-Wl,--end-group \
					-Wl,-EL \
					-Wl,--no-relax \
					-g \
					-O3 -O2 \
					-lm \
					-lstdc++ \
					-lc -lgloss -lgcc\
					-specs=nano.specs -specs=nosys.specs\
               		-lnosys \
					-Wl,-gc-sections,-Map,$(TARGET_MAP) \
					-Wl,--cref \
					-Wl,--print-memory-usage \
					-lnosys \
					$(LTO_FLAGS) 
#--------------------------------------------------------------------#
# GCC FLAGS 
CFLAGS			 = \
					$(ARCH_FLAGS) \
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
					$(addprefix -I,$(INCLUDE_DIRS)) \
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

