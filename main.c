
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


// 0-127: tile locations on display
// 128: blank
// 129: target
// 130: snake head -> prev leads to snake head in display
// 131: snake tail -> next leads to snake tail in display
node assets [132];
node *snake_head;
node *snake_tail;


uint8_t snake_length(){
    uint8_t count = 1;
    node *ptr = snake_tail;
    while(ptr != snake_head)
    {
        ptr = &assets[ptr->next];
        ++count;
    }
    return count;
}

void place_target()
{
    uint8_t ind;
    do {
        ind = rand() % (DISPLAY_CELL_WIDTH * DISPLAY_CELL_HEIGHT);
    } while (assets[ind].next != 128 && assets[ind].prev != 128);

    assets[ind].next = 129;
    assets[ind].prev = 129;
}

void grow_sound_riff()
{
    uint8_t snake_len = snake_length();

    if((snake_len - 1) == high_score_all && high_score_all != 0)
    {
        start_tune(&riff_win_big);
    }
    else if(
        (snake_len - 1) == high_score_session && high_score_session != 0)
    {
        start_tune(&riff_win_small);
    }
    else if((snake_len) % 5 == 0)
    {
        start_tune(&riff_gain_big);
    }else{
        start_tune(&riff_gain_small);
    }
}


void init_game()
{
    // assign assets array to 'blank' location
    for(uint8_t i = 0; i < 128; ++i)
    {
        assets[i] = (node){.next = 128, .prev = 128};
    }

    // Place snake in middle(ish) of screen
    snake_head = &assets[36];
    snake_tail = &assets[52];
    // Update graph with snake
    *snake_head = (node){.next=36, .prev=52};
    *snake_tail = (node){.next=36, .prev=52};

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
    place_target();

}

void end_game()
{
    uint8_t snake_len = snake_length();

    uint8_t row = 0;
    start_tune(&riff_lose);

    set_column_address(0,127);
    set_page_address(row++,7);
    render_text_P(text_end);
    
    if(snake_len > high_score_all)
    {
        high_score_all = snake_len;
        high_score_session = high_score_all;
        EEPROM_write(eeprom_addr_high_score, high_score_all);
        set_column_address(0,127);
        set_page_address(row++,7);
        render_text_P(text_new);
        render_text_P(text_all_time_best);
    }
    else if(snake_len > high_score_session)
    {
        high_score_session = snake_len;
        set_column_address(0,127);
        set_page_address(row++,7);
        render_text_P(text_new);
        render_text_P(text_session_best);
    }
    set_column_address(0,127);
    set_page_address(row,7);
    render_text_P(text_score);
    render_number(snake_len);
    
    while(read_action_buttons() != BTN_AUX_E) 
    { 
        // Wait for btn press
        update_audio();
    }
    stop_tune();
    _delay_ms(500);
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
                default:
                    break;
            }
            
            if(dx + dy > 0)
            {
                // Calculate coordinates from asset index
                uint8_t x = (snake_head - assets) % DISPLAY_CELL_WIDTH + dx;
                uint8_t y = (snake_head - assets) / DISPLAY_CELL_WIDTH + dy;
                // Wrap coordinates at screen edges
                x = x % DISPLAY_CELL_WIDTH;
                if(x < 0)
                {x += DISPLAY_CELL_WIDTH;}
                
                y = y % DISPLAY_CELL_HEIGHT;
                if(y < 0)
                {y += DISPLAY_CELL_HEIGHT;}
                
                uint8_t index = y * DISPLAY_CELL_WIDTH + x;

                if(assets[index].next == 128 && assets[index].prev == 128)
                {
                    // Blank location
                    // Move head to next location
                    node *new_head = &assets[y * DISPLAY_CELL_WIDTH + x];
                    snake_head->next = new_head - assets;
                    *new_head = (node){.next=new_head - assets, .prev=snake_head->next};
                    snake_head = new_head;
                    
                    //move tail up
                    node *new_tail = &assets[snake_tail->next];
                    snake_tail->prev = 128;
                    snake_tail->next = 128;
                    snake_tail = new_tail;
                }
                else if(assets[index].next == 129 || assets[index].prev == 129)
                {
                    // Target at location, grow snake
                    assets[index].prev = snake_head->next;
                    snake_head->next = index;
                    snake_head = &assets[index];
                    snake_head->next = index;

                    place_target();
                    grow_sound_riff();

                }
                else{
                    // Snake at location
                    end_game();
                }
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
        render_tiles(assets);
        update_audio();
    
    }

    return 0;
}