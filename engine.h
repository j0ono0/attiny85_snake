#ifndef GAMEENGINE_j0ono0_h
#define GAMEENGINE_j0ono0_h
    
    #include <avr/pgmspace.h>
    #include <avr/interrupt.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include "ssd1306.h"
    #include "t85_i2c.h"

    #define DISPLAY_WIDTH 128
    #define DISPLAY_HEIGHT 64
    // Cell width and height
    #define CELL_SIZE 8
    #define DISPLAY_CELL_WIDTH 16
    #define DISPLAY_CELL_HEIGHT 8


    typedef struct tile_location {
        uint8_t x;
        uint8_t y;
    } cell;
    
    typedef struct node {
        uint8_t next;
        uint8_t prev;
    } node;

    typedef struct glyph {
        const uint8_t len;
        const uint8_t *data;
    } glyph;


    void render_tiles(node *assets);
    void render_number(uint8_t num);
    void render_text_P(const char *text); // text stored in progmem
    void render_char(char character);
    void EEPROM_write(uint16_t ucAddress, uint8_t  ucData);
    uint8_t EEPROM_read(uint16_t ucAddress);
#endif