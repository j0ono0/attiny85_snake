
#include <util/delay.h> 
#include <avr/pgmspace.h>

#include "i2c_t85.h"

#define PIN_SDA PB0
#define PIN_SCL PB2

#define WAIT_LONG  5 
#define WAIT_SHORT 4 

// USISR mask
#define USISR_CLOCK_8_BITS 0b11110000
#define USISR_CLOCK_1_BIT  0b11111110


void i2c_init() {
	DDRB |= (1 << PIN_SDA);
	DDRB |= (1 << PIN_SCL);

	PORTB |= (1 << PIN_SCL);
	PORTB |= (1 << PIN_SDA);

	USISR = (1 << USISIF)| // Start Condition Interrupt Flag
			(1 << USIOIF)| //  Counter Overflow Interrupt Flag
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
	
	// !!! this is NEEDED !!! BUT WHY ???
	PORTB |= (1<<PIN_SDA); // sda high (release to start transmitting)
}

void i2c_stop() {
	PORTB |= (1<<PIN_SCL);
	// Wait until PIN_SCL is high
	while (!PIN_SCL){};
	_delay_us(WAIT_LONG);
	PORTB |= (1<<PIN_SDA);
}

uint8_t i2c_transfer(uint8_t usisr_mask) {

	// force SDL low (it's already low probably, since we're
	// toggling it up and down in pairs, but just in case...)
	PORTB &= ~(1<<PIN_SCL);

	USISR = usisr_mask;

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
	uint8_t ack = i2c_transfer(USISR_CLOCK_1_BIT);
	DDRB |= (1 << PIN_SDA);
	return ack;
}

// controller sends a byte to the bus
// returns 0 if there's a valid nack, otherwise 1
void i2c_write_byte(uint8_t data) {
	USIDR = data;
	i2c_transfer(USISR_CLOCK_8_BITS);

	// wait for ack
	// DDRB &= ~(1<< PIN_SDA); // to input
	// i2c_transfer(USISR_CLOCK_1_BIT);
	// DDRB |= (1 << PIN_SDA);

	// Read acknologement from slave
	if (i2c_read_ack())
	{
		DDRB |= (1 << DDB3);
		PORTB |=  (1 << PB3);
	}

}

// controller reads 1 byte from the bus
// and sends a nack if wanna read another one
// (1 = will read another one, 0 = stop sending)
// returns the read byte
uint8_t i2c_read_byte(uint8_t nack) {

	DDRB &= ~(1<< PIN_SDA); // reciving, so change to input
	uint8_t data = i2c_transfer(USISR_CLOCK_8_BITS);
	DDRB |= (1 << PIN_SDA);

	// send nack
	USIDR = nack;
	i2c_transfer(USISR_CLOCK_1_BIT);

	return data;
}