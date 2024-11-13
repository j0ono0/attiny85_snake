#ifndef ADC_attiny85_constants_h
#define ADC_attiny85_constants_h

#include <stdbool.h>
#include <util/delay.h> 
#include <avr/io.h>
#include <avr/interrupt.h>


enum btn_input {
    BTN_N,
    BTN_E,
    BTN_S,
    BTN_W,
    BTN_AUX,
    BTN_AUX_W,
    BTN_NULL,
    BTN_ERROR
};

void init_adc();
uint16_t read_adc();
enum btn_input read_buttons();

#endif