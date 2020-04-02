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
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));	//init uart to get data
	sei();
	
	
	/* memory full erase */ 
	uint16_t eeprom_cell = 0;
	uint16_t progLenght = 0;
	uint8_t recive[2];
	uart_putc(85);	//готовы принимать
	//============get disasembled data===================//
	UCSR0B = (1<<RXEN0);
	
	while ( !(UCSR0A & 0b10000000) );
	recive[0] = UDR0;	
	
	while ( !(UCSR0A & 0b10000000) );
	recive[1] = UDR0;
	
	progLenght = (recive[1]<<8)|recive[0];	//assemble it in to uint16_t
	
	//=============erase eeprom=================//
	for(;eeprom_cell < progLenght; eeprom_cell++){
			/* Wait for completion of previous write */
			while(EECR & (1<<EEPE));
			
			/* Set up address and Data Registers */
			EEAR = eeprom_cell;
			
			/* erase only */
			EECR |= (1<<EEPM0);
			
			/* Write logical one to EEMPE */
			EECR |= (1<<EEMPE);
			/* start wrie*/
			EECR |= (1<<EEPE);
			
	}
	//uart_putc(86);
	eeprom_cell = 0;
	//			indication		//
	DDRB = 0b00111000;	//13 as output
	PORTB = 0b00111000;	// turn on build in led 
	
	// recive//
	
	//uint8_t byte = 0;
	
	
	//uart_putc(85);	//готовы принимать
	//_delay_ms(1);
	
	UCSR0B = (1<<RXEN0);  //  включаем прием
	//=============write eeprom=============//
	while( eeprom_cell < progLenght){	
		while ( !(UCSR0A & 0b10000000) );	/* Wait for data to be received */
		
												 
		/* Wait for completion of previous write */
		while(EECR & (1<<EEPE));
		
		/* set up address and data registers */
		EEAR = eeprom_cell;
		EEDR = UDR0;
		
		/* write only */
		EECR &=(0<<EEPM0);
		EECR |= (1<<EEPM1);
		
		/* Write logical one to EEMPE */
		EECR |= (1<<EEMPE);
		
		/* Start eeprom write by setting EEPE */
		EECR |= (1<<EEPE);		
		//eeprom_write_byte(eeprom_cell,UDR0);	//записываем в еепром
		eeprom_cell++;		// ячейча +1
	}
	
	
	uart_putc(0b10101010);	//закончили принимать
	
	PORTB = 0b00000000;	// turn off build in led 
}
