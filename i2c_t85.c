

#include <avr/pgmspace.h>
#include "i2c_t85.h"


void i2c_init() {
	DDRB |= (1 << PIN_SDA);
	DDRB |= (1 << PIN_SCL);

	PORTB |= (1 << PIN_SCL);
	PORTB |= (1 << PIN_SDA);

	USIDR = 0x80;

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
	PORTB |= (1<<PIN_SDA); // sda high (release to start transmitting)
}

void i2c_stop() {

	// SDA goes low
	PORTB &= ~(1<<PIN_SDA);

	// release SCL
	PORTB |= (1<<PIN_SCL);
	while (! (PINB & (1<<PIN_SCL)));

	_delay_us(WAIT_LONG);

	// release SDA
	PORTB |= (1<<PIN_SDA);
	
	_delay_us(WAIT_SHORT);

}

uint8_t i2c_transfer(unsigned char usisr_mask) {

	// force SDL low (it's already low probably, since we're
	// toggling it up and down in pairs, but just in case...)
	PORTB &= ~(1<<PIN_SCL);

	USISR = usisr_mask;

	// transfer until counter overflow
	// do {
	// 	// _delay_us(WAIT_LONG);
	// 	USICR |= (1 << USITC);
	// 	while (! (PINB & (1<<PIN_SCL))); //Waiting for SCL to go high
	// 	// _delay_us(WAIT_SHORT);
	// 	USICR |= (1 << USITC);
	// } while (!(USISR & (1 << USIOIF)));
	// // _delay_us(WAIT_LONG);

	while(!(USISR & (1 << USIOIF)))
	{
		USICR |= (1 << USITC);
	}

	uint8_t response_data = USIBR;

	// release SDA
	// "The output pin (DO or SDA, depending on the wire mode)
	// is connected via the output latch to the most significant
	// bit (bit 7) of the USI Data Register."
	// (so write 1 to set SDA high before the next operation,
	// otherwise it'll be pulled down and read may not work)
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
void i2c_write_byte(unsigned char data) {
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
unsigned char i2c_read_byte(unsigned char nack) {

	DDRB &= ~(1<< PIN_SDA); // reciving, so change to input
	unsigned char data = i2c_transfer(USISR_CLOCK_8_BITS);
	DDRB |= (1 << PIN_SDA);

	// send nack
	USIDR = nack;
	i2c_transfer(USISR_CLOCK_1_BIT);

	return data;
}