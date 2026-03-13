/*
1. Use the Watchdog Timer to make the alarm go off after x seconds ?
2. Use the Button to turn off the alarm ?
3. Use the Speaker to play the alarm sound <-
4. Use the Microphone to make the alarm snooze <-
5. Use the Motors to make the wings flap when the alarm goes off ?

*/
/*
 * Alarm thingy.c
 *
 * Created: 9/3/2025 12:12:27 AM
 * Author : mattp
 */

#define BAUD 9600
//#define TURN_ON_LED PINB |= (1 << PINB2)
#define SOUND_LED PORTB |= (1 << PINB2)
#define SOUND_LED_OFF (PORTB &= ~(1 << PINB2))
#define ALARM_IS_ACTIVE PINC & (1 << PINC5) // Button is not pressed
#define BUTTON_PRESSED ~PINC & (1 << PINC5)
#define SNOOZE PINB & (1 << PINB2)

#include "common.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "watchdog.h"
#include "uart.h"
#include "adc.h"
#include "servo.h"
#include "audio.h"


const uint16_t snooze_seconds = 16; // Set the snooze time to 16
volatile uint32_t count_s = 0; // The number of seconds counted
volatile bool start_counting = 0; // Indicates when to start counting
uint32_t target_seconds = 30; // Number of seconds to count to

void count_down(void); // Count until target has been reached

ISR(WDT_vect) {
	// Increment count_s whenever the Watchdog Timer times out
	count_s += 8;
}

int main(void)
{
	// Initialize the peripherals
	uart_init(BAUD);
	adc_init();
	wdt_init();
	servo_init();
	servo_write_angles(90, 90);
	sei();
	
	// Set the inputs and output pins ### DECIDE WHICH TO USE ###
	DDRC &= ~(1 << DDC5); // Set PC5 as button input
	PORTC |= (1 << PORTC5);
	
	// Enable sleep
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode to Power-down
	sleep_enable();
	
	// Enable sound
	audio_init();      
	audio_set_loop(0);        // loop alarm(0=no loop, 1=loop)  
	sei();
	
    while (1) 
    {	
		// Wait until the alarm time has been sent to the MCU, then set target_seconds from the input
		//uint8_t data = uart_receive();
		//target_seconds = data;
		//target_seconds = 16;
		
		//_delay_ms(1000);
		// Wait until alarm goes off
		count_down();
		
		// Wait until the alarm is turned off
		while (1) {
			// Check if microphone detects noise while arms are moving ### POSSIBLY CHANGE TO NOT DO AT SAME TIME ###
			if (servos_sweep_in_phase(30)) {
				target_seconds = snooze_seconds;
				count_down();
				continue;
			}
			
			// Alarm button is pressed and alarm turns off
			if (BUTTON_PRESSED) {
				break;
			}
			
			// Play the audio once
			audio_play(1);
			_delay_ms(1500);
			audio_stop();
			
			// Alarm button is pressed and alarm turns off
			if (BUTTON_PRESSED) {
				break;
			}
		}
    }
}

void count_down(void) {
	while(1) {
		sleep_cpu();
		
		// Stop counting when the set time has passed
		if (count_s >= target_seconds) {
			count_s = 0;
			break;
		}
	}
}

