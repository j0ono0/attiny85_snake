/*
	ssd1306 library.
*/

#ifndef SSD1306_attiny85_c
#define SSD1306_attiny85_c

#include "ssd1306.h"


static const uint8_t PROGMEM ssd1306_initData[] =
{
    SSD1306_DISPLAYOFF, 			// Display off
    SSD1306_MEMORYMODE, 0x00, 		// Page Addressing mode
    SSD1306_COMSCANDEC,             // Scan from 127 to 0 (Reverse scan)
    SSD1306_SETSTARTLINE | 0x00,    // First line to start scanning from
    SSD1306_SETCONTRAST, 0x7F,      // Contast value to 0x7F according to datasheet
    SSD1306_SEGREMAP | 0x01,        // Use reverse mapping. 0x00 - is normal mapping
    SSD1306_NORMALDISPLAY,			// Normal orientation (ie not inversed)
    SSD1306_SETMULTIPLEX, 63,       // Reset to default MUX. See datasheet
    SSD1306_SETDISPLAYOFFSET, 0x00, // No offset
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,// Set to default ratio/osc frequency
    SSD1306_SETPRECHARGE, 0x22,     // Switch precharge to 0x22 // 0xF1
    SSD1306_SETCOMPINS, 0x12,       // Set divide ratio
    SSD1306_SETVCOMDETECT, 0x20,    // Vcom deselect to 0x20 // 0x40
    SSD1306_CHARGEPUMP, 0x14,       // Enable charge pump
    SSD1306_DISPLAYALLON_RESUME,	// Display from memory
    SSD1306_DISPLAYON				// Display on
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

void ssd1306_start_data()
{
	ssd1306_start();
	i2c_write_byte(SSD1306_CONTROL_BYTE_DATA);
}

void ssd1306_start_cmd()
{
	ssd1306_start();
	i2c_write_byte(SSD1306_CONTROL_BYTE_CMD);
}

void ssd1306_stop()
{
	i2c_stop();
}

void ssd1306_send_commands(int length, const uint8_t commands[]) {
	void ssd1306_start_cmd();
	for (int i = 0; i < length; i++) {
		i2c_write_byte(commands[i]);
	}
	ssd1306_stop();
}


void ssd1306_send_data(int length, const uint8_t data[]) {
	ssd1306_start_data();
	for (int i = 0; i < length; i++) {
		i2c_write_byte(data[i]);
	}
	ssd1306_stop();
}

// ATTN: Reads from prog memory
void ssd1306_send_progmem_data(int length, const uint8_t data[]) {
	ssd1306_start_data();
	for (int i = 0; i < length; i++) {
		i2c_write_byte(pgm_read_byte(&data[i]));
	}
	ssd1306_stop();
}

void clear_screen()
{
	set_column_address(0, 127);
	set_page_address(0, 7);
	ssd1306_start_data();
	for (int i = 0; i < 1024; i++) {
		i2c_write_byte(0x00);
	}
	ssd1306_stop();
}

void set_page_address(uint8_t start, uint8_t  end)
{
	// Start and end must be between 0-7 inclusive
	ssd1306_start_cmd();
	i2c_write_byte(0x22); 	// Set page address
	i2c_write_byte(start);		// Start
	i2c_write_byte(end);		// End
	ssd1306_stop();
}

void set_column_address(uint8_t start, uint8_t  end)
{
	// Start and end must be between 0-127 inclusive
	ssd1306_start_cmd();
	i2c_write_byte(0x21); 	// Set column address
	i2c_write_byte(start);		// Start
	i2c_write_byte(end);	// End
	ssd1306_stop();
}

void draw_lines()
{
	set_column_address(64, 127);
	set_page_address(3, 3);

	ssd1306_start_data();
	i2c_write_byte(0xFF);
	i2c_write_byte(0x00);
	i2c_write_byte(0xFF);
	i2c_write_byte(0x00);
	i2c_write_byte(0xFF);
	ssd1306_stop();

}


#endif