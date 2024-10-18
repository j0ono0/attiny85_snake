#ifndef i2c_t85_j0ono0_h
#define i2c_t85_j0ono0_h

    void i2c_init();
    void i2c_start();
    void i2c_stop();
    uint8_t i2c_transfer(unsigned char usisr_mask);
    uint8_t i2c_read_ack();
    void i2c_write_byte(unsigned char data);
    unsigned char i2c_read_byte(unsigned char nack);


#endif