

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h> 
#include <avr/pgmspace.h> // ssd1306

#include "ssd1306.h"
#include "i2c_t85.h"


void green_led_on()
{
	DDRB |= (1 << DDB3);
	PORTB |=  (1 << PB3);   // green LED on
}

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


void draw_box(uint8_t x, uint8_t y)
{
    // draw a box 8x8 pixels into a 8x8 grid
    // screen grid is 16 wide x 8 high
    set_column_address(x*8, x*8 + 7);

    set_page_address(y, y+1);
    ssd1306_start();
    i2c_write_byte(SSD1306_CONTROL_BYTE_DATA);
    for(int i=0; i < 8; ++i)
    {
        i2c_write_byte(0xFF);
    }
    ssd1306_stop();
}

typedef struct box {
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
} box;


int main()
{
    //==================================================
    // Enable update to CLKPR
    CLKPR=0x80; 
    // Change prescaler to 1 (run attiny85 at 8Mhz)
    CLKPR=0;
    //==================================================

    // IDEA: space saver - upper nibble as x1, lower nibble as x2

    box b1 = {.x1=0, .x2=7, .y1=4, .y2=11};
    
    uint8_t dy = 1;

	i2c_init();
	ssd1306_init();


    clear_screen();


	// ssd1306_send_progmem_data(default_image_length, image_2); // ssd1306 raw example: show an image
    // _delay_ms(1000);


    while(1)
    {
    ssd1306_start();
    i2c_write_byte(SSD1306_CONTROL_BYTE_DATA);
        
        for(uint8_t page = 0; page < 8; ++page)
        {
            for(uint8_t col = 0; col < 128; ++col)
            {
                uint8_t bytebuffer = 0x00;
                for(uint8_t bit = 0; bit < 8; ++bit)
                {
                    uint8_t x = col;
                    uint8_t y = page * 8 + bit;
                    if(x >= b1.x1 && x <= b1.x2 && y >= b1.y1 && y <= b1.y2)
                    {
                        bytebuffer |= (1 << bit);
                    }
                }
            i2c_write_byte(bytebuffer);

            }
        }
    ssd1306_stop();
    b1.y1 += dy;
    b1.y2 += dy;
    if(b1.y1 == 0)
    {
        dy = 1;
    }
    else if(b1.y2 == 63){
        dy = -1;
    }
        _delay_ms(0);
    }
    
    return 0;

}