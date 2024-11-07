

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h> 
#include <avr/pgmspace.h>


#include "ssd1306.h"
#include "t85_i2c.h"
#include "t85_adc.h" // button input
#include "audio.h"
#include "engine.h"


#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})




// bool collision = false; // Track plr collision status

box plr = {.x1=0, .x2=15, .y1=56, .y2=60, .x_dir=0, .y_dir=0};
box ball1 = {.x1=16, .x2=20, .y1=16, .y2=20, .x_dir=2, .y_dir=2};

box b1 = {.x1=1, .x2=15, .y1=9, .y2=13, .x_dir=0, .y_dir=0};
box b2 = {.x1=17, .x2=31, .y1=9, .y2=13, .x_dir=0, .y_dir=0};
box b3 = {.x1=33, .x2=47, .y1=9, .y2=13, .x_dir=0, .y_dir=0};
box b4 = {.x1=49, .x2=63, .y1=9, .y2=13, .x_dir=0, .y_dir=0};
box b5 = {.x1=65, .x2=79, .y1=9, .y2=13, .x_dir=0, .y_dir=-0};
box b6 = {.x1=81, .x2=95, .y1=9, .y2=13, .x_dir=0, .y_dir=0};
box b7= {.x1=97, .x2=111, .y1=9, .y2=13, .x_dir=0, .y_dir=0};
box b8= {.x1=113, .x2=127, .y1=9, .y2=13, .x_dir=0, .y_dir=0};

box *entityArray[] = {&b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8, &plr, &ball1};




void movePlr(box *plr)
{
    // Limit movement to screen boundaries
    if(plr->x_dir < 0)
    {
        // Moving left
        plr->x_dir = max(-1*plr->x1, plr->x_dir);
    }
    else if ((127 - plr->x2) < plr->x_dir)
    {
        // Moving right
        plr->x_dir = 127 - plr->x2;
    }
    // update box corners
    plr->x1 += plr->x_dir;
    plr->x2 += plr->x_dir;
    plr->y1 += plr->y_dir;
    plr->y2 += plr->y_dir;

}

void moveBall(box *ball, box **eArr)
{
    // update box corners
    ball->x1 += ball->x_dir;
    ball->x2 += ball->x_dir;
    ball->y1 += ball->y_dir;
    ball->y2 += ball->y_dir;
    // Bounce off display sides
    if(ball->x1 <= 0)
    {
        ball->x_dir *= -1;
    } 
    if(ball->x2 >= 127)
    {
        ball->x_dir *= -1;
    } 

}

void render_display()
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

                for(int i = 0; i < sizeof(entityArray)/sizeof(entityArray[0]); ++i)
                {
                    box *b = entityArray[i];
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
}


int main()
{
    //==================================================
    // Enable update to CLKPR
    CLKPR=0x80; 
    // Change prescaler to 1 (run attiny85 at 8Mhz)
    CLKPR=0;
    //==================================================


    // Configure hardware
    // ==================

    audio_config();
    init_adc();
	i2c_init();
	ssd1306_init();
    





	// ssd1306_send_progmem_data(default_image_length, image_2); // ssd1306 raw example: show an image
    // _delay_ms(1000);


    while(1)
    {

        
        // Update display
        //===============
        render_display();



        // Read button input
        //==================

        // TODO: Read buttons within long display render. And queue button presses?
        enum btn_input btn = read_buttons();
        
        if (btn == BTN_N)
        {
            plr.y_dir = -8;
            plr.x_dir = 0;
        }
        else if (btn == BTN_E)
        {
            plr.x_dir = 8;
            plr.y_dir = 0;
        }
        else if (btn == BTN_W)
        {
            plr.x_dir = -8;
            plr.y_dir = 0;
        }
        else if (btn == BTN_S)
        {
            plr.y_dir = 8;
            plr.x_dir = 0;
        }
        else
        { 
            plr.x_dir = 0;
            plr.y_dir = 0;
        }

        movePlr(&plr);
        moveBall(&ball1, entityArray);

        if (ball1.y2 >= plr.y2)
        {
            // Ball out-of-bounds
            start_tune(&riff_lose);
            while(update_audio());

            // Reset game
            ball1.x1 = plr.x1+4;
            ball1.x2 = plr.x1+7;
            ball1.y1 = plr.y1 - 4;
            ball1.y2 = plr.y1 - 1;
            ball1.x_dir = 2;
            ball1.y_dir = -2;

        }
        else if(boxes_overlap(&plr, &ball1))
        {
            // Plr hits ball
            ball1.y_dir = -2;
            start_tune(&riff_rebound_bottom);
        }
        else if(ball1.y1 <= 13)
        {
            // Plr hits ball
            ball1.y_dir = 2;
            start_tune(&riff_rebound_top);
        }
    

        if(update_audio())
        {
            led_on();
        }
        else
        {
            led_off();
        }


    }
    return 0;
}