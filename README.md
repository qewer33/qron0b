![banner](https://github.com/qewer33/qron0b/blob/main/assets/qron0b_banner.png?raw=true)

**qron0b** is a minimalist and low-power digital **binary wristwatch**. It uses an **Attiny24A/44A/84A as it's MCU**, uses the **DS1302 as it's RTC** and displays the time in **BCD (binary coded decimal)** format with it's integrated **4x4 LED matrix driven by an 74HC595 shift register**. It's powered by a single **CR2032 coin cell battery**. It's designed to use minimal power and **can run without a battery change for months**.

> [!WARNING]  
> This project is NOT perfect by any means, it's my first ever low-power board design and thus contains many design flaws (although mostly small). It does work but build it at your own risk and definitely don't expect everything to go smoothly.

## Repository Layout
- [pcb/](https://github.com/qewer33/qron0b/tree/main/pcb): The KiCAD project for the qron0b PCB.
- [cad/](https://github.com/qewer33/qron0b/tree/main/cad): The FreeCAD files (and .stl + .step for 3D printing) for the watch case.
- [firmware/](https://github.com/qewer33/qron0b/tree/main/firmware): The source code for the qron0b firmware, written in bare metal AVR C.
- [assets/](https://github.com/qewer33/qron0b/tree/main/assets) Screenshots and render files used in the README.

## The Product

![photo](https://github.com/qewer33/qron0b/blob/main/assets/main_photo.jpeg?raw=true)

### Features

qron0b's default firmware is very minimalistic and thus only contains two basic features: **reading time** and **setting time**. The watch is in the sleep state by default with the CPU in power-down mode and the display off. When the onboard button is clicked, the CPU is wakes up from sleep and displays the current time in the LED matrix for 10 seconds. The time can be set while the watch is awake by long pressing the onboard button. Both features are explained in detail under the next two headings.

### Reading Time

The time is displayed in BCD (binary coded decimal) format on qron0b's 4x4 LED matrix in columns (HH:MM), with the LSB (least-significant bir) being the bottom-most LED. Meaning for each column; the bottom-most LED represents 1, the one above represents 2, two above represents 4 and the top-most LED represents 8. All LEDs being off in a column represents 0. The first column is the tens digit of hour, the second column is the ones digit of hour, third column is the tens digit of minute and the fourth column is the ones digit of minute.

Below is a visual explanation and a few examples to help explain it better:

![reading_time](https://github.com/qewer33/qron0b/blob/main/assets/qron0b_time_examples.png?raw=true)

### Setting Time

The time can be set while the watch is awake by long pressing the onboard button. The whole LED matrix will flash for ~0.5 seconds to indicate entering edit mode. After the watch enters edit mode, the first column (tens digit of hour) will start flashing (it may not flash if it's 0). Press the button once to increment the current column/digits value (it will wrap automatically, eg. between 0-2 for tens digit of hour or 0-5 for the tens digit of minute). After setting the current digit to the desired value, long press the button to move on to the next digit (the next column will start flashing). Long pressing on the last digit/column (the ones column of minute) will save the newly set time to the RTC and exit edit mode.

## Hardware

### PCB Design

The qron0b PCB is designed in the free and open source EDA software, KiCAD. You can find the PCB design source files under [pcb/](https://github.com/qewer33/qron0b/tree/main/pcb).

![schematic](https://github.com/qewer33/qron0b/blob/main/assets/schematic.png?raw=true)

![pcb_kicad](https://github.com/qewer33/qron0b/blob/main/assets/pcb_kicad_screenshot.png?raw=true)

![pcb](https://github.com/qewer33/qron0b/blob/main/assets/qron0b_pcb.png?raw=true)

### Mechanical Design

The mechanical parts of qron0b (the watch casing) is designed in the free and open source CAD software, FreeCAD. The watch case is designed in 2 parts: the bottom base and the top "glass". They connect together via a simple clip system. The bottom base casing accepts 22mm GT2/GT3 style watch straps.

You can find the CAD source and 3D print files under [cad/](https://github.com/qewer33/qron0b/tree/main/cad).

![freecad](https://github.com/qewer33/qron0b/blob/main/assets/freecad_screenshot.png?raw=true)

## Firmware

The qron0b firmware is written in bare-metal AVR C. It's written to be as minimal as possible and is only 1916 bytes, meaning it can even run on the ATtiny24A (in fact, it's designed for the 24A!).

For more information on the firmware, see the README.md file inside [firmware/](https://github.com/qewer33/qron0b/tree/main/firmware).
