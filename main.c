

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
    uint8_t x;
    uint8_t y;
} box;


int main()
{
    //==================================================
    // Enable update to CLKPR
    CLKPR=0x80; 
    // Change prescaler to 1 (run attiny85 at 8Mhz)
    CLKPR=0;
    //==================================================


    // box b1 = {.x=0, .y=0};

	i2c_init();
	ssd1306_init();


    clear_screen();

    // Draw a box
    set_column_address(8, 15);
	set_page_address(1, 1);
    ssd1306_start();
    i2c_write_byte(SSD1306_CONTROL_BYTE_DATA);
    for(int i = 0; i < 8; ++i)
    {
        i2c_write_byte(0xFF<<3);
    }
    ssd1306_stop();
    // Draw a box bottom
    set_column_address(8, 15);
	set_page_address(2, 2);
    ssd1306_start();
    i2c_write_byte(SSD1306_CONTROL_BYTE_DATA);
    for(int i = 0; i < 8; ++i)
    {
        i2c_write_byte(0xFF>>5);
    }
    ssd1306_stop();




	// ssd1306_send_progmem_data(default_image_length, image_2); // ssd1306 raw example: show an image
    // _delay_ms(1000);

    // uint8_t x_dir = 1;

    while(1)
    {
        // clear_screen();
        // draw_box(b2.x, b2.y);
        // draw_box(b1.x, b1.y);
        // _delay_ms(250);
        // b1.x += x_dir;
        // if(b1.x == 15){
        //     x_dir = -1;
        // }else if(b1.x == 0)
        // {
        //     x_dir = 1;
        // }



    }
    
    return 0;

}