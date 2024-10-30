#include "engine.h"


#define BTN_HISTORY_LEN 20
uint8_t button_history[BTN_HISTORY_LEN];
uint8_t button_index = 0;


void btn_ctrl()
{

    int val = read_adc();
    if(val < 120)
    {
        clear_screen();
    }
    else if ( val < 135)
    {
        draw_box(7, 0);
    }
    else if (val < 180){
        draw_box(15, 4);
    }
    else if (val < 223) {
        draw_box(7, 7);
    }
    else{
        draw_box(0, 4);
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


void led_on()
{
	DDRB |= (1 << DDB4);
	PORTB |=  (1 << PB4); 
}


void led_off()
{
	PORTB &=  ~(1 << PB4); 
}