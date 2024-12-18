#ifndef i2c_t85_j0ono0_h
#define i2c_t85_j0ono0_h

    #include <avr/io.h>
    #include <util/delay.h> 


    void i2c_init();
    void i2c_start();
    void i2c_stop();
    uint8_t i2c_transfer(uint8_t usisr_mask);
    uint8_t i2c_write_byte(uint8_t data);
    uint8_t i2c_read_ack();
    uint8_t i2c_read_byte(uint8_t nack);
    
#endif