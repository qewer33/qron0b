# --- CONFIGURATION ---

MCU = attiny24a
F_CPU = 1000000UL
PROGRAMMER = arduino
PORT ?= USB0
BAUD = 19200

BUILD_DIR = build
SRC_DIR = src

TARGET = $(BUILD_DIR)/main

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# --- FLAGS ---

CFLAGS = -mmcu=$(MCU) -Os -DF_CPU=$(F_CPU)
CFLAGS += -include src/config.h
CFLAGS += -mcall-prologues -flto
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -fvisibility=hidden
CFLAGS += -Wall -Wextra

LDFLAGS = -mmcu=$(MCU) -flto -Wl,--gc-sections

# --- BUILD RULES ---

all: $(BUILD_DIR) $(TARGET).hex size

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@avr-gcc $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	@echo "Linking..."
	@avr-gcc $(LDFLAGS) $(OBJS) -o $@

$(TARGET).hex: $(TARGET).elf
	@echo "Creating HEX..."
	@avr-objcopy -O ihex -R .eeprom $< $@

# --- UTILITIES ---

size: $(TARGET).elf
	@avr-size -C --mcu=$(MCU) $(TARGET).elf

flash: $(TARGET).hex
	@echo "Flashing firmware to /dev/tty$(PORT)..."
	avrdude -c $(PROGRAMMER) -p t24 -P /dev/tty$(PORT) -b $(BAUD) -B 250 -e -U flash:w:$(TARGET).hex:i -F

flash_fuses: $(TARGET).hex
	@echo "Flashing fuses & firmware /dev/tty$(PORT)..."
	avrdude -c $(PROGRAMMER) -p t24 -P /dev/tty$(PORT) -b $(BAUD) -B 250 -e -U lfuse:w:0x62:m -U hfuse:w:0xDD:m -U flash:w:$(TARGET).hex:i -F

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash flash_fuses clean size
