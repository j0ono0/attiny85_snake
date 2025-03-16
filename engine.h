#ifndef GAMEENGINE_j0ono0_h
#define GAMEENGINE_j0ono0_h
    
    #include <avr/pgmspace.h>
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
    
    const uint8_t sym_blank[] PROGMEM = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    const uint8_t sym_x_square[] PROGMEM = {0xFF, 0x81, 0xA5, 0x99, 0x99, 0xA5, 0x81, 0xFF};
    const uint8_t sym_square[] PROGMEM = {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF};
    const uint8_t sym_a[] PROGMEM = {0xc0, 0x30, 0x2c, 0x30, 0xc0};
    const uint8_t sym_b[] PROGMEM = {0xfc, 0x94, 0x94, 0x94, 0x68};
    const uint8_t sym_c[] PROGMEM = {0x78, 0x84, 0x84, 0x84, 0x48};
    const uint8_t sym_d[] PROGMEM = {0xfc, 0x84, 0x84, 0x84, 0x78};
    const uint8_t sym_e[] PROGMEM = {0xfc, 0x94, 0x94, 0x84};
    const uint8_t sym_f[] PROGMEM = {0xfc, 0x14, 0x14, 0x4};
    const uint8_t sym_g[] PROGMEM = {0x78, 0x84, 0x84, 0xa4, 0xe8};
    const uint8_t sym_h[] PROGMEM = {0xfc, 0x10, 0x10, 0xfc};
    const uint8_t sym_i[] PROGMEM = {0xfc};
    const uint8_t sym_j[] PROGMEM = {0xc0, 0x80, 0x7c};
    const uint8_t sym_k[] PROGMEM = {0xfc, 0x30, 0x48, 0x84};
    const uint8_t sym_l[] PROGMEM = {0xfc, 0x80, 0x80};
    const uint8_t sym_m[] PROGMEM = {0xfc, 0xc, 0x30, 0xc0, 0x30, 0xc, 0xfc};
    const uint8_t sym_n[] PROGMEM = {0xfc, 0xc, 0x30, 0xc0, 0xfc};
    const uint8_t sym_o[] PROGMEM = {0x78, 0x84, 0x84, 0x84, 0x78};
    const uint8_t sym_p[] PROGMEM = {0xfc, 0x24, 0x24, 0x18};
    const uint8_t sym_q[] PROGMEM = {0x78, 0x84, 0xa4, 0x44, 0xb8};
    const uint8_t sym_r[] PROGMEM = {0xfc, 0x24, 0x24, 0x24, 0xd8};
    const uint8_t sym_s[] PROGMEM = {0x48, 0x94, 0xa4, 0x48};
    const uint8_t sym_t[] PROGMEM = {0x4, 0x4, 0xfc, 0x4, 0x4};
    const uint8_t sym_u[] PROGMEM = {0x7c, 0x80, 0x80, 0x80, 0x7c};
    const uint8_t sym_v[] PROGMEM = {0xc, 0x30, 0xc0, 0x30, 0xc};
    const uint8_t sym_w[] PROGMEM = {0xc, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc};
    const uint8_t sym_x[] PROGMEM = {0x84, 0x48, 0x30, 0x48, 0x84};
    const uint8_t sym_y[] PROGMEM = {0x4, 0x8, 0xf0, 0x8, 0x4};
    const uint8_t sym_z[] PROGMEM = {0xc4, 0xa4, 0x94, 0x8c};

    typedef struct glyph {
        const uint8_t len;
        const uint8_t *data;
    } glyph;

    const glyph symbols[] PROGMEM = {
        {1, sym_blank}, // Blank
        {8, sym_x_square}, // x_square
        {8, sym_square}, // square
    };

    //TODO: store these in PROGMEM
    const glyph glyph_blank = {.len=1, .data=sym_blank};
    const glyph glyph_x_square = {.len=8, .data=sym_x_square};
    const glyph glyph_square = {.len=8, .data=sym_square};

    // const glyph glyph_a  = {.len=5, .data=sym_a};
    // const glyph glyph_b  = {.len=5, .data=sym_b};
    // const glyph glyph_c  = {.len=5, .data=sym_c};
    // const glyph glyph_m  = {.len=7, .data=sym_m};


    void led_on();
    void led_off();
    void toggle_led();
#endif