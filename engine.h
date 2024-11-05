#ifndef GAMEENGINE_j0ono0_h
#define GAMEENGINE_j0ono0_h
    
    #include <avr/interrupt.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include "ssd1306.h"
    #include "t85_pwm.h"


    // IDEA: space saver - upper nibble as x1, lower nibble as x2
    typedef struct box {
        int8_t x1;
        int8_t x2;
        int8_t y1;
        int8_t y2;
        int8_t x_dir;
        int8_t y_dir;
    } box;

    void audio_config();
    bool boxes_overlap(box *b1, box *b2);
    void draw_box(uint8_t x, uint8_t y);
    void led_on();
    void led_off();
    void toggle_led();
    void start_tune();
    void update_audio();
#endif