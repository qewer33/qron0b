#pragma once

#define PIN_INPUT(ddr, pin) (ddr &= ~(1 << pin))
#define PIN_OUTPUT(ddr, pin) (ddr |= (1 << pin))

#define PIN_HIGH(port, pin) (port |= (1 << pin))
#define PIN_LOW(port, pin) (port &= ~(1 << pin))

#define IS_PIN_HIGH(reg, pin) (reg & (1 << (pin)))
#define IS_PIN_LOW(reg, pin) !(reg & (1 << (pin)))
