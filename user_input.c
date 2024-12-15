
#include "user_input.h"

#define BTN_INPUT_LEN 8
#define BTN_COUNT_THRESHOLD 10



void init_adc()
{
    // ADMUX |= (1 << MUX1) | (1 << MUX0);     //  Analog Channel: PB3
    
    ADCSRA |= (1 << ADEN) |                 // ADC Enable
              (1 << ADPS2) | (1 << ADPS1);  // Prescale divide factor = 64
}

uint16_t read_adc()
{
    // Average readings 
    #define AV_COUNT 5
        uint16_t sum = 0;
        for(uint8_t i=0; i<AV_COUNT; ++i)
        {
            ADCSRA |= (1 << ADSC);          // Start Conversion
            while(!(ADCSRA & (1 << ADSC))); // Wait for conversion to complete
            uint8_t low_byte = ADCL;
            sum += ADCH<<8 | low_byte;
        }
        return sum/AV_COUNT;
}

enum btn_input read_action_buttons()
{
    // Log button readings into count
    uint8_t count[BTN_INPUT_LEN] = {0};

    //  Enable analog read: PB4
    ADMUX |= (1 << MUX1);     
    ADMUX &= ~(1 << MUX0);   


    for(int i = 0; i < 200; ++i)
    {
    
        // Read pin 
        cli();
        int val = read_adc();
        sei();

        if(val < 700 && val > 600)
        { 
            if(++count[BTN_AUX_W] >= BTN_COUNT_THRESHOLD)
                return BTN_AUX_W;
        }
        else if(val < 1000 && val > 800)
        {
            if(++count[BTN_AUX_E] >= BTN_COUNT_THRESHOLD)
                return BTN_AUX_E;
        }
        else if(val < 5)
        { 
            if(++count[BTN_NULL] >= BTN_COUNT_THRESHOLD)
                return BTN_NULL;
        }
    }
    return BTN_ERROR;
}


enum btn_input read_dpad_buttons()
{
    //  Enable analog read on PB3
    ADMUX |= (1 << MUX1) | (1 << MUX0);

    // Log button readings into count
    uint8_t count[BTN_INPUT_LEN] = {0};

    for(int i = 0; i < 200; ++i)
    {
        cli();
        int val = read_adc();
        sei();

        if (val < 941 && val > 921 ) // 931
        {
            if(++count[BTN_W] >= BTN_COUNT_THRESHOLD)
                return BTN_W;
        }
        else if (val < 839 && val > 819) // 829
        {
            if(++count[BTN_N] >= BTN_COUNT_THRESHOLD)
                return BTN_N;
        }
        else if (val < 693 && val > 678) // 683
        {
            if(++count[BTN_S] >= BTN_COUNT_THRESHOLD)
                return BTN_S;
        }
        else if (val < 522 && val > 502) // 512
        {
            if(++count[BTN_E] >= BTN_COUNT_THRESHOLD)
                return BTN_E;
        }
        else if(val < 5)
        { 
            if(++count[BTN_NULL] >= BTN_COUNT_THRESHOLD)
                return BTN_NULL;
        }
    }
    return BTN_ERROR;
}
