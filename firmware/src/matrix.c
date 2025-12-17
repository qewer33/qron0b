#include "matrix.h"
#include "gpio.h"

#include <avr/io.h>
#include <util/delay.h>

// Initialize shift register
void shift_register_init() {
    // Set pins as outputs
    PIN_OUTPUT(DDRA, REG_SER);
    PIN_OUTPUT(DDRB, REG_SRCLK);
    PIN_OUTPUT(DDRB, REG_RCLK);
    PIN_OUTPUT(DDRB, REG_SRCLR);

    // Pulse reset
    PIN_LOW(PORTB, REG_SRCLR);
    _delay_us(10);
    PIN_HIGH(PORTB, REG_SRCLR);
}

// Write data to the shift register
void shift_register_write(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        // Set data
        if (data & 0x80)
            PIN_HIGH(PORTA, REG_SER);
        else
            PIN_LOW(PORTA, REG_SER);

        _delay_us(10);

        // Pulse clock
        PIN_HIGH(PORTB, REG_SRCLK);
        _delay_us(10);
        PIN_LOW(PORTB, REG_SRCLK);

        data <<= 1; // Shift data
    }

    // Pulse latch
    PIN_HIGH(PORTB, REG_RCLK);
    _delay_us(10);
    PIN_LOW(PORTB, REG_RCLK);
}

// Display a pattern
// Pattern format: Each set of 4 bits corresponds to a row pattern
void matrix_display_pattern(uint16_t pattern) {
    for (uint8_t col = 0; col < 4; col++) {
        uint8_t col_data = 0b11110000; // All columns OFF (Q4-Q7 = HIGH)
        col_data &= ~(1 << (7 - col)); // Enable column `col` (Q4+col = LOW)

        uint8_t row_data = 0;
        for (uint8_t row = 0; row < 4; row++) {
            row_data |= ((pattern >> (row*4 + col)) & 0x01) << row; // Row `row` = Q0-Q3
        }

        shift_register_write(col_data | row_data);
        _delay_us(MATRIX_COL_US);
    }
}

// Display time (hours and minutes) in BCD format
void matrix_display_time(uint8_t hours, uint8_t minutes) {
    uint16_t pattern = 0;
    pattern |= (hours/10) << 12;
    pattern |= (hours % 10) << 8;
    pattern |= (minutes/10) << 4;
    pattern |= (minutes % 10);

    matrix_display_pattern(pattern);
}

// Display time while blinking the specified column
void matrix_blink_time(uint8_t hours, uint8_t minutes, uint8_t col, uint16_t time_ms) {
    // Blink at ~1 Hz based on time in ms
    uint8_t blink_state = ((time_ms / 500) & 0x01) == 0;

    uint16_t pattern = 0;
    if (!(blink_state && col == 0)) pattern |= (hours/10) << 12;
    if (!(blink_state && col == 1)) pattern |= (hours % 10) << 8;
    if (!(blink_state && col == 2)) pattern |= (minutes/10) << 4;
    if (!(blink_state && col == 3)) pattern |= (minutes % 10);

    matrix_display_pattern(pattern);
}


// Turn off all LEDs
void matrix_all_off() {
    shift_register_write(0xF0); // Columns HIGH rows LOW
}

// Turn on all LEDs
void matrix_all_on() {
    shift_register_write(0x0F); // Columns LOW rows HIGH
}
