
//https://hackaday.io/project/184848-storing-and-playing-back-lofi-audio-on-an-mcu/log/206218-pwm-on-attiny85

#include <util/delay.h> 
#include<avr/io.h>
#include "t85_pwm.h"

// #define F_CPU 16500000UL
#define FASTEST 0
void pwm_config()
{


//////////////////////////////////////////////////////////

    TCCR1 |= (1 << PWM1A) |                 // Pulse Width Modulator A Enable
             (1 << COM1A1) |                // OC1x cleared on compare match. Set when TCNT1 = $00.
             (1 << CS12) | (1 << CS11)  ;   // CK/32

    TCNT1 = 0x00;



    //5th octave
    // uint8_t tone_c5 = 239;
    // uint8_t tone_d5 = 213;
    // uint8_t tone_e5 = 190;
    // uint8_t tone_f5 = 179;
    // uint8_t tone_g5 = 159;
    // uint8_t tone_a5 = 142;
    // uint8_t tone_b5 = 127;
    // 6th octave
    // uint8_t tone_c6 =119;

    uint8_t tone_val = 240;
    OCR1C  = tone_val;
    OCR1A = tone_val / 2;

}

void play_tone(uint8_t tone)
{
    OCR1C  = tone;
    OCR1A = tone / 2;
    DDRB |= (1<<PB1); // set PB1 as output to enable PWM output
}

void stop_tone()
{
    DDRB &= ~(1<<PB1); // PB1 disable PWM output
}