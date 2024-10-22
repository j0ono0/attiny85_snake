#ifndef i2c_t85_j0ono0_h
#define i2c_t85_j0ono0_h

    void green_led_on();
    void sda_down();
    void sda_release();
    void scl_down();
    void scl_release();
    void i2c_init();
    void i2c_start();
    void i2c_stop();
    void i2c_transfer(uint8_t usisr_mask);
    uint8_t i2c_read_ack();
    void i2c_write_byte(uint8_t data);

#endif