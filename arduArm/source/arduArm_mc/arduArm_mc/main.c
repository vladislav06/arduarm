/*
 * _0.c
 *
 * Created: 10.09.2019 21:19:19
 *  Author: mrkev
 */ 
#include "main.h"



/*
ddr  data direction 1out 0 in
port  output reg
pin input

*/

#define UART_BAUD_RATE 9600
unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

int main(void) {
	//tmp
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));	//init uart to get data
	sei();
	
	//---------
	
	uint8_t data = 0;
	
	DDRB = 0b0000000;		//all input
	PORTB = 0xFF;			//pull up
	data = PINB;
	
	_delay_ms(1000);
	
	if(data == 0b00000000){	//if 8 is off
		boot_eeprom();				// download programm and load it to eeprom
	}
	
	
	
	
			
	for(uint16_t i = 0; i < 1024;i++){	//���������� � ���� ��� ������
		uart_putc(EEPROM_read(i));
	}
	
	
	
}
