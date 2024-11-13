#ifndef GAMEENGINE_j0ono0_h
#define GAMEENGINE_j0ono0_h
    

    #include <stdbool.h>
    #include <stdio.h>
    #include "ssd1306.h"
    #include "audio.h"


    #define DISPLAY_WIDTH 128
    #define DISPLAY_HEIGHT 64
    // Cell width and height
    #define CELL_SIZE 8


    typedef struct tile_location {
        uint8_t x;
        uint8_t y;
    } cell;


    void led_on();
    void led_off();
    void toggle_led();
#endif