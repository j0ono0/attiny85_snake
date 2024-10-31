#include "engine.h"





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

void play_tune()
{
    static bool playing = false;
    static int8_t i = 0;

    uint8_t tune[] = {239, 213, 190, 179, 159, 142, 127, 119};
    uint8_t length = sizeof(tune) / sizeof(tune[0]);

    if (!playing)
    {
        playing = true;
        play_tone(tune[i]);
        i = (i + 1) % length;
    }
    else
    {
        stop_tone();
        playing = false;
    }
}