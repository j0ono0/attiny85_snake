#include "engine.h"



const uint8_t sym_blank[] PROGMEM = {};
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

const glyph symbols[] PROGMEM = {
    {0, sym_blank}, // Blank
    {8, sym_x_square}, // x_square
    {8, sym_square}, // square
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
                        memcpy_P(&this_glyph, &symbols[1], sizeof(this_glyph));
                    }else{
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

void render_title()
{
	set_column_address(0, 127);
    set_page_address(0, 7);
    ssd1306_start_data();
	
	for(uint8_t i = 0; i < 4 ; ++i)
	{
		i2c_write_byte(pgm_read_byte(&(sym_s[i])));
	}
	i2c_write_byte(0x0);
	
	for(uint8_t i = 0; i < 5 ; ++i)
	{
		i2c_write_byte(pgm_read_byte(&(sym_n[i])));
	}
	i2c_write_byte(0x0);
	
	
	for(uint8_t i = 0; i < 5 ; ++i)
	{
		i2c_write_byte(pgm_read_byte(&(sym_a[i])));
	}
	i2c_write_byte(0x0);
	
	
	for(uint8_t i = 0; i < 4 ; ++i)
	{
		i2c_write_byte(pgm_read_byte(&(sym_k[i])));
	}
	i2c_write_byte(0x0);
	
	
	for(uint8_t i = 0; i < 4 ; ++i)
	{
		i2c_write_byte(pgm_read_byte(&(sym_e[i])));
	}
	i2c_write_byte(0x0);
	
	ssd1306_stop();
}