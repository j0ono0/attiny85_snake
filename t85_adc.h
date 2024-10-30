#ifndef ADC_attiny85_constants_h
#define ADC_attiny85_constants_h

#include <util/delay.h> 
#include <avr/io.h>


void init_adc();
uint8_t read_adc();


#endif