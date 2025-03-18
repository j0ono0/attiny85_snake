#include "engine.h"



const uint8_t sym_blank[] PROGMEM = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const uint8_t sym_x_square[] PROGMEM = {0xFF, 0x81, 0xA5, 0x99, 0x99, 0xA5, 0x81, 0xFF};
const uint8_t sym_square[] PROGMEM = {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF};
const uint8_t sym_snake_head[] PROGMEM = {0xFF, 0x81, 0x81, 0x99, 0x99, 0x81, 0x81, 0xFF};

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
const uint8_t sym_space[] PROGMEM = {};

const uint8_t sym_1[] PROGMEM = {0x8, 0xfc};
const uint8_t sym_2[] PROGMEM = {0xc8, 0xa4, 0xa4, 0x98};
const uint8_t sym_3[] PROGMEM = {0x48, 0x84, 0x94, 0x68};
const uint8_t sym_4[] PROGMEM = {0x60, 0x58, 0xfc, 0x40};
const uint8_t sym_5[] PROGMEM = {0x9c, 0x94, 0x64};
const uint8_t sym_6[] PROGMEM = {0x78, 0x94, 0x94, 0x60};
const uint8_t sym_7[] PROGMEM = {0x4, 0xf4, 0xc};
const uint8_t sym_8[] PROGMEM = {0x68, 0x94, 0x94, 0x68};
const uint8_t sym_9[] PROGMEM = {0x48, 0x94, 0x94, 0x78};
const uint8_t sym_0[] PROGMEM = {0x78, 0x84, 0x84, 0x78};

const glyph symbols[] PROGMEM = {
    {8, sym_blank},
    {8, sym_x_square}, 
    {8, sym_square}, 
    {8, sym_snake_head},
};

// These are variable widths
// NOTE: the order *must* match 'alpha_glyph_pos' enum
const glyph alpha_glyphs[] PROGMEM = {
    {5, sym_a},
    {5, sym_b},
    {5, sym_c},
    {5, sym_d},
    {4, sym_e},
    {4, sym_f},
    {4, sym_g},
    {4, sym_h},
    {1, sym_i},
    {3, sym_j},
    {4, sym_k},
    {3, sym_l},
    {7, sym_m},
    {5, sym_n},
    {5, sym_o},
    {4, sym_p},
    {5, sym_q},
    {5, sym_r},
    {4, sym_s},
    {5, sym_t},
    {5, sym_u},
    {5, sym_v},
    {7, sym_w},
    {5, sym_x},
    {5, sym_y},
    {4, sym_z},
    {0, sym_space},
    {2, sym_1},
    {4, sym_2},
    {4, sym_3},
    {4, sym_4},
    {3, sym_5},
    {4, sym_6},
    {3, sym_7},
    {4, sym_8},
    {4, sym_9},
    {4, sym_0},

};

const glyph number_glyph[] PROGMEM = {
    {4, sym_0},
    {2, sym_1},
    {4, sym_2},
    {4, sym_3},
    {4, sym_4},
    {3, sym_5},
    {4, sym_6},
    {3, sym_7},
    {4, sym_8},
    {4, sym_9},
};

void render_tiles(cell *assets, uint8_t *assets_len)
{
    // Render tiles 8x8 size
    set_column_address(0, 127);
    set_page_address(0, 7);
    ssd1306_start_data();
    
    glyph this_glyph;
    
    for(uint8_t page = 0; page < 8; ++page)
    {
        for(uint8_t col = 0; col < 16; ++col)
        {
            // Set blank tile as default
            memcpy_P(&this_glyph, &symbols[0], sizeof(this_glyph));

            // Search for snake cell at [col, page] location
            for(uint8_t i=0; i < *assets_len; ++i)
            {
                if(assets[i].x == col*8 && assets[i].y == page*8)
                {
                    if(i == 0)
                    {
                        // Target always at assets[0]
                        memcpy_P(&this_glyph, &symbols[1], sizeof(this_glyph));

                    }
                    else if(i == *assets_len - 1)
                    {
                        // Snake head always at assets[assets_len]
                        memcpy_P(&this_glyph, &symbols[3], sizeof(this_glyph));
                    }
                    else{
                        memcpy_P(&this_glyph, &symbols[2], sizeof(this_glyph));
                    }
                    break;
                }
            }
            // Draw to screen
            for(uint8_t i = 0; i < this_glyph.len; ++i)
            {
                i2c_write_byte(pgm_read_byte(&(this_glyph.data[i])));
            }
            // Fill the remainder width
            for (uint8_t i = 0; i < 8 - this_glyph.len; ++i)
            {
                i2c_write_byte(0x0);
            }
        }
    }
    ssd1306_stop();
}



void render_text(uint8_t col, uint8_t row, uint8_t arr_len, const alpha_glyph_pos *glyph_arr)
{
    
	
    set_column_address(col, 127);
    set_page_address(row, 7);
    ssd1306_start_data();

    glyph glyph;
    alpha_glyph_pos glyph_pos;

    for(uint8_t i = 0; i < arr_len; ++i)
    {
        // copy alpha_glyph_pos value from progmem        
        memcpy_P(&glyph_pos, &(glyph_arr[i]), sizeof(alpha_glyph_pos));
        // copy glyph struct from progmem
        memcpy_P(&glyph, &alpha_glyphs[glyph_pos], sizeof(glyph));
        // Write data to screen
        for(uint8_t j = 0; j < glyph.len ; ++j)
        {
            i2c_write_byte(pgm_read_byte(&(glyph.data[j])));
        }
        // Pad between letters
        i2c_write_byte(0x0);
        i2c_write_byte(0x0);
    }

	ssd1306_stop();
}

void render_numeral(uint8_t num){
    if(num > 9)
    {
        return;
    }
    glyph this_glyph;   
    switch(num)
    {
        case 0:
            memcpy_P(&this_glyph, &number_glyph[0], sizeof(this_glyph));
            break;
        case 1:
            memcpy_P(&this_glyph, &number_glyph[1], sizeof(this_glyph));
            break;
        case 2:
            memcpy_P(&this_glyph, &number_glyph[2], sizeof(this_glyph));
            break;
        case 3:
            memcpy_P(&this_glyph, &number_glyph[3], sizeof(this_glyph));
            break;
        case 4:
            memcpy_P(&this_glyph, &number_glyph[4], sizeof(this_glyph));
            break;
        case 5:
            memcpy_P(&this_glyph, &number_glyph[5], sizeof(this_glyph));
            break;
        case 6:
            memcpy_P(&this_glyph, &number_glyph[6], sizeof(this_glyph));
            break;
        case 7:
            memcpy_P(&this_glyph, &number_glyph[7], sizeof(this_glyph));
            break;
        case 8:
            memcpy_P(&this_glyph, &number_glyph[8], sizeof(this_glyph));
            break;
        case 9:
            memcpy_P(&this_glyph, &number_glyph[9], sizeof(this_glyph));
            break;
    }
    
    // set_column_address(col, 127);
    // set_page_address(row, 7);
    ssd1306_start_data();
    for(uint8_t j = 0; j < this_glyph.len ; ++j)
    {
        i2c_write_byte(pgm_read_byte(&(this_glyph.data[j])));
    }

    i2c_write_byte(0x0);
    i2c_write_byte(0x0);
	
    ssd1306_stop();
}

void render_number(uint8_t num)
{
    // The number must be less than 128 (snake fills the whole screen)
    // if(num >= 100){
    //     num = num - 100;
    //     //render_number(1);
    // }
    // if(num >= 10)
    // {
    //     uint8_t tens = (num - num % 10) / 10;
    //     //render_number(tens);
    //     num = num - tens;
    // }
    uint8_t ones = num % 10;
    num /= 10;
    uint8_t tens = num % 10;
    num /= 10;
    uint8_t hundreds = num % 10;
    render_numeral(hundreds);
    render_numeral(tens);
    render_numeral(ones);

    
}