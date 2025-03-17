#ifndef GAMEENGINE_j0ono0_h
#define GAMEENGINE_j0ono0_h
    
    #include <avr/pgmspace.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include "ssd1306.h"
    #include "t85_i2c.h"


    #define DISPLAY_WIDTH 128
    #define DISPLAY_HEIGHT 64
    // Cell width and height
    #define CELL_SIZE 8


    typedef struct tile_location {
        uint8_t x;
        uint8_t y;
    } cell;
    

    typedef struct glyph {
        const uint8_t len;
        const uint8_t *data;
    } glyph;


    void render_tiles(cell *assets, uint8_t *assets_len);
    void render_title();

#endif