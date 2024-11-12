#ifndef AUDIO_J0ONO0_h
#define AUDIO_J0ONO0_h

    #include <stdio.h>
    #include <stdbool.h>
    #include <util/delay.h> 
    #include<avr/io.h>
    #include <avr/interrupt.h>
    #include "engine.h"
    #include "timer.h"




    typedef struct note {
        uint8_t freq;
        uint8_t dur;
    } Note;

    typedef struct tune {
        Note *notes;
        uint8_t len;
    } Tune;


    extern const Tune riff_lose;
    extern const Tune riff_rebound_top;
    extern const Tune riff_rebound_bottom;


    void audio_config();
    void play_tone(uint8_t tone);
    void stop_tone();
    void start_tune(const Tune *riff);
    void stop_tune();
    bool update_audio();
    

#endif