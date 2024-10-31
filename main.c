

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h> 
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "ssd1306.h"
#include "t85_i2c.h"
#include "t85_adc.h" // button input
#include "t85_pwm.h"
#include "engine.h"


int intr_count=0;
ISR (TIMER0_COMPA_vect)
{
    if (intr_count++ == 25) //waiting for 100 count because to get 1 sec compare match should occur 100 times
    {
        toggle_led();
        play_tune();
        intr_count=0; //making intr_count=0 to continue the process
    }

}

// IDEA: space saver - upper nibble as x1, lower nibble as x2
typedef struct box {
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
    uint8_t x_dir;
    uint8_t y_dir;
} box;

void moveboxes(box *boxarr, int len)
{
    for(int i = 0; i < len; ++i)
    {
        boxarr[i].x1 += boxarr[i].x_dir;
        boxarr[i].x2 += boxarr[i].x_dir;
        boxarr[i].y1 += boxarr[i].y_dir;
        boxarr[i].y2 += boxarr[i].y_dir;
        if((boxarr[i].y2 >= 63) | (boxarr[i].y1 <= 0))
        {
            boxarr[i].y_dir *= -1;
        }
        if((boxarr[i].x2 >= 127) | (boxarr[i].x1 <= 0))
        {
            boxarr[i].x_dir *= -1;
        }
    }
}




int main()
{
    //==================================================
    // Enable update to CLKPR
    CLKPR=0x80; 
    // Change prescaler to 1 (run attiny85 at 8Mhz)
    CLKPR=0;
    //==================================================

    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    TCCR0A = 0x00;
    TCCR0B = 0x00;

    TCCR0A  |= (1 << WGM01); // Mode 2.  Clear Timer on Compare Match (CTC) mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // Clk/1024 prescale
    
    OCR0A = 78;
    TCNT0 = 0;

    sei();   //enabling global interrupts
    TIMSK|=(1<<OCIE0A); // enable specific interrupt 


    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////




    pwm_config();


    box b1 = {.x1=1, .x2=16, .y1=0, .y2=7, .x_dir=0, .y_dir=1};
    box b2 = {.x1=10, .x2=17, .y1=50, .y2=57, .x_dir=0, .y_dir=1};
    box b3 = {.x1=18, .x2=25, .y1=13, .y2=20, .x_dir=0, .y_dir=-1};
    box b4 = {.x1=27, .x2=34, .y1=40, .y2=47, .x_dir=0, .y_dir=-1};
    box b5 = {.x1=50, .x2=57, .y1=40, .y2=47, .x_dir=0, .y_dir=1};
    box b6 = {.x1=70, .x2=77, .y1=30, .y2=37, .x_dir=0, .y_dir=-1};
    box b7 = {.x1=30, .x2=37, .y1=50, .y2=57, .x_dir=0, .y_dir=1};
    box b8= {.x1=80, .x2=87, .y1=20, .y2=27, .x_dir=2, .y_dir=2};
    
    box boxarr[] = {b1, b2, b3, b4, b5, b6, b7, b8};

	i2c_init();
	ssd1306_init();
    init_adc();

    clear_screen();

    led_on();
    _delay_ms(500);
    led_off();
    _delay_ms(500);

	// ssd1306_send_progmem_data(default_image_length, image_2); // ssd1306 raw example: show an image
    // _delay_ms(1000);

    bool processed = false;

    while(1)
    {
        enum btn_input btn = read_buttons();

        if(btn == BTN_NULL)
        {
        processed = false;
        set_column_address(0, 127);
        set_page_address(0, 7);
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

                        for(int i = 0; i < sizeof(boxarr)/sizeof(boxarr[0]); ++i)
                        {
                            box *b = &boxarr[i];
                            if(x >= b->x1 && x <= b->x2 && y >= b->y1 && y <= b->y2 )
                            {
                                bytebuffer |= (1 << bit);
                            }
                        }
                    }
                i2c_write_byte(bytebuffer);

                }
            }
        ssd1306_stop();
        moveboxes(boxarr, sizeof(boxarr)/sizeof(boxarr[0]));
        }
        
        else if (btn == BTN_N)
        {
            if(processed == false)
            {
                clear_screen();
                processed = true;
            }
            draw_box(7, 0);
        }
        else if (btn == BTN_E)
        {
            if(processed == false)
            {
                clear_screen();
                processed = true;
            }
            draw_box(14, 3);
        }
        else if (btn == BTN_W)
        {
            if(processed == false)
            {
                clear_screen();
                processed = true;
            }
            draw_box(0, 3);
        }
        else if (btn == BTN_S)
        {
            if(processed == false)
            {
                clear_screen();
                processed = true;
            }
            draw_box(7, 7);
        }
        else
        {
            clear_screen();
        }
        
    }
    
    return 0;

}