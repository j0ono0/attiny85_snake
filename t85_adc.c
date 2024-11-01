
#include "t85_adc.h"

#define BTN_INPUT_LEN 6
#define BTN_COUNT_THRESHOLD 80


void init_adc()
{
    ADMUX |= (1 << MUX1) | (1 << MUX0);     //  Analog Channel: PB3
    
    ADCSRA |= (1 << ADEN) |                 // ADC Enable
              (1 << ADPS2) | (1 << ADPS1);  // Prescale divide factor = 64
}

uint16_t read_adc()
{

        ADCSRA |= (1 << ADSC);          // Start Conversion
        while(!(ADCSRA & (1 << ADSC))); // Wait for conversion to complete
        uint8_t low_byte = ADCL;
        return ADCH<<8 | low_byte;
}

enum btn_input read_buttons()
{
    // Log button readings
    uint8_t count[BTN_INPUT_LEN] = {0};

    for(int i = 0; i < 500; ++i)
    {
        cli();
        int val = read_adc();
        sei();
        if (val < 940 && val > 920 ) // 930
        {
            if(++count[BTN_N] >= BTN_COUNT_THRESHOLD)
                return BTN_N;
        }
        else if (val < 863 && val > 843) // 853
        {
            if(++count[BTN_E] >= BTN_COUNT_THRESHOLD)
                return BTN_E;
        }
        else if (val < 711 && val > 691) // 701
        {
            if(++count[BTN_S] >= BTN_COUNT_THRESHOLD)
                return BTN_S;
        }
        else if (val < 522 && val > 502) // 512
        {
            if(++count[BTN_W] >= BTN_COUNT_THRESHOLD)
                return BTN_W;
        }
        else if(val < 50)
        { 
            if(++count[BTN_NULL] >= BTN_COUNT_THRESHOLD)
                return BTN_NULL;
        }
    }
    return BTN_ERROR;
}
