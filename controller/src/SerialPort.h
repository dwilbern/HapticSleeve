#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#include <stdint.h>

#ifdef WIN32
void PrintErrorMsg(DWORD err);
HANDLE OpenSerialPort(char *portName);
void WriteToSerialPort(HANDLE hSerial, uint8_t *buffer, int length);
void ReadFromSerialPort(HANDLE hSerial, uint8_t *buffer, int length);
void CloseSerialPort(HANDLE hSerial);
int IsHandleValid(HANDLE hSerial);
HANDLE GetInvalidHandle();

#else
void PrintErrorMsg(unsigned int err);
int OpenSerialPort(char *portName);
void WriteToSerialPort(int serialfd, uint8_t *buffer, int length);
void ReadFromSerialPort(int serialfd, uint8_t *buffer, int length);
void CloseSerialPort(int serialfd);
int IsHandleValid(int serialfd);
int GetInvalidHandle();
#endif // WIN32

#endif // _SERIALPORT_H_

