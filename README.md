![banner](https://github.com/qewer33/qron0b/blob/main/assets/qron0b_banner.png?raw=true)

qron0b is a minimalist and low-power digital binary wristwatch. It uses an Attiny24A/44A/84A as it's MCU, uses the DS1302 as it's RTC and displays the time in BCD (binary coded decimal) format with it's integrated 4x4 LED matrix driven by an 74HC595 shift register. It's powered by a single CR2032 coin cell battery. It's designed to use minimal power and can run without a battery change for months.

> [!WARNING]  
> This project is NOT perfect by any means, it's my first ever low-power board design and thus contains many design flaws (although mostly small). It does work but build it at your own risk and definitely don't expect everything to go smoothly

## The Product

### Features

### How To Read

### Setting time

## Repository Layout
- pcb/: The KiCAD project for the qron0b PCB
- cad/: The FreeCAD files (and .stl + .step for 3D printing) for the watch case
- firmware/: The source code for the qron0b firmware, written in bare metal AVR C
- assets/: Screenshots and render files used in the README

## Hardware

### PCB Design

### Mechanical Design

## Firmware
