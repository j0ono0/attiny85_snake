#include "engine.h"

typedef struct Note {
    uint8_t freq;
    uint8_t dur;
}  Note;

typedef struct Tune {
    Note *notes;
    uint8_t len;
} Tune;

Note riff1_data[] = {{239, 20}, {213, 10}, {190, 20}, {239, 10}};
Tune riff1 = {riff1_data, sizeof(riff1_data)/sizeof(Note)};

uint8_t tune1[] = {239, 213, 190, 179, 159, 142, 127, 119};
uint8_t tune2[] = {239, 190, 239, 119, 142, 119, 142, 119};

Tune *current_tune = NULL;

uint8_t audio_counter = 0;
int intr_count=0;

ISR (TIMER0_COMPA_vect)
{
    ++intr_count;
}

void audio_config()
{
    // Initialises interrupt on timer/counter

    TCCR0A = 0x00;
    TCCR0B = 0x00;

    TCCR0A  |= (1 << WGM01); // Mode 2.  Clear Timer on Compare Match (CTC) mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // Clk/1024 prescale
    
    OCR0A = 195; // Sets interrupt to occur every 25ms (at 8MHz clock)
    TCNT0 = 0;

    TIMSK|=(1<<OCIE0A); // enable specific interrupt 
}


bool boxes_overlap(box *b1, box *b2)
{
    if(
        (b1->x1 > b2->x2) |
        (b1->x2 < b2->x1) |
        (b1->y1 > b2->y2) |
        (b1->y2 < b2->y1)
    )
    {
        return false;
    }
    return true;
}


void draw_box(uint8_t x, uint8_t y)
{
    // draw a box 8x8 pixels into a 8x8 grid
    // screen grid is 16 wide x 8 high
    set_column_address(x*8, x*8 + 7);

    set_page_address(y, y+1);
    ssd1306_start_data();
    for(int i=0; i < 8; ++i)
    {
        i2c_write_byte(0xFF);
    }
    ssd1306_stop();
}


void led_on()
{
	DDRB |= (1 << DDB4);
	PORTB |=  (1 << PB4); 
}

void led_off()
{
	PORTB &=  ~(1 << PB4); 
}

void toggle_led()
{
    PORTB ^= (1 << PB4);
}

void start_tune()
{
    current_tune = &riff1;
    audio_counter = 0;
    intr_count = 0;
    sei();
}

void update_audio()
{
    //40 count = 1 sec
    if (intr_count >= current_tune->notes[audio_counter].dur) 
    {
        ++audio_counter;
        intr_count=0;
    }
    // End tune
    if(audio_counter >= current_tune->notes[audio_counter].dur)
    {
        current_tune = NULL;
        stop_tone();
        led_off();
        cli();
    }
    // Play next note
    if(current_tune)
    {
        play_tone(current_tune->notes[audio_counter].freq);
        led_on();
    }

}