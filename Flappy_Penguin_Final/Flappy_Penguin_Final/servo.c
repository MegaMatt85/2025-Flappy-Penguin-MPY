#include "common.h"
#include "servo.h"
#include "adc.h"
#include <avr/io.h>
#include <util/delay.h>

// -------- Timer0 settings (Phase Correct, TOP=0xFF) --------
// F_CPU = 2 MHz assumed. Prescaler = 64 -> tick = 32 us.
// Phase-correct period = 510 ticks -> ~16.32 ms (~61.3 Hz).
#if (F_CPU != 2000000UL)
#warning "This config is tuned for F_CPU=2 MHz. If you change F_CPU, recompute prescaler."
#endif

#define T0_PRESC_BITS   ((1<<CS01) | (1<<CS00))   // /64
#define T0_PRESC_VAL    64UL

// Compute tick and period in microseconds using integer math (no overflow)
static inline uint16_t t0_tick_us(void) {
	// For 2 MHz and /64 this is 32 us exactly
	return (uint16_t)((T0_PRESC_VAL * 1000000UL) / F_CPU);
}
static inline uint32_t t0_period_us(void) {
	return (uint32_t)510U * (uint32_t)t0_tick_us();
}

// Map microseconds to OCR (0..255) based on actual PWM period
static inline uint8_t us_to_ocr(uint16_t pulse_us) {
	if (pulse_us < SERVO_MIN_US) pulse_us = SERVO_MIN_US;
	if (pulse_us > SERVO_MAX_US) pulse_us = SERVO_MAX_US;
	uint32_t per_us = t0_period_us();                // ?16320
	uint32_t ocr = ( (uint32_t)pulse_us * 255U + (per_us/2U) ) / per_us;  // round
	if (ocr < 1U)   ocr = 1U;                        // keep non-zero pulse
	if (ocr > 254U) ocr = 254U;                      // avoid 100% edge case
	return (uint8_t)ocr;
}

static inline uint16_t angle_to_us(uint8_t deg) {
	if (deg > 180) deg = 180;
	uint32_t span = (uint32_t)(SERVO_MAX_US - SERVO_MIN_US);
	return (uint16_t)(SERVO_MIN_US + (span * deg) / 180UL);
}

void servo_init(void)
{
	// PD6 (OC0A) + PD5 (OC0B) as outputs
	DDRD |= (1 << DDD6) | (1 << DDD5);

	// Phase Correct PWM (WGM02:0 = 0b001): TOP=0xFF
	// Non-inverting on both channels
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
	TCCR0B = 0;                       // WGM02=0, prescaler not set yet

	// Center both servos (~1.5 ms)
	OCR0A = us_to_ocr(1500);          // PD6
	OCR0B = us_to_ocr(1500);          // PD5

	// Start timer with prescaler /64
	TCCR0B |= T0_PRESC_BITS;
}

// ---- Channel A: PD6 (OC0A) ----
void servo_write_us(uint16_t pulse) { OCR0A = us_to_ocr(pulse); }
void servo_write_angle(uint8_t deg) { servo_write_us(angle_to_us(deg)); }

// ---- Channel B: PD5 (OC0B) ----
void servo2_write_us(uint16_t pulse) { OCR0B = us_to_ocr(pulse); }
void servo2_write_angle(uint8_t deg) { servo2_write_us(angle_to_us(deg)); }


// Write both channels in the same frame
void servo_write_angles(uint8_t a_deg, uint8_t b_deg)
{
	// PD6 (OC0A)
	servo_write_angle(a_deg);
	// PD5 (OC0B)
	servo2_write_angle(b_deg);
}

// Sweep both the same way (0?180?0) with a fixed frame time
bool servos_sweep_in_phase(uint8_t step_deg)
{
	if (step_deg == 0) step_deg = 1;
	
	bool snoozed = 0;
	
	for (uint8_t a = 0; a <= 180; a += step_deg) {
		servo_write_angles(a, a);
		if (alarm_snoozed()) {
			snoozed = 1;
		}
		_delay_ms(30);
	}
	for (int16_t a = 180; a >= 0; a -= step_deg) {
		servo_write_angles((uint8_t)a, (uint8_t)a);
		if (alarm_snoozed()) {
			snoozed = 1;
		}
		_delay_ms(30);
	}
	
	return snoozed;
}