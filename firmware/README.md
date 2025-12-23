# Development

## Programming

The qron0b board can be programmed with any AVR ISP programmer (including any AVR Arduino board via Arduino as ISP). You need to have the `avr-gcc` toolchain installed. If you're on Linux, you can use the provided shell scripts to build and flash the firmware.

Use the `make` command to build & flash the firmware. Build artifacts (`.elf`, `.hex` and `.o` files) will be generated in the `build/` directory. Use the `make flash_fuses` command in first flash to set the correct fuses. If programming with Arduino as ISP, put a 10uF cap between the Arduino's RES and GND pins.

```sh
# Build the firmware
make

# Flash firmware and fuses to USB0 (do this for first time flashing!)
make flash_fuses
# Flash firmware only to USB0 (do this after first flash)
make flash

# Specify a different port for flashing
make flash PORT=USB1

# See firmware size
make size

# Clean the build directory
make clean
```

Alternatively, you can also use the provided bash scripts:

Use the provided `build.sh` script to compile & build the firmware.

Use the provided `flasn.sh` script to flash with an Arduino as ISP mounted at USB0 (change the port inside the script if necessary). Use the `--fuses` option in first flash to set the correct fuses.

```sh
# Build the firmware
./build.sh

# Flash the firmware with fuses (do this for first time flashing!)
./flash.sh --fuses
# Flash the firmware without fuses (do this after first flash)
./flash.sh

# Build and flash firmware
./build.sh && ./flash.sh
```
