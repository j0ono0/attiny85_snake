/*
	ssd1306 library.
*/

#ifndef SSD1306_attiny85_c
#define SSD1306_attiny85_c

#include "ssd1306.h"

const uint8_t ssd1306_init_sequence [] PROGMEM = {	// Initialization Sequence

	0xAE,			// Set Display ON/OFF - AE=OFF, AF=ON
	0xD5, 0xF0,		// Set display clock divide ratio/oscillator frequency, set divide ratio
	0xA8, 0x3F,		// Set multiplex ratio (1 to 64) ... (height - 1)
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0x40 | 0x00,	// Set start line address, at 0.
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0x20, 0x00,		// Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, 10=Page, 11=Invalid
	0xA0 | 0x01,	// Set Segment Re-map
	0xC8,			// Set COM Output Scan Direction
	0xDA, 0x12,		// Set COM Pins Hardware Configuration - 128x32:0x02, 128x64:0x12
	0x81, 0x3F,		// Set contrast control register
	0xD9, 0x22,		// Set pre-charge period (0x22 or 0xF1)
	0xDB, 0x20,		// Set Vcomh Deselect Level - 0x00: 0.65 x VCC, 0x20: 0.77 x VCC (RESET), 0x30: 0.83 x VCC
	0xA4,			// Entire Display ON (resume) - output RAM to display
	0xA6,			// Set Normal/Inverse Display mode. A6=Normal; A7=Inverse
	0x2E,			// Deactivate Scroll command
	0xAF,			// Set Display ON/OFF - AE=OFF, AF=ON
	//
	0x22, 0x00, 0x3f,	// Set Page Address (start,end)
	0x21, 0x00,	0x7f,	// Set Column Address (start,end)
	//
	// 0xD6, 0x01,		// Set Zoom In, 0=disabled, 1=enabled
/*
	0xAE,			// Display OFF (sleep mode)
	0x20, 0x00,		// Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, , 10=Page, 11=Invalid
	0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
	0xC8,			// Set COM Output Scan Direction
	0x00,			// ---set low column address
	0x10,			// ---set high column address
	0x40,			// --set start line address
	0x81, 0x3F,		// Set contrast control register
	0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
	0xA6,			// Set display mode. A6=Normal; A7=Inverse
	0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
	0xA4,			// Output RAM to Display - 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0xD5, 0xF0,		// --set display clock divide ratio/oscillator frequency
	0xD9, 0x22,		// Set pre-charge period
	0xDA, 0x12,		// Set com pins hardware configuration
	0xDB, 0x20,		// --set vcomh, 0x20,0.77xVcc
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0xAF,			// Display ON in normal mode
*/
};


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
		i2c_write_byte(pgm_read_byte(&ssd1306_init_sequence[i]));
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