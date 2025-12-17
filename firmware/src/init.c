#include "gpio.h"
#include "matrix.h"
#include "rtc.h"

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

// Disable unused peripherals
void disable_unused() {
    ADCSRA &= ~(1 << ADEN); // Disable ADC
    PRR |= (1 << PRADC) | (1 << PRUSI) | (1 << PRTIM0); // Power down unused blocks
    ACSR |= (1 << ACD); // Disable analog comparator
}

// Configure PB0 as the wake-up pin
void setup_wakeup_pin() {
    // Set KEY as input
    PIN_INPUT(DDRA, KEY);

    // Enable interrupts
    PCMSK0 |= (1 << PCINT0);
    GIMSK |= (1 << PCIE0);
}

// Initialize timer (TIM1)
void timer_init() {
    // Reset control registers
    TCCR1A = 0;
    TCCR1B = 0;

    // Set compare match value
    // 1000 ticks at 1MHz = ISR triggers every 1ms
    OCR1A = 999;

    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS10); // No prescaling
    TIMSK1 |= (1 << OCIE1A); // Enable COMPA interrupt
}

// Set CPU to deep sleep
void enter_deep_sleep() {
    // Setup shift register for sleep
    shift_register_write(0xFF);
    PIN_LOW(PORTB, REG_SRCLK);
    PIN_LOW(PORTB, REG_RCLK);
    PIN_LOW(PORTA, REG_SER);
    PIN_HIGH(PORTB, REG_SRCLR); // Keep reset deasserted

    PIN_OUTPUT(DDRA, REG_SER);
    PIN_OUTPUT(DDRB, REG_SRCLK);
    PIN_OUTPUT(DDRB, REG_RCLK);
    PIN_OUTPUT(DDRB, REG_SRCLR);

    // Set sleep mode settings
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    GIFR |= (1 << PCIF0); // Clear any pending pin-change interrupt

    // Sleep...
    sleep_cpu();

    // After sleep
    sleep_disable();
    shift_register_init(); // Restore shift register
}

// Util to parse __TIME__ macro (__TIME__ is "HH:MM:SS")
static uint8_t parse_two_digit(const char *s) {
    return (uint8_t) ((s[0] - '0') * 10 + (s[1] - '0'));
}

// Read compile time from __TIME__ macro
void get_compile_time(uint8_t* h, uint8_t* m, uint8_t* s) {
    *h = parse_two_digit(&__TIME__[0]);
    *m = parse_two_digit(&__TIME__[3]);
    *s = parse_two_digit(&__TIME__[6]);
}
