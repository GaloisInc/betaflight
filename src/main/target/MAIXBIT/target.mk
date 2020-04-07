MAIXBIT_TARGETS  += $(TARGET)
FEATURES         += ONBOARDFLASH

TARGET_SRC += \
                $(STDPERIPH_DIR)/drivers/fpioa.c \
                $(STDPERIPH_DIR)/bsp/entry_user.c \
                $(STDPERIPH_DIR)/bsp/entry.c \
                $(STDPERIPH_DIR)/bsp/interrupt.c \
                $(STDPERIPH_DIR)/bsp/locks.c \
                $(STDPERIPH_DIR)/drivers/gpio.c 