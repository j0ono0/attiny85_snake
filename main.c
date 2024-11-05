

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h> 
#include <avr/pgmspace.h>


#include "ssd1306.h"
#include "t85_i2c.h"
#include "t85_adc.h" // button input
#include "t85_pwm.h"
#include "engine.h"






void moveboxes(box **boxarr, int len)
{
    for(int i = 0; i < len; ++i)
    {
        boxarr[i]->x1 += boxarr[i]->x_dir;
        boxarr[i]->x2 += boxarr[i]->x_dir;
        boxarr[i]->y1 += boxarr[i]->y_dir;
        boxarr[i]->y2 += boxarr[i]->y_dir;
        if((boxarr[i]->y2 >= 63) | (boxarr[i]->y1 <= 0))
        {
            boxarr[i]->y_dir *= -1;
        }
        if((boxarr[i]->x2 >= 127) | (boxarr[i]->x1 <= 0))
        {
            boxarr[i]->x_dir *= -1;
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

    audio_config();

    pwm_config();

    bool collision = false; // Track plr collision status

    box plr = {.x1=16, .x2=31, .y1=16, .y2=31, .x_dir=0, .y_dir=0};
    box b1 = {.x1=1, .x2=7, .y1=0, .y2=7, .x_dir=2, .y_dir=2};
    // box b2 = {.x1=10, .x2=17, .y1=50, .y2=57, .x_dir=0, .y_dir=1};
    // box b3 = {.x1=18, .x2=25, .y1=13, .y2=20, .x_dir=0, .y_dir=-1};
    // box b4 = {.x1=27, .x2=34, .y1=40, .y2=47, .x_dir=0, .y_dir=-1};
    // box b5 = {.x1=50, .x2=57, .y1=40, .y2=47, .x_dir=0, .y_dir=1};
    // box b6 = {.x1=70, .x2=77, .y1=30, .y2=37, .x_dir=0, .y_dir=-1};
    // box b7 = {.x1=30, .x2=37, .y1=50, .y2=57, .x_dir=0, .y_dir=1};
    // box b8= {.x1=80, .x2=87, .y1=20, .y2=27, .x_dir=2, .y_dir=2};
    
    // box *boxarr[] = {&plr, &b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8};
    box *boxarr[] = {&plr, &b1};

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


    while(1)
    {

        set_column_address(0, 127);
        set_page_address(0, 7);
        ssd1306_start_data();
        
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
                        box *b = boxarr[i];
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

        if(boxes_overlap(&plr, &b1))
        {
            if(!collision)
            {
                start_tune();
                collision = true;
            }
        }
        else
        {
            collision = false;
        }
        update_audio();


        enum btn_input btn = read_buttons();
        
        if (btn == BTN_N)
        {
            plr.y_dir = -2;
            plr.x_dir = 0;
        }
        else if (btn == BTN_E)
        {
            plr.x_dir = 2;
            plr.y_dir = 0;
        }
        else if (btn == BTN_W)
        {
            plr.x_dir = -2;
            plr.y_dir = 0;
        }
        else if (btn == BTN_S)
        {
            plr.y_dir = 2;
            plr.x_dir = 0;
        }
    }
    return 0;
}