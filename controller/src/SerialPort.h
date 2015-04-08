#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
void PrintErrorMsg(DWORD err);
HANDLE OpenSerialPort(const char *portName);
void WriteToSerialPort(HANDLE hSerial, void *buffer, int length);
int ReadFromSerialPort(HANDLE hSerial, void *buffer, int length);
void CloseSerialPort(HANDLE hSerial);
int IsHandleValid(HANDLE hSerial);
HANDLE GetInvalidHandle();

#else
void PrintErrorMsg(unsigned int err);
int OpenSerialPort(const char *portName);
void WriteToSerialPort(int serialfd, void *buffer, int length);
int ReadFromSerialPort(int serialfd, void *buffer, int length);
void CloseSerialPort(int serialfd);
int IsHandleValid(int serialfd);
int GetInvalidHandle();
#endif // WIN32

extern int verbosity;

#endif // _SERIALPORT_H_

