

#include "timer.h"

uint64_t counter = 0;

ISR (TIMER0_COMPA_vect)
{
    ++counter;
}

void init_global_timer()
{

    // Initialises interrupt on timer/counter 0
    // ========================================

    TCCR0A = 0x00;
    TCCR0B = 0x00;

    TCCR0A  |= (1 << WGM01);                // Mode 2.  Clear Timer on Compare Match (CTC) mode
    TCCR0B |= (1 << CS02) | (1 << CS00);    // Clk/1024 prescale
    
    OCR0A = 195;                            // Sets interrupt to occur every 25ms (at 8MHz clock)
    TCNT0 = 0;

    TIMSK|=(1<<OCIE0A);                     // enable specific interrupt 
}

uint64_t global_timer()
{
    cli();
    uint64_t _counter = counter;
    sei();
    return _counter;
}