/*
 * eeprom_start.c
 *
 * Created: 12.09.2019 19:51:23
 *  Author: mrkev
 */
#include "eeprom.h" 

			//refresh memory blocks
void eeprom_read_prog(char prog_num ){
		
		
		char i;
			prog_num=prog_num-1;
			prog_num=prog_num*100;
		for(i=1;i!=101;i=i+1){
			programm[i]=eeprom_read_byte((void*)prog_num+i);
		}
			
			
}
/*

void eeprom_start(){
			//read all memory 
		
		eeprom_read_block((void*)&prog1,(const void*)0,100);
		eeprom_read_block((void*)&prog2,(const void*)100,100);
		eeprom_read_block((void*)&prog3,(const void*)200,100);
		eeprom_read_block((void*)&prog4,(const void*)300,100);
		eeprom_read_block((void*)&prog5,(const void*)400,100);
}


*/
void  eeprom_save(char prog[100],char prog_num){
	
			char i;
			prog_num=prog_num-1;
			prog_num=prog_num*100;
			
			//eeprom_write_block((const void*)&prog,( void*)prog_num,100);
			
			
			for(i=1;i!=101;i=i+1){
				eeprom:eeprom_write_byte((void*)prog_num+i,prog[i]);
			}
			

}