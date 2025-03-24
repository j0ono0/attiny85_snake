
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


const char text_snake[] PROGMEM = {"snake"}; 
const char text_end[] PROGMEM = {"game over"}; 
const char text_all_time_best[] PROGMEM = {"all time best "};
const char text_session_best[] PROGMEM = {"session best "}; 
const char text_score[] PROGMEM = {"score "}; 
const char text_new[] PROGMEM = {"new "}; 

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
cell *snake_tail = &assets[1];
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

void grow_sound_riff()
{
     // Queue 'grow sound'
    if((assets_len - 2) == high_score_all && high_score_all != 0)
    {
        start_tune(&riff_win_big);
    }
    else if(
        (assets_len - 2) == high_score_session && high_score_session != 0)
    {
        start_tune(&riff_win_small);
    }
    else if((assets_len - 1) % 5 == 0)
    {
        start_tune(&riff_gain_big);
    }else{
        start_tune(&riff_gain_small);
    }
}

void grow_snake(uint8_t x, uint8_t y)
{

    ++snake_head;
    ++assets_len;

    // Move cells along so new cell can be slotted in *before* 'snake_tail'
    cell *current = snake_head;
    cell *prev = snake_head - 1;
    while(current != snake_tail)
    {
        current->x = prev->x;
        current->y = prev->y;
        --current;
        --prev;
    }
    // Assign snake_head value to snake_tail
    // This is becomes the newly inserted snake cell
    snake_tail->x = snake_head->x;
    snake_tail->y = snake_head->y;

    // Move snake_tail to new tail position
    if(++snake_tail == snake_head)
    {
        snake_tail = &assets[1];
    }
    // Update snake_head to new head position
    snake_head->x = x;
    snake_head->y = y;
}


void move_snake(int8_t x, int8_t y)
{
    snake_tail->x = snake_head->x;
    snake_tail->y = snake_head->y;

    if(++snake_tail >= snake_head)
    {
        snake_tail = &assets[1];
    }

    // Move snake head
    snake_head->x = x;
    snake_head->y = y;
}

void init_game()
{
    snake_head = &assets[1];
    snake_tail = &assets[1];
    snake_head->x = 64;
    snake_head->y = 24;
    assets_len = 2;
    next_direction = BTN_NULL;
    clear_screen();
    
    set_column_address(0,127);
    set_page_address(0,7);
    render_text_P(text_snake);
    
    set_column_address(0,127);
    set_page_address(1,7);
    render_text_P(text_all_time_best);
    render_number(high_score_all);
    
    
    set_column_address(0,127);
    set_page_address(2,7);
    render_text_P(text_session_best);
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

    
    set_column_address(0,127);
    set_page_address(row++,7);
    render_text_P(text_end);
    
    if(assets_len -1 > high_score_all)
    {
        high_score_all = assets_len - 1;
        high_score_session = high_score_all;
        EEPROM_write(eeprom_addr_high_score, high_score_all);
        set_column_address(0,127);
        set_page_address(row++,7);
        render_text_P(text_new);
        render_text_P(text_all_time_best);
    }
    else if(assets_len - 1 > high_score_session)
    {
        high_score_session = assets_len - 1;
        set_column_address(0,127);
        set_page_address(row++,7);
        render_text_P(text_new);
        render_text_P(text_session_best);
    }
    set_column_address(0,127);
    set_page_address(row,7);
    render_text_P(text_score);
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
                move_target();
                grow_sound_riff();
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