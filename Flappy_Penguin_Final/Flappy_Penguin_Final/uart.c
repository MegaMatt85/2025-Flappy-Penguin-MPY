/*
 * uart.c
 *
 * Created: 9/3/2025 5:37:31 PM
 *  Author: mattp
 */ 

#include "common.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init(uint16_t baud) {
	UCSR0A |= (1<<U2X0);
	uint16_t ubrr = (F_CPU/(8UL*baud)) - 1; // Calculate and set the value for the UBRR0 register
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1 << RXEN0);// | (1 << RXCIE0)// Enable UART to receive, transmit, and enable interrupt on receive complete
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set to 8N1 data frame
}

uint8_t uart_receive(void) {
	while ((UCSR0A & (1<<RXC0)) == 0) {
		;
	}
	
	uint8_t data = UDR0;
	return data;
}