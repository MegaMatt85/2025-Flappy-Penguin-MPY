/*
 * watchdog.c
 *
 * Created: 9/3/2025 5:12:28 PM
 *  Author: mattp
 */ 

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "watchdog.h"

void wdt_init(void) {
	cli(); // Disables global interrupts
	wdt_reset(); // Reset the WDT so that it does not time out during configuration
	WDTCSR |= (1 << WDCE) | (1 << WDE); // Enable the other bits in this register to be changed and ensure MCU does not reset
	WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0); // Change WDT to interrupt mode and set prescaler to 1024K (timeout after 8s)
	sei(); // Enables global interrupts
}