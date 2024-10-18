/*
	Raw library for ssd1306 oled-like small screens. This one
	handles	single and multiple command transactions, even with data
	from PROGMEM (mostly used for displaying imaged stored there).

	I made this for my Game of Life on an attiny85 project:
	https://github.com/theisolinearchip/gameoflife_attiny85
	https://hackaday.io/project/181421-game-of-life-on-an-attiny85

	It's the same version but without some extra methods for a small
	matrix system I made in order to display the dynamic game with
	the small SRAM available on the attiny85 (not really relevant
	for the i2c part)

	It was intended to be used with a bit-banging implementation
	for the i2c protocol I made too (same functions, different code;
	changing the base-lib doesn't matter: it also works fine! :D)
*/

#ifndef SSD1306_attiny85_c
#define SSD1306_attiny85_c

#include "ssd1306.h"


static const uint8_t PROGMEM ssd1306_initData[] =
{
    SSD1306_DISPLAYOFF, // display off
    SSD1306_MEMORYMODE, HORIZONTAL_ADDRESSING_MODE, // Page Addressing mode
    SSD1306_COMSCANDEC,             // Scan from 127 to 0 (Reverse scan)
    SSD1306_SETSTARTLINE | 0x00,    // First line to start scanning from
    SSD1306_SETCONTRAST, 0x7F,      // contast value to 0x7F according to datasheet
    SSD1306_SEGREMAP | 0x01,        // Use reverse mapping. 0x00 - is normal mapping
    SSD1306_NORMALDISPLAY,
    SSD1306_SETMULTIPLEX, 63,       // Reset to default MUX. See datasheet
    SSD1306_SETDISPLAYOFFSET, 0x00, // no offset
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,// set to default ratio/osc frequency
    SSD1306_SETPRECHARGE, 0x22,     // switch precharge to 0x22 // 0xF1
    SSD1306_SETCOMPINS, 0x12,       // set divide ratio
    SSD1306_SETVCOMDETECT, 0x20,    // vcom deselect to 0x20 // 0x40
    SSD1306_CHARGEPUMP, 0x14,       // Enable charge pump
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_DISPLAYON
};


void ssd1306_init() {
	ssd1306_start();
	// Accessing prog mem - hence not using ssd1306_send_commands()
	i2c_write_byte(SSD1306_CONTROL_BYTE_CMD);
	for (int i = 0; i < sizeof(ssd1306_initData); i++) {
		i2c_write_byte(pgm_read_byte(&ssd1306_initData[i]));
	}
	i2c_stop();
}

void ssd1306_start()
{
	i2c_start();
	i2c_write_byte(SSD1306_ADDR_WRITE);
}

void ssd1306_stop()
{
	i2c_stop();
}

void ssd1306_send_commands(int length, const uint8_t commands[]) {
	ssd1306_start();
	i2c_write_byte(SSD1306_CONTROL_BYTE_CMD);
	for (int i = 0; i < length; i++) {
		i2c_write_byte(commands[i]);
	}
	ssd1306_stop();
}


void ssd1306_send_data(int length, const uint8_t data[]) {
	ssd1306_start();
	i2c_write_byte(SSD1306_CONTROL_BYTE_DATA);
	for (int i = 0; i < length; i++) {
		i2c_write_byte(data[i]);
	}
	ssd1306_stop();
}


#endif