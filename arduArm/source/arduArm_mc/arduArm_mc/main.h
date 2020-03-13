/*
 * main.h
 *
 * Created: 10.09.2019 21:03:04
 *  Author: mrkev
 */ 


#ifndef MAIN_H_

#define MAIN_H_

#define F_CPU 16000000UL
#define FOSC 16000000
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <uart/uart.c>
//#include <twimaster.c>
//#define LCD_PCF8574_DEVICEID 0x3F
#define UART_BAUD_RATE 9600
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

/* Define UART buad rate here */



#endif /* MAIN_H_ */