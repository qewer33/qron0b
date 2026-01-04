#include "init.h"
#include "matrix.h"
#include "rtc.h"
#include "key.h"

#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

// Flag to set comptime only on first boot after flash
#define COMPTIME_FLAG_ADDR 0x00

// Global variable to track system uptime (in ms)
volatile uint16_t uptime = 0;

// Watch state management
typedef enum {
    STATE_DISPLAY,
    STATE_EDIT
} State;

static State state = STATE_DISPLAY;
static uint8_t h, m, s;

// Edit mode management
typedef enum {
    EDIT_HOUR_TENS,
    EDIT_HOUR_ONES,
    EDIT_MINUTE_TENS,
    EDIT_MINUTE_ONES,
} EditMode;

static EditMode edit_mode = EDIT_HOUR_TENS;
static uint8_t edit_h = 0;
static uint8_t edit_m = 0;
static void state_display(void);
static void state_edit(KeyEvent key_ev);

int main(void) {
    MCUSR = 0;
    wdt_disable();
        
    // Hardware init
    disable_unused();
    setup_wakeup_pin();
    shift_register_init();
    rtc_init();
    timer_init();
    
    // Enable watchdog timer
    wdt_enable(WDTO_2S);

    sei(); // Enable interrupts

    // Use compile time clock only after first flash
    if (eeprom_read_byte(COMPTIME_FLAG_ADDR) == 0xFF) {
        get_compile_time(&h, &m, &s);
        rtc_set_time(h, m, s);
        // Update flag
        eeprom_update_byte(COMPTIME_FLAG_ADDR, 0x00);
    }

    wdt_reset();
    
    // Main loop
    while (1) {
        wdt_reset();
        
        KeyEvent key_ev = read_key(uptime);

        // Switch to edit mode if key long pressed on display mode
        if (state == STATE_DISPLAY && key_ev == KEY_LONG_PRESS) {
            // Consume key event
            key_ev = KEY_NONE;

            // Switch mode and state
            edit_mode = EDIT_HOUR_TENS;
            state = STATE_EDIT;

            // Initialize edit buffers
            edit_h = h;
            edit_m = m;

            // Visual cue for edit mode enter
            matrix_all_on();
            _delay_ms(500);
            matrix_all_off();
            _delay_ms(250);
        }

        switch (state) {
            case STATE_DISPLAY:
                state_display();
            break;
            case STATE_EDIT:
                state_edit(key_ev);
            break;
        }
    }
}

// Display state
static void state_display(void) {
    // Show display for 10 seconds (10000 ms)
    if (uptime < 10000) {
        // Refresh time each second
        if (uptime % 1000 == 0) {
            rtc_get_time(&h, &m, &s);
        }

        matrix_display_time(h, m);

        _delay_us(600); // Idle between frames to cut average LED load
    } else {
        wdt_disable(); // Disable WDT
        enter_deep_sleep();
        // After sleep
        wdt_enable(WDTO_2S); // Re-enable WDT
        rtc_get_time(&h, &m, &s);
        uptime = 0;
    }
}

// Edit state
static void state_edit(KeyEvent key_ev) {
    switch (edit_mode) {
        // Editing first digit of hour
        case EDIT_HOUR_TENS:
            if (key_ev == KEY_PRESS) {
                uint8_t tens = edit_h / 10;
                tens = (tens + 1) % 3; // 0-2
                if (tens == 2 && (edit_h % 10) > 3) {
                    edit_h = 23;
                } else {
                    edit_h = tens * 10 + (edit_h % 10);
                }
            }

            matrix_blink_time(edit_h, edit_m, 0, uptime);

            if (key_ev == KEY_LONG_PRESS) edit_mode = EDIT_HOUR_ONES;
            break;

        // Editing second digit of hour
        case EDIT_HOUR_ONES:
            if (key_ev == KEY_PRESS) {
                uint8_t tens = edit_h / 10;
                uint8_t ones = edit_h % 10;
                ones = (ones + 1) % 10;
                if (tens == 2 && ones > 3) ones = 0;
                edit_h = tens * 10 + ones;
            }

            matrix_blink_time(edit_h, edit_m, 1, uptime);

            if (key_ev == KEY_LONG_PRESS) edit_mode = EDIT_MINUTE_TENS;
            break;

        // Editing first digit of minute
        case EDIT_MINUTE_TENS:
            if (key_ev == KEY_PRESS) {
                uint8_t tens = edit_m / 10;
                tens = (tens + 1) % 6; // 0-5
                edit_m = tens * 10 + (edit_m % 10);
            }

            matrix_blink_time(edit_h, edit_m, 2, uptime);

            if (key_ev == KEY_LONG_PRESS) edit_mode = EDIT_MINUTE_ONES;
            break;

        // Editing second digit of minute
        case EDIT_MINUTE_ONES:
            if (key_ev == KEY_PRESS) {
                uint8_t tens = edit_m / 10;
                uint8_t ones = edit_m % 10;
                ones = (ones + 1) % 10;
                edit_m = tens * 10 + ones;
            }

            matrix_blink_time(edit_h, edit_m, 3, uptime);

            // Save new time to RTC and return to display state
            if (key_ev == KEY_LONG_PRESS) {
                rtc_set_time(edit_h, edit_m, 0);
                h = edit_h;
                m = edit_m;
                s = 0;
                state = STATE_DISPLAY;
            }
            break;
    }

    _delay_us(600); // Idle between frames to cut average LED load
}

// Wakeup interrupt
ISR(PCINT0_vect) {}

// TIM1 interrupt
ISR(TIM1_COMPA_vect) {
    uptime++;
}
