#
# RISCV_K210
#
# flash size is 128kb = 16MB
MCU_FLASH_SIZE := 16384
OPBL = yes

#EXST = yes
# this flag is used for eeprom mem config
DEVICE_FLAGS  += -DRISCV_K210
TARGET_FLAGS  := -D$(TARGET)
STARTUP_SRC    = $(STDPERIPH_DIR)/bsp/crt.S

#$(warning "inside this platfrom.h RUBEN)

#MAIXBIT drivers
STDPERIPH_DIR   = $(ROOT)/lib/main/RISCV_K210
STDPERIPH_SRC   = $(notdir $(wildcard $(STDPERIPH_DIR)/drivers/*.c))    
LD_SCRIPT       = $(LINKER_DIR)/riscv_flash_k210_128k.ld
DEFAULT_LD_SCRIPT   = $(LINKER_DIR)/riscv_flash_k210_128k.ld

TARGET_MAP  = $(OBJECT_DIR)/$(FORKNAME)_$(TARGET).map

EXCLUDES        =  \
                aes.c \
                apu.c \
                clint.c \
                dmac.c \
                dvp.c \
                fft.c \
                gpiohs.c \
                i2c.c \
                i2s.c \
                kpu.c \
                plic.c \
                pwm.c \
                rtc.c \
                sha256.c \
                spi.c \
                sysctl.c \
                timer.c \
                uart.c \
                uarths.c \
                utils.c \
                wdt.c


INCLUDE_DIRS   := $(INCLUDE_DIRS) \
                  $(STDPERIPH_DIR) \
                  $(STDPERIPH_DIR)/drivers/include \
                  $(STDPERIPH_DIR)/utils/include \
                  $(STDPERIPH_DIR)/bsp/include \
                  $(ROOT)/src/main/target/$(TARGET) \

DEVICE_STDPERIPH_SRC = $(STDPERIPH_SRC)

#Flags
#ARCH_FLAGS unique to K210

ARCH_FLAGS      = -march=rv64imafc -mabi=lp64f -mcmodel=medany

#A mix of K210 flags and BF flags
LD_FLAGS       :=  \
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
               


#-Wl, -gc-sections,-Map,$(TARGET_MAP)\
#CLAGS unique to K210. These get appended to CFLAGS in Makefile.
CFLAGS     :=  $(ARCH_FLAGS) \
               -fno-common \
               -ffunction-sections \
               -fdata-sections \
               -fstrict-volatile-bitfields \
               -fno-zero-initialized-in-bss \
               -ffast-math \
               -fno-math-errno \
               -fsingle-precision-constant \
               -Os \
               -ggdb \
               -std=gnu11 \
               -Wno-pointer-to-int-cast \
               -Wall \
               -Werror=all \
               -Wno-error=unused-function \
               -Wno-error=unused-but-set-variable \
               -Wno-error=unused-variable \
               -Wno-error=deprecated-declarations \
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
               -Wno-error=parentheses \
               -Wno-old-style-declaration \
               $(addprefix -I,$(INCLUDE_DIRS)) \
               $(TARGET_FLAGS) \
               $(DEVICE_FLAGS) \
               -D$(TARGET) \
               -D'__FORKNAME__="$(FORKNAME)"' \
               -D'__TARGET__="$(TARGET)"' \
               -D'__REVISION__="$(REVISION)"' \
               -DUSE_STDPERIPH_DRIVER \
               -g
     
#MCU_COMMON_SRC = \
            startup/system_stm32h7xx.c \
            drivers/system_stm32h7xx.c \
            drivers/timer_hal.c \
            drivers/timer_stm32h7xx.c \
            drivers/serial_uart_hal.c \
            drivers/serial_uart_stm32h7xx.c \
            drivers/bus_quadspi_hal.c \
            drivers/bus_spi_hal.c \
            drivers/dma_stm32h7xx.c \
            drivers/light_ws2811strip_hal.c \
            drivers/adc_stm32h7xx.c \
            drivers/bus_i2c_hal.c \
            drivers/pwm_output_dshot_hal.c \
            drivers/pwm_output_dshot_shared.c \
            drivers/persistent.c \
            drivers/transponder_ir_io_hal.c \
            drivers/audio_stm32h7xx.c \
            drivers/memprot_hal.c \
            drivers/memprot_stm32h7xx.c 
            #drivers/accgyro/accgyro_mpu.c \

MCU_COMMON_SRC = \
                $(STDPERIPH_DIR)/drivers/fpioa.c \
                $(STDPERIPH_DIR)/drivers/gpio.c 

#MCU_EXCLUDES = \
            drivers/bus_i2c.c \
            drivers/timer.c  
                 
# Common drivers
#MCU_COMMON_SRC = \
#                drivers/accgyro/accgyro_mpu.c

ifneq ($(DEBUG),GDB)
OPTIMISE_DEFAULT    := -Os
OPTIMISE_SPEED      :=
OPTIMISE_SIZE       :=
LTO_FLAGS           := $(OPTIMISATION_BASE) $(OPTIMISE_DEFAULT)
endif
