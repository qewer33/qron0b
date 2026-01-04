#include <avr/io.h>

#define DEBOUNCE_TIME 50
#define LONG_PRESS_TIME 800

typedef enum {
    KEY_NONE,
    KEY_PRESS,
    KEY_LONG_PRESS
} KeyEvent;

KeyEvent read_key(volatile uint16_t time);
