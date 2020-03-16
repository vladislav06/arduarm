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
#include <util/delay.h>
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
	
	uart_putc(85);	//готовы принимать
	_delay_ms(1);
	
	UCSR0B = (1<<RXEN0);  //  включаем прием
	
	while( eeprom_cell<1024){	
		while ( !(UCSR0A & (1<<RXC0)) );	/* Wait for data to be received */
												//в переменную        
		eeprom_write_byte(eeprom_cell,UDR0);	//записываем в еепром
		eeprom_cell++;		// ячейча +1
	}
	
	
	uart_putc(0b10101010);	//закончили принимать
	
	PORTB = 0b00000000;	// turn off build in led 
}