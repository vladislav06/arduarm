#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <fileapi.h>
#include <cstdint>

namespace USB {
	void write(char ComPortName[], uint8_t lpBuffer[])
	{

		HANDLE hComm;                          // Handle to the Serial port
		//*ComPortName = char("\\\\.\\COM10");// Name of the Serial port(May Change) to be opened,
		BOOL   Status;

		printf("\n\n +==========================================+");
		printf("\n |  Serial Transmission (Win32 API)         |");
		printf("\n +==========================================+\n");
		/*----------------------------------- Opening the Serial Port --------------------------------------------*/

		hComm = CreateFileA(LPCSTR(ComPortName),                        // Name of the Port to be Opened
			GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
			0,                                 // No Sharing, ports cant be shared
			NULL,                              // No Security
			OPEN_EXISTING,                     // Open existing port only
			0,                                 // Non Overlapped I/O
			NULL);                             // Null for Comm Devices

		if (hComm == INVALID_HANDLE_VALUE)
			printf("\n   Error! - Port %s can't be opened", ComPortName);
		else
			printf("\n   Port %s Opened\n ", ComPortName);


		/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

		DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings

		if (Status == FALSE)
			printf("\n   Error! in GetCommState()");

		dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
		dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
		dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
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
			printf("\n\n   Setting Serial Port Timeouts Successfull");


		/*----------------------------- Writing a Character to Serial Port----------------------------------------*/
		//char   lpBuffer[] = "A";		       // lpBuffer should be  char or byte array, otherwise write wil fail
		DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
		DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port

		dNoOFBytestoWrite = sizeof(lpBuffer); // Calculating the no of bytes to write into the port

		Status = WriteFile(hComm,               // Handle to the Serialport
			lpBuffer,            // Data to be written to the port 
			dNoOFBytestoWrite,   // No of bytes to write into the port
			&dNoOfBytesWritten,  // No of bytes written to the port
			NULL);

		if (Status == TRUE)
			printf("\n\n   Written to %s", ComPortName);
		else
			printf("\n\n   Error %d in Writing to Serial Port", GetLastError());

		CloseHandle(hComm);//Closing the Serial Port
		printf("\n ==========================================\n");
	//	_getch();
	}
}