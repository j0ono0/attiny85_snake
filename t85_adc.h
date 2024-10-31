#ifndef ADC_attiny85_constants_h
#define ADC_attiny85_constants_h

#include <stdbool.h>
#include <util/delay.h> 
#include <avr/io.h>


enum btn_input {
    BTN_N,
    BTN_E,
    BTN_S,
    BTN_W,
    BTN_NULL
};


typedef struct {
    enum btn_input current;
    bool processed;
} BtnHandler;

void init_adc();
uint16_t read_adc();
enum btn_input read_buttons();
enum btn_input current_button();
bool button_processed();
void mark_button_processed();

#endif