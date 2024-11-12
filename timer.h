#ifndef TIMER_J0ONO0_h
#define TIMER_J0ONO0_h

    /*

    Utility timer for various tasks

    */

    #include <avr/interrupt.h>
    

    void init_global_timer();
    uint64_t global_timer();


#endif