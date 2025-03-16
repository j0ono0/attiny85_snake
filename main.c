
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


const uint8_t sym_blank[] PROGMEM = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const uint8_t sym_x_square[] PROGMEM = {0xFF, 0x81, 0xA5, 0x99, 0x99, 0xA5, 0x81, 0xFF};
const uint8_t sym_square[] PROGMEM = {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF};
const uint8_t sym_a[] PROGMEM = {0xc0, 0x30, 0x2c, 0x30, 0xc0};
const uint8_t sym_b[] PROGMEM = {0xfc, 0x94, 0x94, 0x94, 0x68};
const uint8_t sym_c[] PROGMEM = {0x78, 0x84, 0x84, 0x84, 0x48};
const uint8_t sym_d[] PROGMEM = {0xfc, 0x84, 0x84, 0x84, 0x78};
const uint8_t sym_e[] PROGMEM = {0xfc, 0x94, 0x94, 0x84};
const uint8_t sym_f[] PROGMEM = {0xfc, 0x14, 0x14, 0x4};
const uint8_t sym_g[] PROGMEM = {0x78, 0x84, 0x84, 0xa4, 0xe8};
const uint8_t sym_h[] PROGMEM = {0xfc, 0x10, 0x10, 0xfc};
const uint8_t sym_i[] PROGMEM = {0xfc};
const uint8_t sym_j[] PROGMEM = {0xc0, 0x80, 0x7c};
const uint8_t sym_k[] PROGMEM = {0xfc, 0x30, 0x48, 0x84};
const uint8_t sym_l[] PROGMEM = {0xfc, 0x80, 0x80};
const uint8_t sym_m[] PROGMEM = {0xfc, 0xc, 0x30, 0xc0, 0x30, 0xc, 0xfc};
const uint8_t sym_n[] PROGMEM = {0xfc, 0xc, 0x30, 0xc0, 0xfc};
const uint8_t sym_o[] PROGMEM = {0x78, 0x84, 0x84, 0x84, 0x78};
const uint8_t sym_p[] PROGMEM = {0xfc, 0x24, 0x24, 0x18};
const uint8_t sym_q[] PROGMEM = {0x78, 0x84, 0xa4, 0x44, 0xb8};
const uint8_t sym_r[] PROGMEM = {0xfc, 0x24, 0x24, 0x24, 0xd8};
const uint8_t sym_s[] PROGMEM = {0x48, 0x94, 0xa4, 0x48};
const uint8_t sym_t[] PROGMEM = {0x4, 0x4, 0xfc, 0x4, 0x4};
const uint8_t sym_u[] PROGMEM = {0x7c, 0x80, 0x80, 0x80, 0x7c};
const uint8_t sym_v[] PROGMEM = {0xc, 0x30, 0xc0, 0x30, 0xc};
const uint8_t sym_w[] PROGMEM = {0xc, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc};
const uint8_t sym_x[] PROGMEM = {0x84, 0x48, 0x30, 0x48, 0x84};
const uint8_t sym_y[] PROGMEM = {0x4, 0x8, 0xf0, 0x8, 0x4};
const uint8_t sym_z[] PROGMEM = {0xc4, 0xa4, 0x94, 0x8c};

const glyph symbols[] PROGMEM = {
    {1, sym_blank}, // Blank
    {8, sym_x_square}, // x_square
    {8, sym_square}, // square
};


// uint8_t ptn3[] = {0x5a, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x5a}; // snake body, vertical
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



void render_tiles()
{
    // Render tiles 8x8 size
    set_column_address(0, 127);
    set_page_address(0, 7);
    ssd1306_start_data();
    
    glyph this_glyph;
    
    for(uint8_t page = 0; page < 8; ++page)
    {
        for(uint8_t col = 0; col < 16; ++col)
        {
            // Set blank tile as default
            memcpy_P(&this_glyph, &symbols[0], sizeof(this_glyph));

            // Search for snake cell at [col, page] location
            for(uint8_t i=0; i < assets_len; ++i)
            {
                if(assets[i].x == col*8 && assets[i].y == page*8)
                {
                    if(i == 0)
                    {
                        memcpy_P(&this_glyph, &symbols[1], sizeof(this_glyph));
                    }else{
                        memcpy_P(&this_glyph, &symbols[2], sizeof(this_glyph));
                    }
                    break;
                }
            }
            // Draw to screen
            for(uint8_t i = 0; i < this_glyph.len; ++i)
            {
                i2c_write_byte(pgm_read_byte(&(this_glyph.data[i])));
            }
            // Fill the remainder width
            for (uint8_t i = 0; i < 8 - this_glyph.len; ++i)
            {
                i2c_write_byte(0x0);
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

   

    while(1)
    {

        uint64_t _timemark = global_timer();
        if(_timemark - timemark > 5)
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
                case BTN_AUX_W:
                    dx = -1;
                    dy = -1;
                    break;
                case BTN_AUX_E:
                    dx = 1;
                    dy = 1;
                    break;
                default:
            }

            uint8_t xx = snake_head->x + (dx * CELL_SIZE);
            uint8_t yy = snake_head->y + (dy * CELL_SIZE);


            // wrap coords around screen
            xx = xx % DISPLAY_WIDTH;
            if (xx < 0) 
            {xx = xx + DISPLAY_WIDTH;}
            
            yy = yy % DISPLAY_HEIGHT;
            if (yy < 0) 
            {yy = yy + DISPLAY_HEIGHT;}


            if(target_at_location(xx, yy))
            {
                grow_snake(xx, yy);
                start_tune(&riff_rebound_bottom);
            }
            else if (snake_at_location(xx, yy))
            {
                if(!audio_is_playing())
                start_tune(&riff_lose);
                // temp keep moving snake
                move_snake(xx, yy);
            }
            else
            {
                move_snake(xx, yy);
            }

            // Update display
            render_tiles();

            // Restart timer
            timemark = _timemark;
        }

        // Checking for new user input
        enum btn_input action_btn = read_action_buttons();
        enum btn_input dpad_btn = read_dpad_buttons();

        if(action_btn != BTN_NULL && action_btn != BTN_ERROR)
        {
            next_direction = action_btn;
        }

        else if(dpad_btn != BTN_NULL && dpad_btn != BTN_ERROR)
        {
            next_direction = dpad_btn;
        }
  
        update_audio();

    }

    return 0;
}