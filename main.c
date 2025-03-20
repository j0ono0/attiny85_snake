
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h> 
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "user_input.h" // button input
#include "audio.h"
#include "engine.h"
#include "timer.h"


const alpha_glyph_pos title_text[] PROGMEM = {s,n,a,k,e,space}; // 6 chars 
const alpha_glyph_pos end_text[] PROGMEM = {g,a,m,e,space,o,v,e,r}; // 9 chars
const alpha_glyph_pos all_time_best_text[] PROGMEM = {a,l,l,space,t,i,m,e,space,b,e,s,t,space}; //14 chars
const alpha_glyph_pos session_best_text[] PROGMEM = {s,e,s,s,i,o,n,space,b,e,s,t,space}; //13 chars
const alpha_glyph_pos score_text[] PROGMEM = {s,c,o,r,e,space}; // 6 chars
const alpha_glyph_pos new_text[] PROGMEM = {n,e,w,space}; // 4 chars

// EEPROM address for high score
uint16_t eeprom_addr_high_score = 0x0;

uint64_t timemark;
uint8_t high_score_all = 0;
uint8_t high_score_session = 0;
enum btn_input next_direction;
enum btn_input btn;


// As snake grows head moves along array
cell assets [128];
cell *target = &assets[0];
cell *snake_head = &assets[1];
uint8_t assets_len = 2;

bool target_at_location(uint8_t x, uint8_t y)
{
    return target->x == x && target->y == y; 
}


bool snake_at_location(uint8_t x, uint8_t y)
{
    for(uint8_t i = 1; i < assets_len; ++i)
    {
        if(assets[i].x == x && assets[i].y == y)
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

void init_game()
{
    snake_head = &assets[1];
    snake_head->x = 64;
    snake_head->y = 24;
    assets_len = 2;
    next_direction = BTN_NULL;
    clear_screen();
    render_text(0, 0, 6, title_text);
    render_text(0, 2, 14, all_time_best_text);
    render_number(high_score_all);
    render_text(0, 3, 13, session_best_text);
    render_number(high_score_session);
    while(read_action_buttons() != BTN_AUX_E) 
    { 
        continue;
    }
    srand(global_timer());
    move_target();
}

void end_game()
{
    uint8_t row = 0;
    start_tune(&riff_lose);

    render_text(0, row++, 9, end_text);
    
    if(assets_len -1 > high_score_all)
    {
        high_score_all = assets_len - 1;
        high_score_session = high_score_all;
        EEPROM_write(eeprom_addr_high_score, high_score_all);
        render_text(0, row, 14, new_text);
        render_text(24, row++, 14, all_time_best_text);
    }
    else if(assets_len - 1 > high_score_session)
    {
        high_score_session = assets_len - 1;
        render_text(0, row, 14, new_text);
        render_text(24, row++, 13, session_best_text);
    }
    render_text(0, row, 6, score_text);
    render_number(assets_len-1);
    
    while(read_action_buttons() != BTN_AUX_W) 
    { 
        update_audio();
        // Wait for btn press
    }
    stop_tune();
    init_game();
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

    if(read_action_buttons() == BTN_AUX_W && read_dpad_buttons() == BTN_W)
    {
        // Reset high score in EEPROM
        EEPROM_write(eeprom_addr_high_score, 0x0);
    }

    high_score_all = EEPROM_read(eeprom_addr_high_score);
    

    // Enter start screen and initialise game values
    init_game();

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

            if (dx == 0 && dy == 0)
            {
                //Snake is motionless! Do nothing.
            }
            else if(target_at_location(xx, yy))
            {
                grow_snake(xx, yy);
                start_tune(&riff_rebound_bottom);
            }
            else if (snake_at_location(xx, yy))
            {
                end_game();
            }
            else
            {
                move_snake(xx, yy);
            }


            // Restart timer
            timemark = _timemark;
        }

        // Checking for new user input
        enum btn_input action_btn = read_action_buttons();
        enum btn_input dpad_btn = read_dpad_buttons();

        // if(action_btn != BTN_NULL && action_btn != BTN_ERROR)
        // {
            
        // }
        
        if(action_btn == BTN_AUX_W)
        {
            // Effectively pauses the game
            next_direction = BTN_NULL;
        }
        else if(dpad_btn != BTN_NULL && dpad_btn != BTN_ERROR)
        {
            next_direction = dpad_btn;
        }

        // Update display
        render_tiles(assets, &assets_len);
        update_audio();
    
    }

    return 0;
}