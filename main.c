

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h> 
#include <avr/pgmspace.h>


#include "ssd1306.h"
#include "t85_i2c.h"
#include "user_input.h" // button input
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

uint8_t ptn2[] = {0xFF, 0x81, 0xA5, 0x99, 0x99, 0xA5, 0x81, 0xFF};
uint8_t ptn1[] = {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF};
enum btn_input btn;
cell snake[10];
uint8_t snake_len = 0;



bool move_snake(int8_t dx, int8_t dy)
{
    // Check game boundaries
    if(
        snake[0].x + dx + CELL_SIZE >= DISPLAY_WIDTH || 
        snake[0].x + dx < 0 ||
        snake[0].y + dy + CELL_SIZE >= DISPLAY_HEIGHT || 
        snake[0].y + dy < 0
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
    snake[0].x += dx * CELL_SIZE;
    snake[0].y += dy * CELL_SIZE;
    return true;
}

void render2()
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
                if( 
                    x >= asset->x && 
                    y >= asset->y && 
                    x < asset->x + CELL_SIZE && 
                    y < asset->y + CELL_SIZE
                )
                {
                    uint8_t ax = x - asset->x;
                    uint8_t ay = y - asset->y;
                    bytebuffer = (ptn1[ax] >> ay);
                    break;
                }
            }
            i2c_write_byte(bytebuffer);
        }
    }

    ssd1306_stop();
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

                for(int i = 0; i < snake_len; ++i)
                {
                    cell *asset = &snake[i];

                    if( 
                        x >= asset->x && 
                        x < asset->x + CELL_SIZE && 
                        y >= asset->y && 
                        y < asset->y + CELL_SIZE)
                    {

                        uint8_t *ptn = ptn2;
                        if(i == 0)
                        {
                            ptn = ptn1;
                        }

                        uint8_t ax = x - asset->x;
                        uint8_t ay = y - asset->y;

                        if(ptn[ay] & (1 << ax))
                        {
                            bytebuffer |= (1 << bit);
                        }
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
    clear_screen();

    // Set up snake
    // ============
    
    // Add Snake head
    snake[0].x = 64;
    snake[0].y = 16;
    ++snake_len;

    // Add x2 body cells
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



	// ssd1306_send_progmem_data(default_image_length, image_2); // ssd1306 raw example: show an image
    // _delay_ms(1000);


    while(1)
    {

        
        // Update display
        //===============
        render2();

        
        // Checking for user input
        btn = read_buttons();

        // Read button input
        //==================

        
        if (btn == BTN_N)
        {
            move_snake(0, -1);
        }
        else if (btn == BTN_E)
        {
            move_snake(1, 0);
        }
        else if (btn == BTN_W)
        {
            move_snake(-1, 0);
        }
        else if (btn == BTN_S)
        {
            move_snake(0, 1);
        }
        else if (btn == BTN_ERROR)
        {
            led_on();
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