
#include <util/delay.h> 
#include <avr/pgmspace.h>
#include <avr/cpufunc.h>

#include "i2c_t85.h"

#define SDA PB0
#define SCL PB2

// Counter settings for USISR
// Note: counter increments on *both* rising and falling edges in i2c mode.
// Counter increments twice for each bit transferred.
#define USICTN_8_BIT_TRANSFER 0x0 
#define USICTN_1_BIT_TRANSFER  0xe 

#define TOGGLE_CLK() (USICR |= (1 << USITC))
#define CLEAR_USISR() (USISR = 0xF0)

void green_led_on()
{
	DDRB |= (1 << DDB3);
	PORTB |=  (1 << PB3);   // green LED on
}

	void sda_down()
	{
		PORTB &= ~(1 << SDA);
	}
    void sda_release()
	{
		PORTB |= (1 << SDA);
	}
    void scl_down()
	{
		PORTB &= ~(1 << SCL);
	}
    void scl_release()
	{
		PORTB |= (1 << SCL);
		// Wait for SCL. 
		// Slave may hold it low if busy (clock stretching)
		// Unneeded for ssd1306 interface (?)
		_NOP();
		while(!(PINB & (1 << SCL)));
	}

void i2c_init() {

	DDRB |= (1 << SDA);
	DDRB |= (1 << SCL);

	sda_release();
	scl_release();
	
    // Control regsiter 
	USICR = (1 << USIWM1) | // Wire Mode (Two-wire mode)
			(1 << USICS1) | // Clock Source Select
			(1 << USICLK);  // Clock Select Register


	// Status register - Clear flags and reset counter
	CLEAR_USISR();

	// Data register - Releases SDA?
	USIDR = 0xFF;

}

void i2c_start() {
	sda_down();
	scl_down();
	// Alert if start condition failed
    if (!(USISR & (1 << USISIF)))
		green_led_on();
	// Release SDA
	sda_release();
}

void i2c_stop() {
	scl_release();
	sda_release();
}

void i2c_transfer(uint8_t usictn_mask) {
	// Clear flags and set counter
	USISR = 0xF0 | usictn_mask;
	// Transfer data in USIDR
	while(!(USISR & (1 << USIOIF)))
	{
		TOGGLE_CLK();
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
