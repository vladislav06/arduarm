/*
 * _0.c
 *
 * Created: 10.09.2019 21:19:19
 *  Author: mrkev
 */ 
#include "main.h"
#include "eeprom.h"
#include "uart/uart.h"
 



int main(void) {
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();

	
}
