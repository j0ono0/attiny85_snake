
#include "ssd1306.h"


// control byte:
// [Co][D/C]000000
// Co: 0 = data bytes only for remainder of transmission
// D/C: 0 = following byte is command, 1 = following byte is data
// 0xC0 = 0b0
void ssd1306_init() {
	ssd1306_start_cmd();
	for (int i = 0; i < 29; i++) {
		i2c_write_byte(pgm_read_byte(ssd1306_initialization_sequence + i)); // progmem access
	}
	i2c_stop();
}

///////////////////////////////////////////////////////////////////////////

void ssd1306_start_cmd()
{
	i2c_start();
	i2c_write_byte(SSD1306_ADDR_WRITE);
	i2c_write_byte(SSD1306_CONTROL_BYTE_CMD);
}

void ssd1306_start_data()
{
	i2c_start();
	i2c_write_byte(SSD1306_ADDR_WRITE);
	i2c_write_byte(SSD1306_CONTROL_BYTE_DATA);
}

void ssd1306_stop()
{
	i2c_stop();
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

///////////////////////////////////////////////////////////////////////////



void draw_img() {
	set_page_address(0,7);
	set_column_address(0, 127);
	ssd1306_send_progmem_data(img_hello_length, img_hello);
}