#include "key.h"
#include "gpio.h"

// Read event from key
KeyEvent read_key(volatile uint16_t time) {
    static uint16_t start_time = 0; // Start time of the event
    static uint8_t handled = 0; // Has the event been handled already?
    
    if (IS_PIN_HIGH(PINA, KEY)) { // Button press
        
        // Event begin
        if (start_time == 0) {
            start_time = time;
            handled = 0;
        }

        // Trigger long press
        if (!handled && (uint16_t)(time - start_time) > LONG_PRESS_TIME) {
            handled = 1;
            return KEY_LONG_PRESS;
        }
    } else { // Button release
        // If an event is ongoing
        if (start_time != 0) {
            uint16_t duration = time - start_time;
            start_time = 0; // Reset event timer

            // Trigger short press
            if (!handled && duration > DEBOUNCE_TIME) {
                return KEY_PRESS;
            }
        }
    }

    return KEY_NONE;
}
