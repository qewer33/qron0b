#include "rtc.h"
#include "gpio.h"

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

// Initialize RTC
void rtc_init() {
    PIN_OUTPUT(DDRA, RTC_CE);
    PIN_OUTPUT(DDRA, RTC_SCLK);
    PIN_OUTPUT(DDRA, RTC_IO);
    PIN_LOW(PORTA, RTC_CE);
    PIN_LOW(PORTA, RTC_SCLK);

    // Disable trickle charger (TCS) to reduce standby current
    rtc_write_register(0x90, 0x00);
}

// Write a single byte to the RTC
void rtc_write_byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        // Send data LSB
        if (data & 0x01)
            PIN_HIGH(PORTA, RTC_IO);
        else
            PIN_LOW(PORTA, RTC_IO);

        _delay_us(1);

        // Pulse clock
        PIN_HIGH(PORTA, RTC_SCLK);
        _delay_us(1);
        PIN_LOW(PORTA, RTC_SCLK);
        
        data >>= 1; // Shift data right
    }
}

// Read a single byte from the RTC
uint8_t rtc_read_byte() {
    uint8_t data = 0;
    PIN_LOW(PORTA, RTC_IO);
    PIN_INPUT(DDRA, RTC_IO);

    for (uint8_t i = 0; i < 8; i++) {
        data >>= 1; // Shift data right
        
        // Read and set data MSB
        if (IS_PIN_HIGH(PINA, RTC_IO))
            data |= 0x80;

        // Pulse clock
        PIN_HIGH(PORTA, RTC_SCLK);
        _delay_us(1);
        PIN_LOW(PORTA, RTC_SCLK);
    }

    PIN_OUTPUT(DDRA, RTC_IO);
    return data;
}

// Write specified data to an RTC register
void rtc_write_register(uint8_t reg, uint8_t data) {
    PIN_HIGH(PORTA, RTC_CE);
    _delay_us(1);
    rtc_write_byte(reg);
    rtc_write_byte(data);
    _delay_us(1);
    PIN_LOW(PORTA, RTC_CE);
}

// Read data from an RTC register
uint8_t rtc_read_register(uint8_t reg) {
    uint8_t data;

    PIN_HIGH(PORTA, RTC_CE);
    _delay_us(1);
    rtc_write_byte(reg);
    data = rtc_read_byte();
    _delay_us(1);
    PIN_LOW(PORTA, RTC_CE);

    return data;
}

// Set the current RTC time
void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    rtc_write_register(0x8E, 0x00); // Disable write protection
    rtc_write_register(0x80, binary_to_bcd(seconds) & 0x7F); // Ensure clock is running
    rtc_write_register(0x82, binary_to_bcd(minutes));
    rtc_write_register(0x84, binary_to_bcd(hours));
    rtc_write_register(0x8E, 0x80); // Enable write protection
}

// Get the current time form RTC
void rtc_get_time(uint8_t *hours, uint8_t *minutes, uint8_t *seconds) {
    *seconds = bcd_to_binary(rtc_read_register(0x81)) & 0x7F;
    *minutes = bcd_to_binary(rtc_read_register(0x83));
    *hours = bcd_to_binary(rtc_read_register(0x85));
}

// Convert BCD to binary
uint8_t bcd_to_binary(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Convert binary to BCD
uint8_t binary_to_bcd(uint8_t bin) {
    return ((bin / 10) << 4) | (bin % 10);
}
