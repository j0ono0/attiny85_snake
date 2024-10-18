

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h> 
#include <avr/pgmspace.h> // ssd1306

#include "ssd1306.h"
#include "i2c_t85.h"


void blink_light()
{
        // Set built-in LED pin as output
    DDRB |= (1 << DDB3);
    while (1) {
        PORTB |=  (1 << PB3);   // LED on
        _delay_ms(1000);
        PORTB &= ~(1 << PB3);   // LED off
        _delay_ms(500);
    }
}


int main()
{
	i2c_init();

	ssd1306_init();
	ssd1306_send_data(default_image_length, image_1); // ssd1306 raw example: show an image

    while(1)
    {
        blink_light();
    }
    
    return 0;

}