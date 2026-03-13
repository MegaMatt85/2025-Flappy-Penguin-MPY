
/*
 * adc.h
 *
 * Created: 10/09/2025 12:31:19 pm
 *  Author: calvi
 */ 


#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>
#include <stdbool.h>

void adc_init(); // Initializes the ADC
uint16_t adc_read(uint8_t chan); // Reads the inputted ADC channel
uint16_t adc_convert_mv(int value); // Converts the ADC value to mV
bool alarm_snoozed(void); // Returns true if mic threshold has been reached

#endif