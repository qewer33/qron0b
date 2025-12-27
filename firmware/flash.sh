#!/usr/bin/env bash

set -e # Stop script early if a command fails

HEX_FILE="build/main.hex"
PORT="USB0"

# Check if build exists
if [ ! -f "$HEX_FILE" ]; then
    echo "Error: $HEX_FILE not found. Please run ./build.sh first."
    exit 1
fi

# Flash the board
echo "Running avrdude..."
if [ "$1" = "--fuses" ]; then
    avrdude -c arduino -p t24 -P /dev/tty$PORT -b 19200 -B 50 -e -U lfuse:w:0x62:m -U flash:w:build/main.hex:i -F
else
    avrdude -c arduino -p t24 -P /dev/tty$PORT -b 19200 -B 50 -e -U flash:w:build/main.hex:i -F
fi
