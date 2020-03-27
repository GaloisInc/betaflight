#
# K210 Make file include
#

MCU_FLASH_SIZE := 128
#MAIXBIT drivers
STDPERIPH_DIR   = $(ROOT)/lib/main/MAIXBIT/Drivers/MAIXBIT_Driver
STDPERIPH_SRC   = $(notdir $(wildcard $(STDPERIPH_DIR)/src/*.c))
EXCLUDES        = 
STARTUP_SRC     = crt.S
STDPERIPH_SRC   := $(filter-out ${EXCLUDES}, $(STDPERIPH_SRC))

INCLUDE_DIRS    := $(INCLUDE_DIRS) \
                   $(STDPERIPH_DIR)/inc

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
LD_SCRIPT       = $(LINKER_DIR)/k210.ld
endif

#Flags
ARCH_FLAGS      = #-mthumb -mcpu=cortex-m3

DEVICE_FLAGS    = #-DSTM32F10X_MD

LD_FLAGS    := \
              -lm \
              -lpthread \
              -lc \
              -lrt \
              $(ARCH_FLAGS) \
              $(LTO_FLAGS) \
              $(DEBUG_FLAGS) \
              -Wl,-gc-sections,-Map,$(TARGET_MAP) \
              -Wl,-L$(LINKER_DIR) \
              -Wl,--cref \
              -T$(LD_SCRIPT)

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
            #drivers/adc_stm32f10x.c \
            #drivers/bus_i2c_stm32f10x.c \
            #drivers/bus_spi_stdperiph.c \
            #drivers/dma.c \
            #drivers/inverter.c \
            #drivers/light_ws2811strip_stdperiph.c \
            #drivers/serial_uart_stdperiph.c \
            #drivers/serial_uart_stm32f10x.c \
            #drivers/system_stm32f10x.c \
            #drivers/timer_stm32f10x.c

DSP_LIB :=

ifneq ($(DEBUG),GDB)
OPTIMISE_DEFAULT    := -Os
OPTIMISE_SPEED      :=
OPTIMISE_SIZE       :=

LTO_FLAGS           := $(OPTIMISATION_BASE) $(OPTIMISE_DEFAULT)
endif
