 






	12|34|56|78
0b  11|xx|xx|xx


setup:

	Speed of arm(angle per min)
	11 00 10 01  (201)
	angles per minute(0-255
	

normal commands:

	move (normal byte)
	11000111 (199)

	wait sec
	11001010  (202)
	seconds (0-255)
	
	setup return point 
	11001011 (203)
	number of point (0-255)
	
	return to point 
	11001100(204)
	number of point (0-255)
	
	
	

/*
Start of program (n of first byte) 
11 00 10 10

End of program (n of last byte)
11 00 10 11
*/

comands must be >199(dec)

  Arm control:


11 00 0x xx

1
1
0
0
0
X   if enabled  root angle is enabled 
X   if enabled 1 angle is enabled
X   if enabled 2 angle is enabled 


101 = root angle is enabled and 2 angle is enabled 

 Arm controll byte  |  root angle (0-180) |     2 angle 
					|		       		  |
     11 00 0|1 01|  |     010 11 010      |    00 01 11 10
             x xx


Sec angle 
      ^						
     O_______
    /
   /
O >  first anlgle
^
Root angle 



11001010
00111100
11000111




union{
	float16 fangle;
	struct{
		uint8_t byte1;
		uint8_t byte2;
	}members;
}angle





angle.members.byte1=...
angle.members.byte2=...
...=angle.fangle













