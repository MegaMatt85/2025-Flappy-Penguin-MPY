/*
 * adc.c
 *
 * Created: 10/09/2025 12:31:08 pm
 *  Author: calvi
 */ 

#include "common.h"
#include "adc.h"
#include <avr/io.h>

const uint16_t mic_threshold = 200; // Set the noise threshold for the microphone

void adc_init() {
	ADMUX |= (1 << REFS0); // Sets Vref to AVcc
	ADCSRA |= (1<<ADEN) | (1<<ADPS2); // Enables the ADC and sets the prescaler to 16
	//ADCSRB = 0;
	//DIDR0 = 0;
}
uint16_t adc_read(uint8_t chan) {
	ADMUX &= 0xF0; // De-selects all ADC input channels 
	ADMUX |= chan; // Selects the channel to convert from
	ADCSRA |= (1 << ADSC); // Starts the ADC conversion
	
	// Returns the ADC value once conversion is complete
	while ((ADCSRA & (1 << ADIF)) == 0) {
		;
	}
	return ADC;
}

uint16_t adc_convert_mv(int value) {
	return (float)4.88 * value; // voltage_mV = ...
}

bool alarm_snoozed(void) {
	// Read the voltage input from the microphone if
	uint16_t adc_reading = adc_read(0);
	adc_reading = adc_convert_mv(adc_reading);
	
	// Snooze the alarm when a loud enough noise has been made
	if (adc_reading > mic_threshold) {
		return 1;
	}
	
	return 0;
}