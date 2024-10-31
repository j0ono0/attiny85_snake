#ifndef GAMEENGINE_j0ono0_h
#define GAMEENGINE_j0ono0_h

    #include <stdio.h>
    #include "ssd1306.h"
    #include "t85_adc.h"

    void btn_ctrl();
    void draw_box(uint8_t x, uint8_t y);
    void led_on();
    void led_off();
    void toggle_led();
#endif