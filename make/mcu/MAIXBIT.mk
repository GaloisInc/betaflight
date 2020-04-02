#
# RISCV_K210
#
MCU_FLASH_SIZE := 128

# this flag is used for eeprom mem config
DEVICE_FLAGS += -DRISCVK210

#MAIXBIT drivers
STDPERIPH_DIR   = $(ROOT)/lib/main/RISCV_K210
STDPERIPH_SRC   = $(notdir $(wildcard $(STDPERIPH_DIR)/drivers/*.c))    
LD_SCRIPT       = $(LINKER_DIR)/riscv_flash_k210_128k.ld
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
                  $(STDPERIPH_DIR)\
                  $(STDPERIPH_DIR)/drivers\
                  $(STDPERIPH_DIR)/utils\
                  $(STDPERIPH_DIR)/bsp

DEVICE_STDPERIPH_SRC = $(STDPERIPH_SRC)

# If VCP is enabled in target.h (VCP may be only for STM boards?)
#ifneq ($(filter VCP, $(FEATURES)),)
#INCLUDE_DIRS    := $(INCLUDE_DIRS) \
#                   $(USBFS_DIR)/inc \
#                  $(ROOT)/src/main/vcp

#VPATH           := $(VPATH):$(USBFS_DIR)/src

# DEVICE_STDPERIPH_SRC := $(DEVICE_STDPERIPH_SRC) \
#                        $(USBPERIPH_SRC)
# endif


#Flags
#ARCH_FLAGS unique to K210
ARCH_FLAGS      = -march=rv64imafc -mabi=lp64f


#A mix of K210 flags and BF flags
LD_FLAGS       :=  \
               -nostartfiles\
               -static\
               -Wl,\
               --data-sections\
               -Wl,\
               -static\
               -Wl,\
               --stdarg-opt\
               -Wl,\
               --whole-file\
               -Wl,\
               -Wl,\
               -E\
               -Wl,\
               -mno-relax\
               -T$(LD_SCRIPT)

#CLAGS unique to K210. These get appended to CFLAGS in Makefile.
CFLAGS     :=  -mcmodel=medany\
               -mabi=lp64f\
               -march=rv64imafc\
               -fno-common\
               -ffunction-sections\
               -fdata-sections\
               -fstrict-volatile-bitfields\
               -fno-zero-initialized-in-bss\
               -ffast-math\
               -fno-math-errno\
               -fsingle-precision-constant\
               -Os\
               -ggdb\
               -std=gnu11\
               -Wno-pointer-to-int-cast\
               -Wall\
               -Werror=all\
               -Wno-error=unused-function\
               -Wno-error=unused-but-set-variable\
               -Wno-error=unused-variable\
               -Wno-error=deprecated-declarations\
               -Wextra\
               -Werror=frame-larger-than=32768\
               -Wno-unused-parameter\
               -Wno-sign-compare\
               -Wno-error=missing-braces\
               -Wno-error=return-type\
               -Wno-error=pointer-sign\
               -Wno-missing-braces\
               -Wno-strict-aliasing\
               -Wno-implicit-fallthrough\
               -Wno-missing-field-initializers\
               -Wno-int-to-pointer-cast\
               -Wno-error=comment\
               -Wno-error=logical-not-parentheses\
               -Wno-error=duplicate-decl-specifier\
               -Wno-error=parentheses\
               -Wno-old-style-declaration

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

# Common drivers
MCU_COMMON_SRC = \
                drivers/accgyro/accgyro_mpu.c

ifneq ($(DEBUG),GDB)
OPTIMISE_DEFAULT    := -Os
OPTIMISE_SPEED      :=
OPTIMISE_SIZE       :=
LTO_FLAGS           := $(OPTIMISATION_BASE) $(OPTIMISE_DEFAULT)
endif