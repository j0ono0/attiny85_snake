
//https://hackaday.io/project/184848-storing-and-playing-back-lofi-audio-on-an-mcu/log/206218-pwm-on-attiny85

#include <util/delay.h> 
#include<avr/io.h>
#include "t85_pwm.h"

// #define F_CPU 16500000UL
#define FASTEST 0
void pwm_config()
{


    // TCCR0A |= (1 << WGM02) |(1 << WGM01) | (1 << WGM00); // Fast PWM
    // TCCR0A |= (1 << COM0B1) | (1 << COM0B0); // 

    // TCCR0B |= (1 << CS02) | (1 << CS00);
    // // TCCR0B |= (1 << CS01) ;

    // // OCR0A = 127;
    // OCR0B = 127;

//////////////////////////////////////////////////////////

    TCCR1 |= //(1 << CTC1) |      // Clear Timer/Counter on Compare Match
             (1 << PWM1A) |     // Pulse Width Modulator A Enable
             (1 << COM1A1) |    // OC1x cleared on compare match. Set when TCNT1 = $00.
             
             //(1 << CS13) | (1 << CS12) | (1 << CS11) | (1 << CS10); // Prescaler CK/16384
            // (1 << CS13) | (1 << CS12) | (1 << CS11) ; // CK/8192
            //  (1 << CS13) | (1 << CS12)  ; // CK/2048
            // (1 << CS13) | (1 << CS11) | (1 << CS10) ; // CK/1024
            //  (1 << CS13) | (1 << CS11)  ; // CK/512
            //  (1 << CS13); // CK/128
             (1 << CS12) | (1 << CS11)  ; // CK/32

    TCNT1 = 0x00;

    uint8_t tone_val = 240;


    // uint8_t tone_c = 239;
    // uint8_t tone_d = 213;
    // uint8_t tone_e = 190;
    // uint8_t tone_f = 179;
    // uint8_t tone_g = 159;
    // uint8_t tone_a = 142;
    // uint8_t tone_b = 127;
    // uint8_t tone_c1 =119;

    OCR1C  = tone_val;
    OCR1A = tone_val / 2;

}

void play_tone(uint8_t tone)
{
    
    OCR1C  = tone;
    OCR1A = tone / 2;
    DDRB |= (1<<PB1); // set PB1 as output to enable PWM output
    _delay_ms(200);
    DDRB &= ~(1<<PB1); // PB1 disable PWM output
    _delay_ms(25);
}