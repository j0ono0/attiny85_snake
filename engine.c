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



void render_number(uint8_t num)
{
    uint8_t ones = num % 10;
    num /= 10;
    uint8_t tens = num % 10;
    num /= 10;
    uint8_t hundreds = num % 10;
    char buffer[3] = {hundreds + '0', tens + '0', ones + '0',};
    for (uint8_t i = 0; i < 3; i++) {
        render_char(buffer[i]);
    }
    
}   


// Render text from progmem
void render_text_P(const char *text)
{
    char myChar;
    int txt_len = strlen_P(text);
    for (uint8_t i = 0; i < txt_len; i++) {
        myChar = pgm_read_byte(text + i);
        render_char(myChar);
      }
}


void render_char(char character)
{
    glyph glyph;

    switch(character)
    {
        case 'a':
            memcpy_P(&glyph, &alpha_glyphs[0], sizeof(glyph));
            break;
        case 'b':
            memcpy_P(&glyph, &alpha_glyphs[1], sizeof(glyph));
            break;
        case 'c':
            memcpy_P(&glyph, &alpha_glyphs[2], sizeof(glyph));
            break;
        case 'd':
            memcpy_P(&glyph, &alpha_glyphs[3], sizeof(glyph));
            break;
        case 'e':
            memcpy_P(&glyph, &alpha_glyphs[4], sizeof(glyph));
            break;
        case 'f':
            memcpy_P(&glyph, &alpha_glyphs[5], sizeof(glyph));
            break;
        case 'g':
            memcpy_P(&glyph, &alpha_glyphs[6], sizeof(glyph));
            break;
        case 'h':
            memcpy_P(&glyph, &alpha_glyphs[7], sizeof(glyph));
            break;
        case 'i':
            memcpy_P(&glyph, &alpha_glyphs[8], sizeof(glyph));
            break;
        case 'j':
            memcpy_P(&glyph, &alpha_glyphs[9], sizeof(glyph));
            break;
        case 'k':
            memcpy_P(&glyph, &alpha_glyphs[10], sizeof(glyph));
            break;
        case 'l':
            memcpy_P(&glyph, &alpha_glyphs[11], sizeof(glyph));
            break;
        case 'm':
            memcpy_P(&glyph, &alpha_glyphs[12], sizeof(glyph));
            break;
        case 'n':
            memcpy_P(&glyph, &alpha_glyphs[13], sizeof(glyph));
            break;
        case 'o':
            memcpy_P(&glyph, &alpha_glyphs[14], sizeof(glyph));
            break;
        case 'p':
            memcpy_P(&glyph, &alpha_glyphs[15], sizeof(glyph));
            break;
        case 'q':
            memcpy_P(&glyph, &alpha_glyphs[16], sizeof(glyph));
            break;
        case 'r':
            memcpy_P(&glyph, &alpha_glyphs[17], sizeof(glyph));
            break;
        case 's':
            memcpy_P(&glyph, &alpha_glyphs[18], sizeof(glyph));
            break;
        case 't':
            memcpy_P(&glyph, &alpha_glyphs[19], sizeof(glyph));
            break;
        case 'u':
            memcpy_P(&glyph, &alpha_glyphs[20], sizeof(glyph));
            break;
        case 'v':
            memcpy_P(&glyph, &alpha_glyphs[21], sizeof(glyph));
            break;
        case 'w':
            memcpy_P(&glyph, &alpha_glyphs[22], sizeof(glyph));
            break;
        case 'x':
            memcpy_P(&glyph, &alpha_glyphs[23], sizeof(glyph));
            break;
        case 'y':
            memcpy_P(&glyph, &alpha_glyphs[24], sizeof(glyph));
            break;
        case 'z':
            memcpy_P(&glyph, &alpha_glyphs[25], sizeof(glyph));
            break;
        case ' ': // space
            memcpy_P(&glyph, &alpha_glyphs[26], sizeof(glyph));
            break;
            
        case '1':
            memcpy_P(&glyph, &alpha_glyphs[27], sizeof(glyph));
            break;
        case '2':
            memcpy_P(&glyph, &alpha_glyphs[28], sizeof(glyph));
            break;
        case '3':
            memcpy_P(&glyph, &alpha_glyphs[29], sizeof(glyph));
            break;
        case '4':
            memcpy_P(&glyph, &alpha_glyphs[30], sizeof(glyph));
            break;
        case '5':
            memcpy_P(&glyph, &alpha_glyphs[31], sizeof(glyph));
            break;
        case '6':
            memcpy_P(&glyph, &alpha_glyphs[32], sizeof(glyph));
            break;
        case '7':
            memcpy_P(&glyph, &alpha_glyphs[33], sizeof(glyph));
            break;
        case '8':
            memcpy_P(&glyph, &alpha_glyphs[34], sizeof(glyph));
            break;
        case '9':
            memcpy_P(&glyph, &alpha_glyphs[35], sizeof(glyph));
            break;
        case '0':
            memcpy_P(&glyph, &alpha_glyphs[36], sizeof(glyph));
            break;
    }

    // Write data to screen
    ssd1306_start_data();
    
    for(uint8_t j = 0; j < glyph.len ; ++j)
    {
        i2c_write_byte(pgm_read_byte(&(glyph.data[j])));
    }
    // Pad between letters
    i2c_write_byte(0x0);
    i2c_write_byte(0x0);
    

	ssd1306_stop();
}


void EEPROM_write(uint16_t ucAddress, uint8_t ucData)
{
    cli();
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE))
    ;
    /* Set Programming mode */
    EECR = (0<<EEPM1)|(0<<EEPM0);
    /* Set up address and data registers */
    EEAR = ucAddress;
    EEDR = ucData;
    /* Write logical one to EEMPE */
    EECR |= (1<<EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1<<EEPE);
    sei();
}

uint8_t EEPROM_read(uint16_t ucAddress)
{
    cli();
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE))
    ;
    /* Set up address register */
    EEAR = ucAddress;
    /* Start eeprom read by writing EERE */
    EECR |= (1<<EERE);
    /* Return data from data register */
    sei();
return EEDR;
}