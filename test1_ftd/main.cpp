/*
 * This software is supplied on an as-is basis and no warranty as to their suitability for any particular purpose is either
 * made or implied. Future Technology Devices International Ltd. will not accept any claim for damages howsoever arising as
 * a result of use or failure of this software. Your statutory rights are not affected. This software or any variant of it
 * is not intended for use in any medical appliance, device or system in which the failure of the product might reasonably be
 * expected to result in personal injury. This document provides preliminary information that may be subject to change
 * without notice.
 *
 */

 /*
  * The following code assumes a FT232R chip is connected to a PC with the RXD pin and the TXD pin shorted together.
  * The FTDI VCP driver must be installed on the PC.
  * The first part of the code determines the com port assigned to the device automatically, so the user does not have to
  * check in device manager.
  *
  * This com port number is then then opened with the standard W32 Serial COM API calls.
  * Data is then sent to the device which will loop back on the wire used to short RXD to TXD and the application reads the
  * data to prove it looped round.
  */


#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "ftd2xx.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	FT_HANDLE fthandle;
	FT_STATUS res;
	LONG COMPORT;

	char COMx[5];
	int n;

	DCB dcb;
	HANDLE hCommPort;
	BOOL fSuccess;


	/***********************************************************************
	//Find the com port that has been assigned to your device.
	/***********************************************************************/

	res = FT_Open(0, &fthandle);

	if (res != FT_OK) {

		printf("opening failed! with error %d\n", res);

		return 1;
	}


	res = FT_GetComPortNumber(fthandle, &COMPORT);

	if (res != FT_OK) {

		printf("get com port failed %d\n", res);

		return 1;
	}

	if (COMPORT == -1) {

		printf("no com port installed \n");
	}

	else {
		printf("com port number is %d\n", COMPORT);

	}


	FT_Close(fthandle);


	/********************************************************/
	// Open the com port assigned to your device
	/********************************************************/

	//n = sprintf(COMx, "COM%d", COMPORT);

	hCommPort = CreateFile(
		L"COM8",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hCommPort == INVALID_HANDLE_VALUE)
	{

		printf("Help - failed to open\n");
		return(1);

	}


	printf("Hello World!\n");

	/********************************************************/
	// Configure the UART interface parameters
	/********************************************************/

	fSuccess = GetCommState(hCommPort, &dcb);


	if (!fSuccess)

	{
		printf("GetCommStateFailed \n", GetLastError());
		return (2);

	}

	//set parameters.

	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	fSuccess = SetCommState(hCommPort, &dcb);


	if (!fSuccess)

	{
		printf("SetCommStateFailed \n", GetLastError());
		return (3);

	}


	printf("Port configured \n");


	/********************************************************/
	// Writing data to the USB to UART converter
	/********************************************************/

	DWORD dwwritten = 0, dwErr;
	char data_out[12] = "HELLO WORLD";
	DWORD w_data_len = 12;


	/*fSuccess = WriteFile(hCommPort, &data_out, w_data_len, &dwwritten, NULL);


			if (!fSuccess)

			{
				dwErr = GetLastError();
				printf("Write Failed \n", GetLastError());
				return (4);

			}


			printf("bytes written = %d\n", dwwritten);  /*

	/********************************************************/
	//Reading data from the USB to UART converter
	/********************************************************/

	char buf[256];
	DWORD dwRead;
	int iloop;
	iloop = 0;

	// Configura los tiempos de espera
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (!SetCommTimeouts(hCommPort, &timeouts)) {
		std::cerr << "Error al configurar los tiempos de espera del puerto serie" << std::endl;
		CloseHandle(hCommPort);
		return 1;
	}




	/*while (iloop++ < 50)
	{
		printf("*");


		memset(buf, 0, 256);

		int bufSize = sizeof(buf);

		// Imprime los valores del buffer
		for (int i = 0; i < bufSize; i++) {
			std::cout << std::hex << static_cast<int>(buf[i]) << " "; // Imprime el valor hexadecimal de cada byte
		}


		if (ReadFile(hCommPort, buf, w_data_len, &dwRead, NULL))

		{

			printf("data read = %s\n", buf);
			for (int i = 0; i < sizeof(buf); i++) {
				std::cout << std::hex << static_cast<int>(buf[i]) << " "; // Imprime el valor hexadecimal de cada byte
			}


		}
	} */

	// Configura el puerto serie para recibir eventos de caracteres
	if (!SetCommMask(hCommPort, EV_RXCHAR)) {
		std::cerr << "Error al configurar el puerto serie" << std::endl;
		CloseHandle(hCommPort);
		return 1;
	}


	DWORD dwEventMask;
	DWORD dwSize = 0;
	// Espera a que ocurra un evento en el puerto serie
	if (WaitCommEvent(hCommPort, &dwEventMask, NULL)) {
		DWORD dwIncommingReadSize;

		do {
			// Lee datos del puerto serie
			if (ReadFile(hCommPort, buf, sizeof(buf), &dwIncommingReadSize, NULL) != 0) {
				if (dwIncommingReadSize > 0) {
					dwSize += dwIncommingReadSize;
					// Puedes procesar los datos recibidos aqu� o almacenarlos en otro lugar seg�n sea necesario
					// Imprimir los datos recibidos
					for (DWORD i = 0; i < dwIncommingReadSize; i++) {
						std::cout << std::hex << static_cast<int>(buf[i]) << " ";
					}
					std::cout << std::endl;

					for (DWORD i = 0; i < dwIncommingReadSize; i++) {
						if (isprint(buf[i])) {
							std::cout << buf[i]; // Si el car�cter es imprimible, impr�melo
						}
						else {
							std::cout << "."; // De lo contrario, imprime un punto
						}
					}
					std::cout << std::endl;
				}
			}
			else {
				std::cerr << "Error al leer datos del puerto serie" << std::endl;
				CloseHandle(hCommPort);
				return 1;
			}
		} while (dwIncommingReadSize > 0);
	}
	else {
		std::cerr << "Error al esperar eventos en el puerto serie" << std::endl;
		CloseHandle(hCommPort);
		return 1;
	}








	/********************************************************/
	//Closing the device at the end of the program
	/********************************************************/


	CloseHandle(hCommPort);

	getchar();
	return 0;
}