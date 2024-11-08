#include "engine.h"





void led_on()
{
	DDRB |= (1 << DDB4);
	PORTB |=  (1 << PB4); 
}

void led_off()
{
	PORTB &=  ~(1 << PB4); 
}

void toggle_led()
{
    PORTB ^= (1 << PB4);
}
