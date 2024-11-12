#include "audio.h"


// *******************************************

// TODO: user timer.c for tone durations

// *******************************************


const Tune *current_tune = NULL;

uint8_t audio_counter = 0;
int intr_count=0;



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




Note riff_lose_data[] = {{127, 10}, {159, 10}, {190, 10}, {239, 5}, {213, 5}, {239, 5}, {213, 5}, {239, 5}};
Note riff_rebound_top_data[] = {{142, 10}, {119, 5}, {142, 10}};
Note riff_rebound_bottom_data[] = {{119, 10}, {142, 5}, {119, 10}};

const Tune riff_lose = {riff_lose_data, sizeof(riff_lose_data)/sizeof(Note)};
const Tune riff_rebound_top = {riff_rebound_top_data, sizeof(riff_rebound_top_data)/sizeof(Note)};
const Tune riff_rebound_bottom = {riff_rebound_bottom_data, sizeof(riff_rebound_bottom_data)/sizeof(Note)};


void audio_config()
{

    // Setup PWM for playing tones (square wave) on PB1
    // ================================================


    TCCR1 |= (1 << PWM1A) |                 // Pulse Width Modulator A Enable
             (1 << COM1A1) |                // OC1x cleared on compare match. Set when TCNT1 = $00.
             (1 << CS12) | (1 << CS11)  ;   // CK/32

    TCNT1 = 0x00;

}


void play_tone(uint8_t tone)
{
    OCR1C  = tone;
    OCR1A = tone / 2;
    DDRB |= (1<<PB1);                       // set PB1 as output to enable PWM output
}

void stop_tone()
{
    DDRB &= ~(1<<PB1);                      // PB1 disable PWM output
}

void start_tune(const Tune *riff)
{
    current_tune = riff;
    audio_counter = 0;
    intr_count = 0;
    sei();
}

void stop_tune()
{
     current_tune = NULL;
        stop_tone();
        cli();
}

bool update_audio()
{

    if(!current_tune)
    {
        return false;
    }

    //40 count = 1 sec
    if (intr_count >= current_tune->notes[audio_counter].dur) 
    {
        ++audio_counter;
        intr_count=0;
    }

    if(audio_counter >= current_tune->len)
    {
       stop_tune();
        return false;
    }

    // Update note being played
    play_tone(current_tune->notes[audio_counter].freq);
    
    return true;
}
