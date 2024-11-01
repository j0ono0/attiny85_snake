#ifndef at85_PWM_J0ONO0_h
#define AT85_PWM_J0ONO0_h

    #include <util/delay.h> 
    #include<avr/io.h>

    void pwm_config();
    void play_tone(uint8_t tone);
    void stop_tone();

#endif