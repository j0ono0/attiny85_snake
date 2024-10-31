
#include "t85_adc.h"



BtnHandler bh = {.current = BTN_NULL, .processed = false};


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

void update_bh(enum btn_input input)
{
            bh.current = input;
            bh.processed  = false;
    //  if(bh.current != input)
    //     {
    //     }
}

enum btn_input read_buttons()
{
    // TODO?: inverse the checking order for default action to return BTN_NULL

    int val = read_adc();

    if (val < 10)
    {
        return BTN_NULL;
        // update_bh(BTN_NULL);
    } 
    // button is pressed
    if (val < 520)
    {
        return BTN_N;
        // update_bh(BTN_N);
    }
    else if (val < 710)
    {
        return BTN_E;
        // update_bh(BTN_W);
    }
    else if (val < 870)
    {
        return BTN_S;
        // update_bh(BTN_S);
    }
    else
    {
        return BTN_W;
        // update_bh(BTN_E);
    }
}

enum btn_input current_button()
{
    return bh.current;
}
bool button_processed()
{
    return bh.processed;
}
void mark_button_processed()
{
    bh.processed = true;
}