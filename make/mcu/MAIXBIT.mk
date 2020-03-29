#
# RISCV_K210
#
MCU_FLASH_SIZE := 128
#MAIXBIT drivers
STDPERIPH_DIR   = $(ROOT)/lib/main/RISCV_K210/Drivers
STDPERIPH_SRC   = $(notdir $(wildcard $(STDPERIPH_DIR)/Src/*.c))    
DEFAULT_LD_SCRIPT = $(LINKER_DIR)/riscv_flash_k210_128k.ld
EXCLUDES        =  \
                aes.c\
                apu.c\
                clint.c\
                dmac.c\
                drivers.txt\
                dvp.c\
                fft.c\
                fpioa.c\
                gpio.c\
                gpiohs.c\
                i2c.c\
                i2s.c\
                kpu.c\
                plic.c\
                pwm.c\
                rtc.c\
                sha256.c\
                spi.c\
                sysctl.c\
                timer.c\
                uart.c\
                uarths.c\
                utils.c\
                wdt.c\


INCLUDE_DIRS   := $(INCLUDE_DIRS) \
                  $(STDPERIPH_DIR)/Inc\
                  $(STDPERIPH_DIR)/Inc/Utils

DEVICE_STDPERIPH_SRC = $(STDPERIPH_SRC)

# If VCP is enabled in target.h (VCP may be only for STM boards?)
#ifneq ($(filter VCP, $(FEATURES)),)
#INCLUDE_DIRS    := $(INCLUDE_DIRS) \
#                   $(USBFS_DIR)/inc \
#                  $(ROOT)/src/main/vcp

#VPATH           := $(VPATH):$(USBFS_DIR)/src

#DEVICE_STDPERIPH_SRC := $(DEVICE_STDPERIPH_SRC) \
#                        $(USBPERIPH_SRC)
#endif

ifeq ($(LD_SCRIPT),)
LD_SCRIPT       = $(LINKER_DIR)/riscv_flash_k210_128k.ld
endif

#Flags
ARCH_FLAGS      = rv64imafc
#DEVICE_FLAGS    = #-DSTM32F10X_MD
LD_FLAGS       :=  \
                    -nostartfiles\
                    -static\
                    -Wl,\
                    --gc-sections\
                    -Wl,\
                    -static\
                    -Wl,\
                    --start-group\
                    -Wl,\
                    --whole-archive\
                    -Wl,\
                    --no-whole-archive\
                    -Wl,\
                    --end-group\
                    -Wl,\
                    -EL\
                    -Wl,\
                    --no-relax\
                    -T$(LD_SCRIPT)

# STDPERIPH_SRC   := $(filter-out ${EXCLUDES}, $(STDPERIPH_SRC))
                     

#(VCP may be only for STM boards?)
#VCP_SRC = \
#            vcp/hw_config.c \
#            vcp/stm32_it.c \
#            vcp/usb_desc.c \
#            vcp/usb_endp.c \
#            vcp/usb_istr.c \
#            vcp/usb_prop.c \
#            vcp/usb_pwr.c \
#            drivers/serial_usb_vcp.c \
#            drivers/usb_io.c

#BF drivers
MCU_COMMON_SRC = \
                drivers/accgyro/accgyro_mpu.c \

# DSP_LIB :=

ifneq ($(DEBUG),GDB)
OPTIMISE_DEFAULT    := -Os
OPTIMISE_SPEED      :=
OPTIMISE_SIZE       :=

LTO_FLAGS           := $(OPTIMISATION_BASE) $(OPTIMISE_DEFAULT)
endif
