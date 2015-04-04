#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "SerialPort.h"

#ifdef WIN32
 #include <windows.h>
#else
 #include <unistd.h>
 #include <errno.h>
 #include <termios.h>
 #include <fcntl.h>	
#endif

#ifdef WIN32
// handle an error code from GetLastError()
void PrintErrorMsg(DWORD err) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;

    FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );

	fprintf(stderr,"%s", (char *)lpMsgBuf);

    LocalFree(lpMsgBuf);
}

/*! 
 * \brief Initialize Serial Port
 * \param COM PORT e.g. "COM3"
 * \return void, calls error handling function on error
 */

HANDLE OpenSerialPort(char *portName)
{
    char portname[15];

    sprintf(portname,"\\\\.\\%s",portName);
    HANDLE hSerial = CreateFile(portname,
            GENERIC_READ | GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            0,
            0);
    if (hSerial == INVALID_HANDLE_VALUE) {
		fprintf(stderr,"ERROR opening port %s.\n",portName);
        PrintErrorMsg(GetLastError());
        return INVALID_HANDLE_VALUE;
    }

    // configure port
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        //could not get the state of the comport
		CloseHandle(hSerial);
		fprintf(stderr,"ERROR getting state of the COM port.\n");
		PrintErrorMsg(GetLastError());
        return INVALID_HANDLE_VALUE;
    }
    // hardcoded to 115200 N81
    dcbSerialParams.BaudRate=115200;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams)){
		CloseHandle(hSerial);
		fprintf(stderr,"ERROR configuring COM port.\n");
		PrintErrorMsg(GetLastError());
        return INVALID_HANDLE_VALUE;
    }

    // configure timeouts
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts)){
		CloseHandle(hSerial);
		fprintf(stderr,"ERROR configuring timeouts.\n");
        PrintErrorMsg(GetLastError());
        return INVALID_HANDLE_VALUE;
    }

	return hSerial;
}

/* writes length bytes from buffer to hSerial.
 * returns when length bytes are written, cals the error handling function in case of error */
void WriteToSerialPort(HANDLE hSerial, uint8_t * buffer, int length)
{

    DWORD TotalBytesWritten = 0;
	DWORD BytesWritten;
	BOOL retValue;

    while(TotalBytesWritten < length) {
		retValue = WriteFile(hSerial,
							buffer + TotalBytesWritten,
							length - TotalBytesWritten,
							&BytesWritten,
							NULL);
		if(!retValue) {
			fprintf(stderr,"ERROR writing.\n");
			PrintErrorMsg(GetLastError());
		}

		TotalBytesWritten += BytesWritten;
    }
}

/* reads length bytes from hSerial into buffer.
 * returns when length bytes have been read, calls the error handling function in case of error */
void ReadFromSerialPort(HANDLE hSerial, uint8_t * buffer, int length)
{
    DWORD TotalBytesRead = 0;
	DWORD BytesRead;
	BOOL retValue;

	while(TotalBytesWritten < length) {
		retValue = ReadFile(hSerial,
							buffer + TotalBytesRead,
							length - TotalBytesRead,
							&BytesRead,
							NULL);
		if(!retValue) {
			fprintf(stderr,"ERROR reading.\n");
			PrintErrorMsg(GetLastError());
		}

		TotalBytesRead += BytesRead;
    }
}

void CloseSerialPort(HANDLE hSerial)
{
    CloseHandle(hSerial);
}

int IsHandleValid(HANDLE hSerial) {
	return (hSerial != INVALID_HANDLE_VALUE);
}

HANDLE GetInvalidHandle() { return INVALID_HANDLE_VALUE; }

#else // defined(WIN32)
//Handle an error code obtained from errno
void PrintErrorMsg(unsigned int err) {

	perror(NULL);
}

int OpenSerialPort(char *portName) {
	
	int serialfd;
	struct termios serialport;

	/* Open file descriptor */
	serialfd = open(portName, O_RDWR | O_NONBLOCK);

	if(serialfd < 0) {
		fprintf(stderr,"ERROR oprning serial port %s.\n",portName);
		PrintErrorMsg(errno);
		return -1;
	}

	/* Configure Serial Port */
	memset(&serialport,0,sizeof(struct termios));
	if(tcgetattr(serialfd,&serialport) == -1) {
		close(serialfd);
		fprintf(stderr,"ERROR getting attributes.\n");
		PrintErrorMsg(errno);
		return -1;
	}
	
	cfsetospeed(&serialport,B115200);
	cfsetispeed(&serialport,B115200);

	serialport.c_cflag &= ~PARENB; /* no parity checking */
	serialport.c_cflag &= ~CSTOPB; /* one stop bit */
	serialport.c_cflag &= ~CSIZE;
	serialport.c_cflag |= CS8; /* 8 bit character size */
	serialport.c_cflag &= ~CRTSCTS; /* no hardware flow control */
	serialport.c_lflag = 0; /* no signaling chars, no echo, no canonical processing */
	serialport.c_oflag = 0; /* no remapping, no delays */
	serialport.c_cc[VMIN] = 0; /* read doesn't block */
	serialport.c_cc[VTIME] = 5; /* 0.5 second read timeout */
	serialport.c_cflag |= CREAD | CLOCAL; /* turn on READ and ignore control lines */
	serialport.c_iflag &= ~(IXON | IXOFF | IXANY); /* turn off software flow control */
	serialport.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* make raw */
	serialport.c_oflag &= ~OPOST; /* make raw */

	/* flush port, then apply attributes */
	tcflush(serialfd,TCIFLUSH);

	if(tcsetattr(serialfd,TCSANOW,&serialport) == -1) {
		close(serialfd);
		fprintf(stderr,"ERROR setting attributes.\n");
		PrintErrorMsg(errno);
		return -1;
	}

	return serialfd;
}

/* write length bytes from buffer to serialfd.
 * return when length bytes are written, call error handler function in case of error */
void WriteToSerialPort(int serialfd, uint8_t *buffer, int length) {

	int totalBytesWritten = 0;
	int bytesWritten;

	while(totalBytesWritten < length) {
		do {
			bytesWritten = write(serialfd,
							buffer + totalBytesWritten,
							length - totalBytesWritten);

			if(bytesWritten < 0 && errno != EINTR) { /* do not abort the write on an interrupt, try again instead */
				fprintf(stderr,"ERROR: write error.\n");
				PrintErrorMsg(errno);
			}

	} while(bytesWritten < 0);

	totalBytesWritten += bytesWritten;
	}
}

/* read length bytes from serialfd into buffer.
 * return when length bytes are read, call error handler function in case of error */
void ReadFromSerialPort(int serialfd, uint8_t *buffer, int length) {

	int totalBytesRead = 0;
	int bytesRead;

	while(totalBytesRead < length) {
		do {
			bytesRead = read(serialfd,
							buffer + totalBytesRead,
							length - totalBytesRead);

			if(bytesRead < 0 && errno != EINTR) { /* do not abort the read on an interrupt, try again instead */
				fprintf(stderr,"ERROR: read error.\n");
				PrintErrorMsg(errno);
			}

	} while(bytesRead < 0);

	totalBytesRead += bytesRead;
	}
}

/* close the serial port's file descriptor */
void CloseSerialPort(int serialfd) {
	close(serialfd);
}

int IsHandleValid(int serialfd) {
	return (serialfd > 0);
}

int GetInvalidHandle() { return -1; }

#endif

