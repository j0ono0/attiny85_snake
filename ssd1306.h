#ifndef SSD1306_attiny85_constants_h
#define SSD1306_attiny85_constants_h

#include <avr/pgmspace.h>
#include "t85_i2c.h"


#define SSD1306_ADDR_WRITE 0x78

#define SSD1306_CONTROL_BYTE_CMD  0x00
#define SSD1306_CONTROL_BYTE_DATA 0x40


#define SSD1306_SETLOWCOLUMN      	0x00
#define SSD1306_SETHIGHCOLUMN     	0x10
#define SSD1306_MEMORYMODE        	0x20
#define SSD1306_COLUMNADDR        	0x21
#define SSD1306_PAGEADDR          	0x22
#define SSD1306_SETSTARTLINE      	0x40
#define SSD1306_DEFAULT_ADDRESS   	0x78
#define SSD1306_SETCONTRAST       	0x81
#define SSD1306_CHARGEPUMP        	0x8D
#define SSD1306_SEGREMAP          	0xA0
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON      	0xA5
#define SSD1306_NORMALDISPLAY     	0xA6
#define SSD1306_INVERTDISPLAY     	0xA7
#define SSD1306_SETMULTIPLEX      	0xA8
#define SSD1306_DISPLAYOFF        	0xAE
#define SSD1306_DISPLAYON         	0xAF
#define SSD1306_SETPAGE           	0xB0
#define SSD1306_COMSCANINC        	0xC0
#define SSD1306_COMSCANDEC        	0xC8
#define SSD1306_SETDISPLAYOFFSET  	0xD3
#define SSD1306_SETDISPLAYCLOCKDIV 	0xD5
#define SSD1306_SETPRECHARGE      	0xD9
#define SSD1306_SETCOMPINS        	0xDA
#define SSD1306_SETVCOMDETECT    	0xDB
#define SSD1306_SWITCHCAPVCC     	0x02
#define SSD1306_NOP              	0xE3


void ssd1306_init();
void ssd1306_start();
void ssd1306_stop();
void ssd1306_send_commands(int length, const uint8_t commands[]);
void ssd1306_send_data(int length, const uint8_t data[]);
void ssd1306_send_progmem_data(int length, const uint8_t data[]);

void clear_screen();
void set_page_address(uint8_t start, uint8_t  end);
void set_column_address(uint8_t start, uint8_t  end);
void draw_lines();


const static int default_image_length = 1024;

const static uint8_t PROGMEM image_2[] = {
0xe0, 0xe0, 0xe0, 0xe0, 0xf0, 0xf8, 0xff, 0x7f, 0x3f, 0x1f, 0x8f, 0xc7, 0xc7, 0xcf, 0xcf, 0xcf, 0x8f, 0x8f, 0x8f, 0x8f, 0x9f, 0x1f, 0x3f, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf0, 0xe0, 0xe0, 0xe0, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3, 0x0, 0x0, 0x0, 0x1, 0x3, 0x3, 0x3, 0x3, 0x1, 0x0, 0x0, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0x0, 0x0, 0x3, 0x7, 0xf, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x8f, 0x87, 
0x83, 0x1, 0x78, 0xfc, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xfd, 0xfb, 0xf7, 0xe7, 0x7, 0xf, 0x1f, 0x1f, 0x1f, 0x1f, 0x9f, 0x9f, 0x9f, 0xbf, 0x7f, 0x7f, 0x7f, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x3, 0xc1, 0xe0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xf8, 0xf0, 0xe0, 0xc1, 0x3, 0x7, 0x1f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 
0x7, 0xf, 0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x1e, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xc0, 0x0, 0x0, 0xc0, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x3, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xf3, 0x87, 0xf, 0xf, 0xf, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3e, 0x1e, 0x1e, 0x1e, 0x1e, 0x3e, 0x7e, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xf8, 0xf0, 0xf0, 0xf0, 0xf0, 0xf8, 
0xf8, 0xfc, 0xfc, 0x78, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0xe0, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0x81, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xe0, 0xc0, 0xc0, 0x40, 0x40, 0x20, 0x10, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x3, 0xf, 0x1f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x80, 0xc0, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0xf, 0x0, 0xf0, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x1, 0x3, 0x7, 0x7, 0x7, 0x7, 0xf, 0x8f, 0x8f, 0x8f, 0x87, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 
0x8f, 0x8f, 0x87, 0x87, 0xe3, 0xf0, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf9, 0xf1, 0xf1, 0xf1, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xc0, 0x0, 0x7, 0x1f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xf8, 0xf8, 0xf8, 0xfc, 0x7e, 0x3d, 0x3, 0x7, 0x7, 0x87, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xe3, 0xe1, 0xe1, 0xe1, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf0, 0xe0, 0xc0, 0x87, 0x8f, 0x9f, 0xbf, 0xbf, 0xbf, 0xbf, 0xff, 0xff, 0xff, 0xbf, 0x87, 0x87, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x8f, 0x9f, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0x9f, 0x8f, 0x87, 0x80, 0xc0, 0xe0, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x3, 0x3, 0x3, 0x7, 0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf, 0x7, 0x7, 0x7, 
};

#endif