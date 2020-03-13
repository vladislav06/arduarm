/*
 * eeprom_start.h
 *
 * Created: 12.09.2019 19:51:36
 *  Author: mrkev
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
	void eeprom_read_prog(char func_number );
	//void eeprom_start(void);
	void eeprom_save(char prog[100],char prog_num);
	char programm[100];
	

	


#endif /* EEPROM_START_H_ */