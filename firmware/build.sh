#!/usr/bin/env bash

set -e # Stop script early if a command fails

mkdir -p build # Ensure the build directory exists

FLAGS="-mmcu=attiny24a -include src/config.h -Os -mcall-prologues -flto -fno-exceptions -ffunction-sections -fdata-sections -Wl,--gc-sections -fvisibility=hidden"

# Compile
echo "Running avr-gcc..."
avr-gcc $FLAGS src/main.c src/init.c src/matrix.c src/rtc.c src/key.c -o build/main.elf

# Prepare hex file
echo "Running avr-objcopy..."
avr-objcopy -O ihex build/main.elf build/main.hex

# Show size
echo "Running avr-size..."
avr-size -C --mcu=attiny24a build/main.elf
