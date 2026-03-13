/*
 * watchdog.h
 *
 * Created: 9/3/2025 5:11:47 PM
 *  Author: mattp
 */ 


#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include <avr/io.h>

void wdt_init(void); // Initializes the Watchdog timer in interrupt mode to go off every ~8s

#endif /* WATCHDOG_H_ */