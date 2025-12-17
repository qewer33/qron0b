#pragma once

#define F_CPU 1000000UL // 1 MHz clock

// Pin definitions
#define KEY PA0

// Shift register pins
#define REG_SER PA7
#define REG_SRCLK PB1
#define REG_RCLK PB2
#define REG_SRCLR PB0

// RTC pins
#define RTC_CE PA1
#define RTC_SCLK PA3
#define RTC_IO PA2
