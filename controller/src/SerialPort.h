#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#include <stdint.h>

#ifdef WIN32
void PrintErrorMsg(DWORD err);
HANDLE OpenSerialPort(char *portName);
void writeToSerialPort(HANDLE hSerial, uint8_t *buffer, int length);
void readFromSerialPort(HANDLE hSerial, uint8_t *buffer, int length);
void closeSerialPort(HANDLE hSerial);

#else
void handleErrno(unsigned int err);
int OpenSerialPort(char *portName);
void writeToSerialPort(int serialfd, uint8_t *buffer, int length);
void readFromSerialPort(int serialfd, uint8_t *buffer, int length);
void closeSerialPort(int serialfd);
#endif // WIN32

#endif // _SERIALPORT_H_

