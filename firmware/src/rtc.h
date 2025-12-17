#include <avr/io.h>

// RTC functions
void rtc_init();

void rtc_write_byte(uint8_t data);
uint8_t rtc_read_byte();

void rtc_write_register(uint8_t reg, uint8_t data);
uint8_t rtc_read_register(uint8_t reg);

void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);
void rtc_get_time(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);

// Helpers
uint8_t bcd_to_binary(uint8_t bcd);
uint8_t binary_to_bcd(uint8_t bin);
