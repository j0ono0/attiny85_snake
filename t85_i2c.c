

#include "t85_i2c.h"



#define PIN_SDA PB0
#define PIN_SCL PB2

#define WAIT_LONG 0 // 5 // 4,7us (some fine tunning can be made here...)
#define WAIT_SHORT 0 // 4 // 4,0us

// USISR mask
#define USISR_CLOCK_8_BITS		0b11110000
#define USISR_CLOCK_1_BIT  		0b11111110


// Counter settings for USISR
// Note: counter increments on *both* rising and falling edges in i2c mode.
// Counter increments twice for each bit transferred.
#define USICTN_8_BIT_TRANSFER 0x0 
#define USICTN_1_BIT_TRANSFER  0xe 

#define TOGGLE_CLK() (USICR |= (1 << USITC))
#define SDA_LOW() (PORTB &= ~(1 << PIN_SDA))
#define SCL_LOW() (PORTB &= ~(1 << PIN_SCL))
// Note: "release" drives the pin high.
#define SDA_RELEASE() (PORTB |= (1 << PIN_SDA))
#define SCL_RELEASE() (PORTB |= (1 << PIN_SCL))


void i2c_init() {

    DDRB |= (1 << PIN_SDA);
    DDRB |= (1 << PIN_SCL);

    SCL_RELEASE();
    SDA_RELEASE();

    USIDR = 0xFF;

    // Set TWI mode
    USICR = (1 << USIWM1) | (1 << USICS1) | (1 << USICLK);
    // Clear flags
    USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF); 


}

void i2c_start() {

    // generate start condition    
    SDA_RELEASE();
    SCL_RELEASE();
    while (!(PORTB & (1<<PIN_SCL)));

    SDA_LOW();

    // _delay_us(WAIT_LONG);

    SCL_LOW();
    SDA_RELEASE();

}

void i2c_stop() {

    // Generate end condition
    SDA_LOW();
    SCL_RELEASE();
    while (!(PORTB & (1<<PIN_SCL)));

    // _delay_us(WAIT_LONG);

    // release SDA
    SDA_RELEASE();
    
    // _delay_us(WAIT_SHORT);

}

uint8_t i2c_transfer(uint8_t usisr_mask) {

    // Ensure SCL is low
    SCL_LOW();

    USISR = usisr_mask;

	// Transfer data in USIDR
	while(!(USISR & (1 << USIOIF)))
	{
		TOGGLE_CLK();
		_delay_us(1);
		TOGGLE_CLK();
		_delay_us(1);
	}

    uint8_t data_buffer = USIBR;

    // release SDA
    USIDR = 0xFF; // 0x80 will work too (bit 7)

    return data_buffer; // previous USIDR copy

}

// controller sends a byte to the bus
// returns 0 if there's a valid nack, otherwise 1
uint8_t i2c_write_byte(uint8_t data) {
    USIDR = data;
    i2c_transfer(USISR_CLOCK_8_BITS);

    return i2c_read_ack();

}

uint8_t i2c_read_ack()
{
    // wait for ack
    DDRB &= ~(1<< PIN_SDA); // to input
    uint8_t ack = i2c_transfer(USISR_CLOCK_1_BIT);
    DDRB |= (1 << PIN_SDA);
    return ack;
}
