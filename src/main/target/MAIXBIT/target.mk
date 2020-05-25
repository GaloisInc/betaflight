RISCV_K210_TARGETS  += $(TARGET)
FEATURES         += ONBOARDFLASH

TARGET_SRC      += \
              	#crt.S \
                #main.c\
                #riscv_k210_locks.c \
                #riscv_k210_entry.c \
                #riscv_k210_interrupt.c \
                #riscv_k210_printf.c \
                #riscv_k210_sleep.c \
                #riscv_k210_syscalls.c \
                #riscv_k210_entry_user.c \
                #riscv_k210_dvp.c \
                #riscv_k210_i2c.c \
                #riscv_k210_pwm.c \
                #riscv_k210_timer.c \
                #riscv_k210_aes.c \
                #riscv_k210_fft.c \
                #riscv_k210_i2s.c \
                #riscv_k210_rtc.c \
                #riscv_k210_uart.c \
                #riscv_k210_aes.c \
                #riscv_k210_fft.c \
                #riscv_k210_i2s.c \
                #riscv_k210_rtc.c \
                #riscv_k210_uart.c \
                #riscv_k210_apu.c \
                #riscv_k210_fpioa.c \
                #riscv_k210_iomem.c \
                #riscv_k210_sha256.c \
                #riscv_k210_uarths.c \
                #riscv_k210_clint.c \
                #riscv_k210_gpio.c \
                #riscv_k210_kpu.c \
                #riscv_k210_spi.c \
                #riscv_k210_utils.c \
                #riscv_k210_dmac.c \
                #riscv_k210_gpiohs.c \
                #riscv_k210_plic.c \
                #riscv_k210_sysctl.c \
                #riscv_k210_wdt.c \
                #main.c \
