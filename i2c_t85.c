
#include <util/delay.h> 
#include <avr/pgmspace.h>
#include <avr/cpufunc.h>

#include "i2c_t85.h"

#define PIN_SDA PB0
#define PIN_SCL PB2

#define WAIT_LONG  5 
#define WAIT_SHORT 4 

// Counter settings for USISR
// Note: counter increments on *both* rising and falling edges in i2c mode.
// Counts twice for each bit transferred.
#define USICTN_8_BIT_TRANSFER 0x0 
#define USICTN_1_BIT_TRANSFER  0xe 


void i2c_init() {

    // Control regsiter 
	USICR = (1 << USIWM1) | // Wire Mode (Two-wire mode)
			(1 << USICS1) | // Clock Source Select
			(1 << USICLK);  // Clock Strobe

	USISR = (1 << USISIF)| // Start Condition Interrupt Flag
			(1 << USIOIF)| // Counter Overflow Interrupt Flag
			(1 << USIPF)|  // Stop Condition Flag
			(1 << USIDC);  // Data Output Collision

	USISR &= ~0xF; // Set counter to 0 

	DDRB |= (1 << PIN_SDA);
	DDRB |= (1 << PIN_SCL);

	PORTB |= (1 << PIN_SCL);
	PORTB |= (1 << PIN_SDA);

	// SDA and SCL are both high and ready for i2c start
}

void i2c_start() {
		
	PORTB &= ~(1<<PIN_SDA); // sda low
	PORTB &= ~(1<<PIN_SCL); // scl low
	
	// !!! this is NEEDED !!! BUT WHY, where is the documentation for it ???
	PORTB |= (1<<PIN_SDA); 
}

void i2c_stop() {

	// Not needed ?! but no gaurantee sda is low after ack ???
	// PORTB &= ~(1<<PIN_SDA); // sda low


	PORTB |= (1<<PIN_SCL);
	PORTB |= (1<<PIN_SDA);
}

void i2c_transfer(uint8_t usictn_mask) {
	// Set counter
	USISR |= usictn_mask;
	while(!(USISR & (1 << USIOIF)))
	{
		USICR |= (1 << USITC);
	}
	// Clear USISR flags and reset counter
	USISR = 0xF0;

}

uint8_t i2c_read_ack()
{
	// Ensure PIN_SDA isn't set low by master when PIN_SCL is toggled
	USIDR = 0xFF;
	
	PORTB |= (1<<PIN_SCL);	// Set clock high
	// _NOP();
	uint8_t ack = PINB & (1<<PIN_SDA);
	PORTB &= ~(1<<PIN_SCL); // Set clock low

	if(ack != 0)
	{
		DDRB |= (1<<PB4);
    	PORTB |= (1<<PB4);
	}

	return ack;

}

void i2c_write_byte(uint8_t data) {
	USIDR = data;
	i2c_transfer(USICTN_8_BIT_TRANSFER);
	// Read acknowledgement from slave
	i2c_read_ack();
}
