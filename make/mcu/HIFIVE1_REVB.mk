#
# RISCV Make file include
#

TARGET_FLASH_SIZE   := 1024

#############################################################
# Makefile Arguments
#############################################################

# BSP_DIR sets the path to the target-specific board support package.
BSP_DIR ?= $(abspath bsp)
#$(info BSP_DIR = $(BSP_DIR))

# SRC_DIR sets the path to the program source directory
SIFIVE_SRC_DIR ?= $(abspath sifive_src)
#$(info SIFIVE_SRC_DIR = $(SIFIVE_SRC_DIR))

#############################################################
# BSP loading
#############################################################

# There must be a settings makefile fragment in the BSP's board directory.
ifeq ($(wildcard $(BSP_DIR)/settings.mk),)
$(error Unable to find BSP for $(TARGET), expected to find $(BSP_DIR)/settings.mk)
endif

# Include the BSP settings
include $(BSP_DIR)/settings.mk

# Check that settings.mk sets RISCV_ARCH and RISCV_ABI
ifeq ($(RISCV_ARCH),)
$(error $(BSP_DIR)/board.mk must set RISCV_ARCH, the RISC-V ISA string to target)
endif

ifeq ($(RISCV_ABI),)
$(error $(BSP_DIR)/board.mk must set RISCV_ABI, the ABI to target)
endif

ifeq ($(RISCV_CMODEL),)
RISCV_CMODEL = medany
endif

ifeq ($(PROGRAM),dhrystone)
ifeq ($(LINK_TARGET),)
  ifneq ($(TARGET),freedom-e310-arty)
  ifneq ($(TARGET),sifive-hifive1)
  ifneq ($(TARGET),sifive-hifive1-revb)
    LINK_TARGET = ramrodata
  endif
  endif
  endif
endif
endif

ifeq ($(PROGRAM),coremark)
ifeq ($(LINK_TARGET),)
LINK_TARGET = ramrodata
endif
endif

ifeq ($(LINK_TARGET),)
LINK_TARGET = default
endif

# Determines the XLEN from the toolchain tuple
ifeq ($(patsubst rv32%,rv32,$(RISCV_ARCH)),rv32)
RISCV_XLEN := 32
else ifeq ($(patsubst rv64%,rv64,$(RISCV_ARCH)),rv64)
RISCV_XLEN := 64
else
$(error Unable to determine XLEN from $(RISCV_ARCH))
endif

QEMU_RISCV32 = qemu-system-riscv32
QEMU_RISCV64 = qemu-system-riscv64

#############################################################
# Software Flags
#############################################################

# Set the arch, ABI, and code model
RISCV_CCASFLAGS += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_CFLAGS    += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_CXXFLAGS  += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_ASFLAGS   += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)

# Prune unused functions and data
RISCV_CFLAGS    += -ffunction-sections -fdata-sections
RISCV_CXXFLAGS  += -ffunction-sections -fdata-sections

# Include the Metal headers
RISCV_CCASFLAGS += -I$(abspath $(BSP_DIR)/install/include/)
RISCV_CFLAGS    += -I$(abspath $(BSP_DIR)/install/include/)
RISCV_CXXFLAGS  += -I$(abspath $(BSP_DIR)/install/include/)

# Use newlib-nano
RISCV_CCASFLAGS += --specs=nano.specs
RISCV_CFLAGS    += --specs=nano.specs
RISCV_CXXFLAGS  += --specs=nano.specs

# Turn on garbage collection for unused sections
RISCV_LDFLAGS += -Wl,--gc-sections
# Turn on linker map file generation
RISCV_LDFLAGS += -Wl,-Map,$(PROGRAM).map
# Turn off the C standard library
RISCV_LDFLAGS += -nostartfiles -nostdlib
# Find the archive files and linker scripts
RISCV_LDFLAGS += -L$(sort $(dir $(abspath $(filter %.a,$^)))) -T$(abspath $(filter %.lds,$^))

# Link to the relevant libraries
RISCV_LDLIBS += -Wl,--start-group -lc -lgcc -lm -lmetal -lmetal-gloss -Wl,--end-group

# Load the configuration Makefile
CONFIGURATION_FILE = $(wildcard $(CONFIGURATION).mk)
ifeq ($(words $(CONFIGURATION_FILE)),0)
$(error Unable to find the Makefile $(CONFIGURATION).mk for CONFIGURATION=$(CONFIGURATION))
endif
include $(CONFIGURATION).mk


# Set the optimization level for release
RISCV_ASFLAGS += -Os
RISCV_CFLAGS += -Os
RISCV_CXXFLAGS += -Os


#############################################################
# Original Cleanflight code below
#############################################################

ifneq ($(TARGET_MCU), HIFIVE1_REVB)
STDPERIPH_DIR   = $(ROOT)/lib/main/STM32F7/Drivers/STM32F7xx_HAL_Driver
STDPERIPH_SRC   = $(notdir $(wildcard $(STDPERIPH_DIR)/Src/*.c))
EXCLUDES        = stm32f7xx_hal_can.c \
                  stm32f7xx_hal_cec.c \
                  stm32f7xx_hal_crc.c \
                  stm32f7xx_hal_crc_ex.c \
                  stm32f7xx_hal_cryp.c \
                  stm32f7xx_hal_cryp_ex.c \
                  stm32f7xx_hal_dcmi.c \
                  stm32f7xx_hal_dcmi_ex.c \
                  stm32f7xx_hal_dfsdm.c \
                  stm32f7xx_hal_dma2d.c \
                  stm32f7xx_hal_dsi.c \
                  stm32f7xx_hal_eth.c \
                  stm32f7xx_hal_hash.c \
                  stm32f7xx_hal_hash_ex.c \
                  stm32f7xx_hal_hcd.c \
                  stm32f7xx_hal_i2s.c \
                  stm32f7xx_hal_irda.c \
                  stm32f7xx_hal_iwdg.c \
                  stm32f7xx_hal_jpeg.c \
                  stm32f7xx_hal_lptim.c \
                  stm32f7xx_hal_ltdc.c \
                  stm32f7xx_hal_ltdc_ex.c \
                  stm32f7xx_hal_mdios.c \
                  stm32f7xx_hal_mmc.c \
                  stm32f7xx_hal_msp_template.c \
                  stm32f7xx_hal_nand.c \
                  stm32f7xx_hal_nor.c \
                  stm32f7xx_hal_qspi.c \
                  stm32f7xx_hal_rng.c \
                  stm32f7xx_hal_rtc.c \
                  stm32f7xx_hal_rtc_ex.c \
                  stm32f7xx_hal_sai.c \
                  stm32f7xx_hal_sai_ex.c \
                  stm32f7xx_hal_sd.c \
                  stm32f7xx_hal_sdram.c \
                  stm32f7xx_hal_smartcard.c \
                  stm32f7xx_hal_smartcard_ex.c \
                  stm32f7xx_hal_smbus.c \
                  stm32f7xx_hal_spdifrx.c \
                  stm32f7xx_hal_sram.c \
                  stm32f7xx_hal_timebase_rtc_alarm_template.c \
                  stm32f7xx_hal_timebase_rtc_wakeup_template.c \
                  stm32f7xx_hal_timebase_tim_template.c \
                  stm32f7xx_hal_wwdg.c \
                  stm32f7xx_ll_adc.c \
                  stm32f7xx_ll_crc.c \
                  stm32f7xx_ll_dac.c \
                  stm32f7xx_ll_exti.c \
                  stm32f7xx_ll_fmc.c \
                  stm32f7xx_ll_i2c.c \
                  stm32f7xx_ll_lptim.c \
                  stm32f7xx_ll_pwr.c \
                  stm32f7xx_ll_rng.c \
                  stm32f7xx_ll_rtc.c \
                  stm32f7xx_ll_sdmmc.c \
                  stm32f7xx_ll_usart.c

STDPERIPH_SRC   := $(filter-out ${EXCLUDES}, $(STDPERIPH_SRC))

#USB
USBCORE_DIR = $(ROOT)/lib/main/STM32F7/Middlewares/ST/STM32_USB_Device_Library/Core
USBCORE_SRC = $(notdir $(wildcard $(USBCORE_DIR)/Src/*.c))
EXCLUDES    = usbd_conf_template.c
USBCORE_SRC := $(filter-out ${EXCLUDES}, $(USBCORE_SRC))

USBCDC_DIR = $(ROOT)/lib/main/STM32F7/Middlewares/ST/STM32_USB_Device_Library/Class/CDC
USBCDC_SRC = $(notdir $(wildcard $(USBCDC_DIR)/Src/*.c))
EXCLUDES   = usbd_cdc_if_template.c
USBCDC_SRC := $(filter-out ${EXCLUDES}, $(USBCDC_SRC))

USBHID_DIR = $(ROOT)/lib/main/STM32F7/Middlewares/ST/STM32_USB_Device_Library/Class/HID
USBHID_SRC = $(notdir $(wildcard $(USBHID_DIR)/Src/*.c))

USBHIDCDC_DIR = $(ROOT)/lib/main/STM32F7/Middlewares/ST/STM32_USB_Device_Library/Class/CDC_HID
USBHIDCDC_SRC = $(notdir $(wildcard $(USBHIDCDC_DIR)/Src/*.c))

USBMSC_DIR = $(ROOT)/lib/main/STM32F7/Middlewares/ST/STM32_USB_Device_Library/Class/MSC
USBMSC_SRC = $(notdir $(wildcard $(USBMSC_DIR)/Src/*.c))
EXCLUDES   = usbd_msc_storage_template.c
USBMSC_SRC := $(filter-out ${EXCLUDES}, $(USBMSC_SRC))


VPATH := $(VPATH):$(USBCDC_DIR)/Src:$(USBCORE_DIR)/Src:$(USBHID_DIR)/Src:$(USBHIDCDC_DIR)/Src:$(USBMSC_DIR)/Src

DEVICE_STDPERIPH_SRC := $(STDPERIPH_SRC) \
                        $(USBCORE_SRC) \
                        $(USBCDC_SRC) \
                        $(USBHID_SRC) \
                        $(USBHIDCDC_SRC) \
                        $(USBMSC_SRC)

#CMSIS
VPATH           := $(VPATH):$(CMSIS_DIR)/Include:$(CMSIS_DIR)/Device/ST/STM32F7xx
VPATH           := $(VPATH):$(STDPERIPH_DIR)/Src
CMSIS_SRC       :=
INCLUDE_DIRS    := $(INCLUDE_DIRS) \
                   $(STDPERIPH_DIR)/Inc \
                   $(USBCORE_DIR)/Inc \
                   $(USBCDC_DIR)/Inc \
                   $(USBHID_DIR)/Inc \
                   $(USBHIDCDC_DIR)/Inc \
                   $(USBMSC_DIR)/Inc \
                   $(CMSIS_DIR)/Core/Include \
                   $(ROOT)/lib/main/STM32F7/Drivers/CMSIS/Device/ST/STM32F7xx/Include \
                   $(ROOT)/src/main/vcp_hal

ifneq ($(filter SDCARD,$(FEATURES)),)
INCLUDE_DIRS    := $(INCLUDE_DIRS) \
                   $(FATFS_DIR)
VPATH           := $(VPATH):$(FATFS_DIR)
endif

#Flags
ARCH_FLAGS      = -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -fsingle-precision-constant -Wdouble-promotion

DEVICE_FLAGS    = -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER

#DEVICE_FLAGS   += -DSTM32F745xx
#LD_SCRIPT       = $(LINKER_DIR)/stm32_flash_f74x.ld
STARTUP_SRC     = startup_stm32f745xx.s
TARGET_FLASH   := 1024

DEVICE_FLAGS    += -DHSE_VALUE=$(HSE_VALUE)

TARGET_FLAGS    = -D$(TARGET)

VCP_SRC = \
            vcp_hal/usbd_desc.c \
            vcp_hal/usbd_conf.c \
            vcp_hal/usbd_cdc_interface.c \
            drivers/serial_usb_vcp.c \
            drivers/usb_io.c

MCU_COMMON_SRC = \
            target/system_stm32f7xx.c \
            drivers/accgyro/accgyro_mpu.c \
            drivers/adc_stm32f7xx.c \
            drivers/audio_stm32f7xx.c \
            drivers/bus_i2c_hal.c \
            drivers/dma_stm32f7xx.c \
            drivers/light_ws2811strip_hal.c \
            drivers/transponder_ir_io_hal.c \
            drivers/bus_spi_ll.c \
            drivers/pwm_output_dshot_hal.c \
            drivers/timer_hal.c \
            drivers/timer_stm32f7xx.c \
            drivers/system_stm32f7xx.c \
            drivers/serial_uart_stm32f7xx.c \
            drivers/serial_uart_hal.c

MCU_EXCLUDES = \
            drivers/bus_i2c.c \
            drivers/timer.c \
            drivers/serial_uart.c

MSC_SRC = \
            drivers/usb_msc_f7xx.c \
            msc/usbd_storage.c


DSP_LIB := $(ROOT)/lib/main/CMSIS/DSP
DEVICE_FLAGS += -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1 -DUNALIGNED_SUPPORT_DISABLE -DARM_MATH_CM7

$(warning HELLLLER RUBEN)
endif
