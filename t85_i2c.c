
/*

Simple i2c master for ATTiny85.
Designed to work with ssd1603 oled screen

*/

#include "t85_i2c.h"

#define SDA PB0
#define SCL PB2

// Counter settings for USISR
// Note: counter increments on *both* rising and falling edges in i2c mode.
// Counter increments twice for each bit transferred.
#define USICTN_8_BIT_TRANSFER 0x0 
#define USICTN_1_BIT_TRANSFER  0xe 

#define TOGGLE_CLK() (USICR |= (1 << USITC))
#define CLEAR_USISR() (USISR = 0xF0)
#define SDA_DOWN() (PORTB &= ~(1 << SDA))
#define SDA_RELEASE() (PORTB |= (1 << SDA))
#define SCL_DOWN() (PORTB &= ~(1 << SCL))
#define SCL_RELEASE() (PORTB |= (1 << SCL))



void i2c_init() {

	DDRB |= (1 << SDA);
	DDRB |= (1 << SCL);

	SCL_RELEASE();
	SDA_RELEASE();
	
    // Control regsiter 
	USICR = (1 << USIWM1) | // Wire Mode (Two-wire mode)
			(1 << USICS1) | // Clock Source Select
			(1 << USICLK);  // Clock Select Register

	// Status register - Clear flags and reset counter
	CLEAR_USISR();

	// Data register - Releases SDA?
	USIDR = 0xFF;
	SDA_RELEASE();
}

void i2c_start() {
	SCL_DOWN();
	SDA_DOWN();

    // if (!(USISR & (1 << USISIF)))
	// {
	// // Alert if start condition not detected
	// }


	// Release SDA
	SDA_RELEASE();
}

void i2c_stop() {
	SCL_RELEASE();
	SDA_RELEASE();
}

void i2c_transfer(uint8_t usictn_mask) {
	// Clear flags and set counter
	USISR = 0xF0 | usictn_mask;
	// Transfer data in USIDR
	while(!(USISR & (1 << USIOIF)))
	{
		TOGGLE_CLK();
		_delay_us(1);
		TOGGLE_CLK();
		_delay_us(1);
	}
}

uint8_t i2c_read_ack()
{
	// SDA as input
	DDRB &= ~(1 << SDA);
	USIDR = 0x0;
	i2c_transfer(USICTN_1_BIT_TRANSFER);
	DDRB |= (1 << SDA);
	// SDA as output
	uint8_t ack = USIBR;
	USIDR = 0xFF;
	// Convert successful ack as '1' rather than '0'
	return !ack;
}

void i2c_write_byte(uint8_t data) {
	USIDR = data;
	i2c_transfer(USICTN_8_BIT_TRANSFER);
	// Read acknowledgement from slave
	if(!i2c_read_ack())
	{
		DDRB |= (1<<PB4);
    	PORTB |= (1<<PB4);
	}
}