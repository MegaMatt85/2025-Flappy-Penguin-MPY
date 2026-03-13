#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "clip_8k.h"   // clip_sr (=8000), clip_length, clip_data[] PROGMEM
/* PB1 (OC1A) output, Timer1 = 31.25 kHz fast PWM carrier @2 MHz,
   Timer2 CTC ? 8 kHz sample tick. */

static volatile uint32_t sample_index = 0;
static volatile uint8_t  g_playing    = 0;
static volatile uint8_t  g_loop       = 1;

/* ---- Timer1: Fast PWM on OC1A, TOP=ICR1=63 => 31.25 kHz ---- */
static void pwm_timer1_init(void) {
    DDRB  |= _BV(PB1);                 // PB1 = OC1A
    ICR1   = 63;                       // 2e6/(63+1)=31.25 kHz
    OCR1A  = ICR1/2;                   // mid duty (silence)
    TCCR1A = _BV(COM1A1) | _BV(WGM11); // Fast PWM (mode 14), non-inverting
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // prescale=1
}

/* ---- Timer2: CTC for sample rate ---- */
static void sampler_timer2_setup(uint32_t rate_hz) {
    TCCR2A = _BV(WGM21);               // CTC
    TCCR2B = _BV(CS21);                // prescale 8
    OCR2A  = (uint8_t)(F_CPU / (8UL * rate_hz) - 1UL); // ~30 @ 8 kHz
    TIMSK2 &= ~_BV(OCIE2A);            // disabled until play()
    TIFR2   = _BV(OCF2A);              // clear pending
}

/* ---- Public API ---- */
void audio_init(void) {
    pwm_timer1_init();
    sampler_timer2_setup(clip_sr);
    OCR1A = ICR1/2;    // silence
    sample_index = 0;
    g_playing = 0;
    g_loop = 1;
}

void audio_play(uint8_t restart) {
    if (restart) sample_index = 0;
    TIFR2   = _BV(OCF2A);
    TIMSK2 |= _BV(OCIE2A);
    g_playing = 1;
}

void audio_stop(void) {
    TIMSK2 &= ~_BV(OCIE2A);
    OCR1A   = ICR1/2;   // silence
    g_playing = 0;
}

void audio_set_loop(uint8_t loop_on) {
    g_loop = (loop_on != 0);
}

/* ---- ~8 kHz sample tick ---- */
ISR(TIMER2_COMPA_vect) {
    if (sample_index >= clip_length) {
        if (g_loop) {
            sample_index = 0;
        } else {
            TIMSK2 &= ~_BV(OCIE2A);
            OCR1A   = ICR1/2;
            g_playing = 0;
            sample_index = 0;
            return;
        }
    }
    uint8_t s8 = pgm_read_byte(&clip_data[sample_index++]); // 0..255
    OCR1A = ((uint16_t)s8 * ICR1) / 255; // map to PWM range
}
