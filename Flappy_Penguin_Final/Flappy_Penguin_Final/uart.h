/*
 * uart.h
 *
 * Created: 9/3/2025 5:37:15 PM
 *  Author: mattp
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdint.h>

void uart_init(uint16_t ubrr); // Initialize the UART to transmit, receive, with 8N1 data frame with no parity
uint8_t uart_receive(void); // Receives a character through the UART


#endif /* UART_H_ */