# qron0b

qron0b is a minimalist and low-power digital binary wristwatch. It uses an Attiny24A/44A/84A as it's MCU, uses the DS1302 as it's RTC and displays the time in BCD (binary coded decimal) format with it's integrated 4x4 LED matrix driven by an 74HC595 shift register. It's powered by a single CR2032 coin cell battery. It's designed to use minimal power and can run without a battery change for months.

> [!WARNING]  
> This project is NOT perfect by any means, it's my first ever low-power board design and thus contains many design flaws (although mostly small). It does work but build it at your own risk and definitely don't expect everything to go smoothly

## Development

### Repository Layout
- firmware/: The source code for the qron0b firmware, written in pure AVR C
- pcb/: The KiCAD project for the qron0b PCB
- cad/: The FreeCAD files (and .stl + .step for 3D printing) for the watch case

### Setup

If using the Zed editor for development, add the following lines to the end of your settings.json:

```json
"lsp": {
  "clangd": {
    "binary": {
      "arguments": ["--query-driver=/usr/bin/avr-gcc"]
    }
  }
}
```

### Programming

The qron0b board can be programmed with any AVR ISP programmer (including any AVR Arduino board via Arduino as ISP). You need to have the `avr-gcc` toolchain installed. If you're on Linux, you can use the provided shell scripts to build and flash the firmware.

Use the provided `build.sh` script to compile & build the firmware. Build artifacts (`.elf` and `.hex` files) will be generated in the `build/` directory.

Use the provided `flasn.sh` script to flash with an Arduino as ISP mounted at USB1 (change the port inside the script if necessary). Use the `--fuses` option in first flash to set the correct fuses. If programming with Arduino as ISP, put a 10uF cap between the Arduino's RES and GND pins.

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
