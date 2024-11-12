

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
cell snake[10];
uint8_t snake_len = 0;

void init_timer()
{
    
    TCCR1 |= (1 << CTC1)                                        // Clear Timer/Counter on Compare Match
          | (1 << CS13)| (1 << CS12)| (1 << CS11)| (1 << CS10); // CK/16384
}


bool collision(){
    // test is snake head (snake[0] shares location with other snake cells)
    for(uint8_t i = 1; i < snake_len; ++i)
    {
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        { 
            return true;
        }
    }
    return false;
}

bool move_snake(int8_t dx, int8_t dy)
{
    dx *= CELL_SIZE;
    dy *= CELL_SIZE;
    // Cancel movement beyond screen boundaries
    // or if stationary
    if (
        (dx == 0 && dy == 0 )||
        dx + snake[0].x < 0 ||
        dx + snake[0].x >= DISPLAY_WIDTH - 1 ||
        dy + snake[0].y < 0 ||
        dy + snake[0].y >= DISPLAY_HEIGHT - 1
    )
    {
        return false;
    }

    // Move body cells into next location
    for(uint8_t i = snake_len; i > 0; --i)
    {
        snake[i].x = snake[i-1].x; 
        snake[i].y = snake[i-1].y; 
    }

    // Move snake head
    snake[0].x += dx;
    snake[0].y += dy;
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
            for(int i = 0; i < snake_len; ++i)
            {
                cell *asset = &snake[i];
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
            for(uint8_t i=0; i < snake_len; ++i)
            {
                if(snake[i].x == col*8 && snake[i].y == page*8)
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

    // Set up snake
    // ============
    
    // Add Snake head
    snake[0].x = 64;
    snake[0].y = 16;
    ++snake_len;

    // Add body cells
    snake[1].x = 56;
    snake[1].y = 16;
    ++snake_len;

    snake[2].x = 48;
    snake[2].y = 16;
    ++snake_len;

    snake[3].x = 40;
    snake[3].y = 16;
    ++snake_len;

    snake[4].x = 32;
    snake[4].y = 16;
    ++snake_len;


    // Set global timer initial value
    timemark = global_timer();

    // Start snake as stationary
    next_direction = BTN_NULL;


	// ssd1306_send_progmem_data(default_image_length, image_2); // ssd1306 raw example: show an image
    // _delay_ms(1000);


    while(1)
    {
        uint64_t _timemark = global_timer();
        if(_timemark - timemark > 10)
        {
            // Move snake
            switch (next_direction)
            {

                // Move snake
                case BTN_N:
                    move_snake(0, -1);
                    break;
                case BTN_E:
                    move_snake(1, 0);
                    break;
                case BTN_S:
                    move_snake(0, 1);
                    break;
                case BTN_W:
                    move_snake(-1, 0);
                    break;
                default:
                    move_snake(0, 0);
            }

            // Update display
            render_tiles();

            // Check for collision
            if(collision())
            {
                if(!audio_is_playing())
                {
                start_tune(&riff_lose);
                led_on();
                }
            }
            else
            {
                led_off();
            }

            // Restart timer
            timemark = _timemark;
        }

        // Checking for new user input
        btn = read_buttons();
        if(btn != BTN_NULL && btn != BTN_ERROR)
        {
            next_direction = btn;
        }
  
        update_audio();


    }
    

    return 0;
}