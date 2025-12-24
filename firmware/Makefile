# --- CONFIGURATION ---
MCU      = attiny24a
F_CPU    = 1000000UL
PROGRAMMER = arduino
PORT     ?= USB0
BAUD     = 19200

# Directory structure
BUILD_DIR = build
SRC_DIR   = src

# Output file name
TARGET = $(BUILD_DIR)/main

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# --- FLAGS ---
CFLAGS   = -mmcu=$(MCU) -Os -DF_CPU=$(F_CPU)
CFLAGS  += -include src/config.h
CFLAGS  += -mcall-prologues -flto
CFLAGS  += -ffunction-sections -fdata-sections
CFLAGS  += -fvisibility=hidden
CFLAGS  += -Wall -Wextra

# Linker Flags
LDFLAGS  = -mmcu=$(MCU) -flto -Wl,--gc-sections

# --- BUILD RULES ---

# Default target
all: $(BUILD_DIR) $(TARGET).hex size

# 1. Create build dir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 2. Compile each .c file to .o (Incremental Build Magic)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@avr-gcc $(CFLAGS) -c $< -o $@

# 3. Link everything together
$(TARGET).elf: $(OBJS)
	@echo "Linking..."
	@avr-gcc $(LDFLAGS) $(OBJS) -o $@

# 4. Create Hex
$(TARGET).hex: $(TARGET).elf
	@echo "Creating HEX..."
	@avr-objcopy -O ihex -R .eeprom $< $@

# --- UTILITIES ---

# Show Size
size: $(TARGET).elf
	@avr-size -C --mcu=$(MCU) $(TARGET).elf

# Flash Firmware
flash: $(TARGET).hex
	@echo "Flashing firmware to /dev/tty$(PORT)..."
	avrdude -c $(PROGRAMMER) -p t24 -P /dev/tty$(PORT) -b $(BAUD) -B 250 -e -U flash:w:$(TARGET).hex:i -F

# Flash Fuses + Firmware
flash_fuses: $(TARGET).hex
	@echo "Flashing fuses & firmware /dev/tty$(PORT)..."
	avrdude -c $(PROGRAMMER) -p t24 -P /dev/tty$(PORT) -b $(BAUD) -B 250 -e -U lfuse:w:0x62:m -U hfuse:w:0xDD:m -U flash:w:$(TARGET).hex:i -F

# Clean up
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash flash_fuses clean size
