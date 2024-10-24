
#include "t85_adc.h"

void init_adc()
{
    // ADMUX defaults:

    // REFS[2:0] = Vcc used as voltage reference

    
    ADMUX |= (1 << ADLAR) |             // Left adjust result (8bit accuracy)
             (1 << MUX1) | (1 << MUX0); //  Analog Channe: PB3
    
    // ADC clock must run between 50kHz and 200KHz. Main clock running at 8MHz
    
    ADCSRA |= (1 << ADEN) |                 // ADC Enable
              (1 << ADPS2) | (1 << ADPS1); // Prescale divide factor = 64
                 
}

uint8_t read_adc()
{
    ADCSRA |= (1 << ADSC); // Start Conversion

    // Wait for conversion to complete
    while(!(ADCSRA & (1 << ADSC)));

    // Read ADCH register for result
    uint8_t result = ADCH;
    return result;
}