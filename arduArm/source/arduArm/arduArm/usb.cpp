#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <fileapi.h>
#include <cstdint>
#include <iostream>
#include <bitset>
#include <climits>
#include <chrono>

namespace USB {
	void write(char ComPortName[], uint8_t lpBuffer[],uint16_t lenght)
	{
		

		HANDLE hComm;                          // Handle to the Serial port
		//*ComPortName = char("\\\\.\\COM10");// Name of the Serial port(May Change) to be opened,
		BOOL   Status;

		printf("\n\n +==========================================+");
		printf("\n |  Serial Transmission (Win32 API)         |");
		printf("\n +==========================================+\n");
		/*----------------------------------- Opening the Serial Port --------------------------------------------*/
		open:
		hComm = CreateFileA(LPCSTR(ComPortName),                        // Name of the Port to be Opened
			GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
			0,                                 // No Sharing, ports cant be shared
			NULL,                              // No Security
			OPEN_EXISTING,                     // Open existing port only
			0,                                 // Non Overlapped I/O
			NULL);                             // Null for Comm Devices

		if (hComm == INVALID_HANDLE_VALUE) {
			goto open;
		}


		/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

		DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings

		if (Status == FALSE)
			printf("\n   Error! in GetCommState()");

		dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
		dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
		dcbSerialParams.StopBits = 1;    // Setting StopBits = 1
		dcbSerialParams.Parity = NOPARITY;      // Setting Parity = None 
	
		Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

		if (Status == FALSE)
		{
			printf("\n   Error! in Setting DCB Structure");
		}
		else
		{
			printf("\n   Setting DCB Structure Successfull\n");
			printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
			printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
			printf("\n       StopBits = %d", dcbSerialParams.StopBits);
			printf("\n       Parity   = %d", dcbSerialParams.Parity);
		}

		/*------------------------------------ Setting Timeouts --------------------------------------------------*/

		COMMTIMEOUTS timeouts = { 0 };

		timeouts.ReadIntervalTimeout = 50;
		timeouts.ReadTotalTimeoutConstant = 50;
		timeouts.ReadTotalTimeoutMultiplier = 10;
		timeouts.WriteTotalTimeoutConstant = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;

		if (SetCommTimeouts(hComm, &timeouts) == FALSE)
			printf("\n   Error! in Setting Time Outs");
		else
			printf("\n\n   Setting Serial Port Timeouts Successfull\n");


		/*----------------------------- Writing a Character to Serial Port----------------------------------------*/
		//char   lpBuffer[] = "A";		       // lpBuffer should be  char or byte array, otherwise write wil fail
		DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
		DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port

		dNoOFBytestoWrite = sizeof(lpBuffer); // Calculating the no of bytes to write into the port
		
		uint8_t reciveD[2];
		DWORD lpNumberOfBytesRead;

		printf("reading....\n");

	read:
		
		Status = ReadFile(hComm,
			reciveD,
			2,
			&lpNumberOfBytesRead,
			NULL);

	//	std::cout << std::bitset<sizeof(reciveD[0]) * CHAR_BIT>(reciveD[0]) << "|" << int(reciveD[0]) << std::endl;

		if (Status) {
			if (reciveD[0] == 85) {	//wait for sync(ready to recive) byte
				goto write;			// if we got that then write!
			}
			else {
				//printf("byte err \n");
				goto read;			// if we dont got that sad(((
			}
		}
		else {
			printf("read err \n");
			goto read;				// if we dont got that sad(((
		}
		
		
	write:
		std::cout << "wrirte" << std::endl;
		uint8_t buffer[1];
		uint8_t lenght8_t[2] = { lenght, (lenght >> 8) };
		//=============send lenght of data=============//
		buffer[0] = lenght8_t[0];
		
		Status = WriteFile(hComm,               // Handle to the Serialport
			buffer,						// Data to be written to the port 
			1,							// No of bytes to write into the port
			&dNoOfBytesWritten,			// No of bytes written to the port
			NULL);

		Sleep(2);
		buffer[0] = lenght8_t[1];

		Status = WriteFile(hComm,               // Handle to the Serialport
			buffer,						// Data to be written to the port 
			1,							// No of bytes to write into the port
			&dNoOfBytesWritten,			// No of bytes written to the port
			NULL);

		Sleep(10+(lenght*2));
		
		//+++++++++++send data++++++++++++//
		
		for (int i = 0; i < lenght; i ++) {

			buffer[0] = lpBuffer[i];
			//buffer[1] = lpBuffer[i+1];


			Status = WriteFile(hComm,               // Handle to the Serialport
				buffer,					// Data to be written to the port 
				1,						// No of bytes to write into the port
				&dNoOfBytesWritten,		// No of bytes written to the port
				NULL);
			Sleep(2);

		}


		if (Status == TRUE)
			printf("\n\n   Written to %s", ComPortName);
		else
			printf("\n\n   Error %d in Writing to Serial Port", GetLastError());

		CloseHandle(hComm);//Closing the Serial Port
		printf("\n ==========================================\n");
	//	_getch();
	}
}