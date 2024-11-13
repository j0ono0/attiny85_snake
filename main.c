
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h> 
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "ssd1306.h"
#include "t85_i2c.h"
#include "user_input.h" // button input
#include "audio.h"
#include "engine.h"
#include "timer.h"

uint64_t timemark;
enum btn_input next_direction;

uint8_t ptn2[] = {0xFF, 0x81, 0xA5, 0x99, 0x99, 0xA5, 0x81, 0xFF}; // X in square
uint8_t ptn1[] = {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF}; // Outlined square
uint8_t ptn0[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Blank (black)
enum btn_input btn;
// As snake grows head moves along array
cell assets [128];
cell *target = &assets[0];
cell *snake_head = &assets[1];
uint8_t assets_len = 2;


bool out_of_bounds(uint8_t x, uint8_t y)
{
    if (x < 0 || y < 0 || x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT)
    {
        return true;
    }
    return false;
}

bool target_at_location(uint8_t x, uint8_t y)
{
    return target->x == x && target->y == y; 
}

bool snake_at_location(uint8_t x, uint8_t y)
{
    cell *asset = snake_head;
    while (--asset != target)
    {
        if(asset->x == x && asset->y == y)
        return true;
    }
    return false;
}


void move_target()
{
    do {
    target->x = rand() % (DISPLAY_WIDTH / CELL_SIZE) * CELL_SIZE;
    target->y = rand() % (DISPLAY_HEIGHT / CELL_SIZE) * CELL_SIZE;
    } while (snake_at_location(target->x, target->y));
}

void grow_snake(uint8_t x, uint8_t y)
{
    ++snake_head;
    ++assets_len;
    snake_head->x = x;
    snake_head->y = y;
    move_target();
}


bool move_snake(int8_t x, int8_t y)
{

    // Move body cells into next location
    for(uint8_t i = 1; i < assets_len - 1; ++i)
    {
        assets[i].x = assets[i+1].x; 
        assets[i].y = assets[i+1].y; 
    }
    // Move snake head
    snake_head->x = x;
    snake_head->y = y;
    
    return true;
}


void render3()
{
    set_column_address(0, 127);
    set_page_address(0, 7);
    ssd1306_start_data();
    for(uint8_t page = 0; page < 8; ++page)
    {
        for(uint8_t col = 0; col < 128; ++col)
        {
            uint8_t x = col;
            uint8_t y = page * 8;
            uint8_t bytebuffer = 0x0;
            for(int i = 0; i < assets_len; ++i)
            {
                cell *asset = &assets[i];
                if (
                    asset->x <= x && asset->x + CELL_SIZE > x
                )
                {
                    if(asset->y <= y && asset->y + CELL_SIZE > y)
                    {
                        int8_t ox = x - asset->x;
                        int8_t oy = y - asset->y;
                        bytebuffer = (ptn1[ox] >> oy);
                        break;
                    }
                    else if(asset->y > y && asset->y < y+8)
                    {
                        int8_t ox = x - asset->x;
                        int8_t oy = asset->y - y;
                        bytebuffer = (ptn1[ox] << oy);
                        break;
                    }
                }
            }
            i2c_write_byte(bytebuffer);
        }
    }
    ssd1306_stop();
}


void render_tiles()
{
    // Render tiles 8x8 size
    set_column_address(0, 127);
    set_page_address(0, 7);
    ssd1306_start_data();
    
    uint8_t *pattern = ptn0;

    for(uint8_t page = 0; page < 8; ++page)
    {
        for(uint8_t col = 0; col < 16; ++col)
        {
            // Set blank tile as default
            pattern = ptn0;

            // Search for snake cell at [col, page] location
            for(uint8_t i=0; i < assets_len; ++i)
            {
                if(assets[i].x == col*8 && assets[i].y == page*8)
                {
                    if(i == 0)
                    {
                        pattern = ptn2;
                    }else{
                        pattern = ptn1;
                    }
                    break;
                }
            }
            for(uint8_t ptn_val = 0; ptn_val < 8; ++ptn_val)
            {
                i2c_write_byte(pattern[ptn_val]);
            }
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
    init_global_timer();
    audio_config();
    init_adc();
	i2c_init();
	ssd1306_init();
    clear_screen();


    // Setup game
    // ==========

    snake_head->x = 64;
    snake_head->y = 24;

    move_target();

    // Snake starts stationary
    next_direction = BTN_NULL;

    // Set global timer initial value
    timemark = global_timer();

	// ssd1306_send_progmem_data(default_image_length, image_2); // ssd1306 raw example: show an image
    // _delay_ms(1000);


    while(1)
    {
        uint64_t _timemark = global_timer();
        if(_timemark - timemark > 10)
        {
            uint8_t dx = 0;
            uint8_t dy = 0;
            // Move snake
            switch (next_direction)
            {
                case BTN_N:
                    dy = -1;
                    break;
                case BTN_E:
                    dx = 1;
                    break;
                case BTN_S:
                    dy = 1;
                    break;
                case BTN_W:
                    dx = -1;
                    break;
                default:
            }

            uint8_t xx = snake_head->x + (dx * CELL_SIZE);
            uint8_t yy = snake_head->y + (dy * CELL_SIZE);
            if(target_at_location(xx, yy))
            {
                grow_snake(xx, yy);
            }
            else if (snake_at_location(xx, yy))
            {
                if(!audio_is_playing())
                start_tune(&riff_lose);
                led_on(); 
                // temp keep moving snake
                move_snake(xx, yy);
            }
            else if(out_of_bounds(xx, yy))
            {
                led_on(); 
                // temp stop snake moving
                dx = 0;
                dy = 0;
            }
            else
            {
                move_snake(xx, yy);
                led_off();
            }

            // Update display
            render_tiles();

            // Restart timer
            timemark = _timemark;
        }

        // Checking for new user input
        btn = read_buttons();
        if(btn == BTN_AUX_W)
        {
            led_on();
            _delay_ms(1000);
            led_off();
        }
        else if(btn != BTN_NULL && btn != BTN_ERROR)
        {
            next_direction = btn;
        }
  
        update_audio();

    }
    

    return 0;
}