#include "audio.h"


// *******************************************

// TODO: user timer.c for tone durations

// *******************************************


const Tune *current_tune = NULL;

uint8_t audio_counter = 0;
uint64_t duration_start = 0;



//5th octave
// uint8_t tone_c5 = 239;
// uint8_t tone_c5# = 225;
// uint8_t tone_d5 = 213;
// uint8_t tone_d5# = 201;
// uint8_t tone_e5 = 190;
// uint8_t tone_f5 = 179;
// uint8_t tone_f5# = 169;
// uint8_t tone_g5 = 159;
// uint8_t tone_g5# = 150;
// uint8_t tone_a5 = 142;
// uint8_t tone_a5# = 134;
// uint8_t tone_b5 = 127;
// 6th octave
// uint8_t tone_c6 =119;


Note riff_lose_data[] = {{127, 12}, {159, 12}, {190, 4}, {239, 4}, {213, 4}, {239, 4}, {213, 4}, {239, 8}};

Note riff_win_big_data[] = {
    {239, 4}, {213, 4}, {239, 4}, {213, 4}, {239, 4}, {213, 4}, {239, 4}, {213, 4}, 
    {190, 4}, {179, 4}, {190, 4}, {179, 4}, {190, 4}, {179, 4}, {190, 4}, {179, 4}, 
    {159, 4}, {142, 4}, {159, 4}, {142, 4}, {159, 4}, {142, 4}, {159,4}, {142,4}, {119, 16}, {142, 4}, {119,16}};

// Note riff_win_small_data[] = {{119, 12}, {142, 4},{159, 8}, {179, 8}, {159, 8}, {179, 8}, {127, 8}, {142, 8}, {127, 8}, {142, 8}, {119, 24}};
Note riff_win_small_data[] = {{159, 8}, {179, 8}, {159, 8}, {179, 8}, {127, 8}, {142, 8}, {127, 8}, {142, 8}, {119, 24}};

Note riff_gain_big_data[] = {{119, 12}, {142, 4}, {239, 8}, {179, 8}, {159, 8}, {142, 8}, {119, 16}};
Note riff_gain_small_data[] = {{119, 12}, {142, 4}, {119, 12}};

const Tune riff_lose = {riff_lose_data, sizeof(riff_lose_data)/sizeof(Note)};
const Tune riff_win_big = {riff_win_big_data, sizeof(riff_win_big_data)/sizeof(Note)};
const Tune riff_win_small = {riff_win_small_data, sizeof(riff_win_small_data)/sizeof(Note)};
const Tune riff_gain_big = {riff_gain_big_data, sizeof(riff_gain_big_data)/sizeof(Note)};
const Tune riff_gain_small = {riff_gain_small_data, sizeof(riff_gain_small_data)/sizeof(Note)};


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
    stop_tune();
    current_tune = riff;
    audio_counter = 0;
    duration_start = global_timer();
}

void stop_tune()
{
    current_tune = NULL;
    stop_tone();
}

bool audio_is_playing()
{
    return current_tune != NULL;
}

bool update_audio()
{

    if(!current_tune)
    {
        return false;
    }

    //40 count = 1 sec
    uint64_t duration_end = global_timer();
    if (duration_end - duration_start >= current_tune->notes[audio_counter].dur) 
    {
        ++audio_counter;
        duration_start = duration_end;
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
