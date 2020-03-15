/*
 * bootloader.c
 *
 * Created: 14.03.2020 12:54:04
 *  Author: mrkev
 */ 


//#include "main.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <uart.h>

#define F_CPU 16000000UL
#define UART_BAUD_RATE 9600

void boot_eeprom(void){
	//uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));	//init uart to get data
	sei();
	
	//			indication		//
	DDRB = 0b00100000;	//13 as output
	PORTB = 0b00000000;	//disable pull ups
	PORTB = 0b00100000;	// turn on build in led 
	
	// recive//
	
	uint8_t byte = 0;
	uint16_t eeprom_cell = 0;
	
	uart_putc(0b01010101);	//готовы принимать
	
	while(eeprom_cell<1024){
		byte = uart_getc();		//в переменную
		eeprom_write_byte(eeprom_cell,byte);	//записыцваем в еепром
		eeprom_cell++;		// €чейча +1
	}
	
	
	uart_putc(0b10101010);	//законьчили принимать
	
	PORTB = 0b00000000;	// turn off build in led 
}