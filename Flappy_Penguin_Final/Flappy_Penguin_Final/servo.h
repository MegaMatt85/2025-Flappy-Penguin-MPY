/*
 * servo.h
 * Combined dual-servo (PD6=OC0A, PD5=OC0B) on Timer0.
 */

#ifndef SERVO_H
#define SERVO_H

// Match Proteus "Clock Frequency". Ensure CLKDIV8 is OFF.
#ifndef F_CPU
#define F_CPU 2000000UL
#endif

// 50 Hz nominal frame and limits (servos tolerate ~50–60 Hz)
#define SERVO_PERIOD_US 20000UL        // target frame (informational)
#define SERVO_MIN_US      500UL        // 0.5 ms
#define SERVO_MAX_US     2400UL        // 2.4 ms

#include <stdint.h>
#include <stdbool.h>

// Initializes Timer0 Phase-Correct PWM (TOP=0xFF) and enables PD6, PD5
void servo_init(void);

// ---- Channel A: PD6 (OC0A) ----
void servo_write_us(uint16_t pulse_us);
void servo_write_angle(uint8_t deg);       // 0..180

// ---- Channel B: PD5 (OC0B) ----
void servo2_write_us(uint16_t pulse_us);
void servo2_write_angle(uint8_t deg);      // 0..180

// Convenience sweeps (blocking)
void servo_rotate(void);                   // PD6
void servo2_rotate(void);                  // PD5
bool servos_sweep_in_phase(uint8_t step_deg);

void servo_write_angles(uint8_t a_deg, uint8_t b_deg);

#endif /* SERVO_H */