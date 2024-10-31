
#include "t85_adc.h"

#define AVG_COUNT 20

void init_adc()
{
    ADMUX |= (1 << MUX1) | (1 << MUX0);     //  Analog Channel: PB3
    
    ADCSRA |= (1 << ADEN) |                 // ADC Enable
              (1 << ADPS2) | (1 << ADPS1);  // Prescale divide factor = 64
}

uint16_t read_adc()
{
    uint32_t val = 0;
    for (int i =0; i < AVG_COUNT; i++)
    {
        // _delay_ms(3);
        ADCSRA |= (1 << ADSC);          // Start Conversion
        while(!(ADCSRA & (1 << ADSC))); // Wait for conversion to complete
        uint8_t low_byte = ADCL;
        val += ADCH<<8 | low_byte;
    }
    return val / AVG_COUNT;
}