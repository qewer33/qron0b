#include <avr/io.h>

void disable_unused();
void setup_wakeup_pin();
void timer_init();
void enter_deep_sleep();
void get_compile_time(uint8_t* h, uint8_t* m, uint8_t* s);