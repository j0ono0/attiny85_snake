#include "engine.h"



bool boxes_overlap(box *b1, box *b2)
{
    if(
        (b1->x1 > b2->x2) |
        (b1->x2 < b2->x1) |
        (b1->y1 > b2->y2) |
        (b1->y2 < b2->y1)
    )
    {
        return false;
    }
    return true;
}


void draw_box(uint8_t x, uint8_t y)
{
    // draw a box 8x8 pixels into a 8x8 grid
    // screen grid is 16 wide x 8 high
    set_column_address(x*8, x*8 + 7);

    set_page_address(y, y+1);
    ssd1306_start_data();
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
