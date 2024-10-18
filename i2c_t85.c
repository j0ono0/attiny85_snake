
#include <util/delay.h> 
#include <avr/pgmspace.h>

#include "i2c_t85.h"

#define PIN_SDA PB0
#define PIN_SCL PB2

#define WAIT_LONG  5 
#define WAIT_SHORT 4 

// Counter settings for USISR
// Note: counter increments on *both* rising and falling edges in i2c mode.
// So counts twice for each bit transferred.
#define USISR_8_BIT_TRANSFER 0b11110000
#define USISR_1_BIT_TRANSFER  0b11111110


void i2c_init() {
	DDRB |= (1 << PIN_SDA);
	DDRB |= (1 << PIN_SCL);

	PORTB |= (1 << PIN_SCL);
	PORTB |= (1 << PIN_SDA);

	USISR = (1 << USISIF)| // Start Condition Interrupt Flag
			(1 << USIOIF)| // Counter Overflow Interrupt Flag
			(1 << USIPF)|  // Stop Condition Flag
			(1 << USIDC);  // Data Output Collision

    // Control regsiter 
	USICR = (1 << USIWM1) | // Wire Mode (Two-wire mode)
			(1 << USICS1) | // Clock Source Select
			(1 << USICLK);  // Clock Strobe
}

void i2c_start() {
	PORTB &= ~(1<<PIN_SDA); // sda low (start condition)
	_delay_us(WAIT_LONG);
	PORTB &= ~(1<<PIN_SCL); // scl low	
	
	// !!! this is NEEDED !!! BUT WHY, where is the documentation for it ???
	PORTB |= (1<<PIN_SDA); // sda high (release to start transmitting)
}

void i2c_stop() {
	PORTB |= (1<<PIN_SCL);
	// Wait until PIN_SCL is high
	while (!PIN_SCL){};
	_delay_us(WAIT_LONG);
	PORTB |= (1<<PIN_SDA);
}

uint8_t i2c_transfer(uint8_t usisr_val) {

	// TODO: change usisr_val to counter value?
	USISR = usisr_val;

	while(!(USISR & (1 << USIOIF)))
	{
		USICR |= (1 << USITC);
	}

	uint8_t response_data = USIBR;

	USIDR = 0xFF; // 0x80 will work too (bit 7)

	// Reset counter overflow flag
	// !!! Not needed ??? !!!
	// USISR |= (1 << USIOIF);

	return response_data; // previous USIDR copy

}

uint8_t i2c_read_ack()
{
	// Set SDA to input
	DDRB &= ~(1<< PIN_SDA);
	uint8_t ack = i2c_transfer(USISR_1_BIT_TRANSFER);
	DDRB |= (1 << PIN_SDA);
	return ack;
}

// controller sends a byte to the bus
// returns 0 if there's a valid nack, otherwise 1
void i2c_write_byte(uint8_t data) {
	USIDR = data;
	i2c_transfer(USISR_8_BIT_TRANSFER);

	// Read acknowledgement from slave
	i2c_read_ack();

}
