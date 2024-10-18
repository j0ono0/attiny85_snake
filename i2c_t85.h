#ifndef i2c_t85_j0ono0_h
#define i2c_t85_j0ono0_h

    #include <util/delay.h> 

    #define PIN_SDA 				PB0
    #define PIN_SCL 				PB2

    #define WAIT_LONG				5 // 4,7us
    #define WAIT_SHORT 				4 // 4,0us

    // USISR mask
    #define USISR_CLOCK_8_BITS		0b11110000
    #define USISR_CLOCK_1_BIT  		0b11111110


    void i2c_init();
    void i2c_start();
    void i2c_stop();
    uint8_t i2c_transfer(unsigned char usisr_mask);
    uint8_t i2c_read_ack();
    void i2c_write_byte(unsigned char data);
    unsigned char i2c_read_byte(unsigned char nack);


#endif