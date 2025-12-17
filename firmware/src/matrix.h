#include <avr/io.h>

// Matrix dwell, shorter dwell reduces LED current/duty
#define MATRIX_COL_US 100

// Shift register functions
void shift_register_init();
void shift_register_write(uint8_t data);

// LED matrix functions
void matrix_display_pattern(uint16_t pattern);
void matrix_display_time(uint8_t hours, uint8_t minutes);
void matrix_blink_time(uint8_t hours, uint8_t minutes, uint8_t col, uint16_t time_ms);
void matrix_col_off(uint8_t col);
void matrix_all_off();
void matrix_all_on();
