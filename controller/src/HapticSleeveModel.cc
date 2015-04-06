
#include <cstdio>
#include <cstring>
#include <cstdlib>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "HapticSleeveModel.hh"
#include "SerialPort.h"

HapticSleeveModel::HapticSleeveModel() {
	hSerial.handle = GetInvalidHandle();
}

HapticSleeveModel::~HapticSleeveModel() {
	if(IsHandleValid(hSerial.handle))
		CloseSerialPort(hSerial.handle);
}

// Given the name of the serial port to open (e.g. "COM5"),
// Open a serial connection and do any necessary initialization.
// Return true on success, false on failure.
bool HapticSleeveModel::ConnectToSleeve(const char *portName) {
	hSerial.handle = OpenSerialPort(portName);
	if(IsHandleValid(hSerial.handle))
		return testConnectionWithEcho();
	else
		return false;
}

// Close the serial port connection.  Returns true if a the port was really closed.
bool HapticSleeveModel::DisconnectFromSleeve() {
	if(IsHandleValid(hSerial.handle)) {
		CloseSerialPort(hSerial.handle);
		hSerial.handle = GetInvalidHandle();
		return true;
	}
	return false;
}

// Run the sleeve's feedback calibration routine.  Return true if the operation was
// successful.
bool HapticSleeveModel::CalibrateSleeve() {
	int timeElapsed = 0;
	char *s = "c ";
	char *buf = (char *)malloc(sizeof(s)+4);
	strcpy(buf,s);

	if(IsHandleValid(hSerial.handle)) {
		WriteToSerialPort(hSerial.handle, buf, 1);
		do {
			usleep(500000); // 0.5 sec
			timeElapsed += 500000;
			if(verbosity >= 1) {
				printf(". ");
				fflush(stdout);
			}
		} while(ReadFromSerialPort(hSerial.handle, buf, 4) == 0 && timeElapsed < 10000000); // 10 sec

		if(verbosity >= 1)
			printf("\n");

		if(timeElapsed >= 10000000) {
			if(verbosity >= 1)
			fprintf(stderr,"Calibration timed out.\n");
			goto retFalse;
		}

		if(strcmp(buf,"done") == 0)
			goto retTrue;
		else {
			if(verbosity >= 1)
				fprintf(stderr,"Recieved unexpected reply.\n");
			goto retFalse;
		}
	}
	if(verbosity >= 1)
			fprintf(stderr,"Not connected to sleeve.\n");
	goto retFalse;

retTrue:
	free(buf);
	return true;
retFalse:
	free(buf);
	return false;
}

// Return true if we have a working connection to the Arduino.
// This is determined by first checking if our serial port handle is
// valid and then sending an echo request to the Arduino.
bool HapticSleeveModel::IsSleeveConnected() {

	if(IsHandleValid(hSerial.handle))
		if(testConnectionWithEcho())
			return true;

	return false;
}

// Test the connection is functioning properly by sending an echo request and verifying
// we got the correct answer.
bool HapticSleeveModel::testConnectionWithEcho() {

	char *testStr = "testing123";
	char *buf = (char *)malloc(strlen(testStr));
	strcpy(buf,testStr);

	EchoFromSleeve(buf, strlen(buf));

	if(verbosity >= 3)
		printf("Echo test: sent %s and got back %s.\n", testStr,buf);

	int result = strcmp(buf,testStr);
	free(buf);
	return (result == 0);
}

// Request the Arduino echo the contents of buf and copy its reply into buf.
void HapticSleeveModel::EchoFromSleeve(char *buf, int bufsz) {

	int microsElapsed = 0;
	char *buf2 = (char *)malloc(bufsz+2);
	buf2[0] = 'e';
	buf2[1] = ' ';
	strcpy(buf2+2,buf);
	memset(buf,0,bufsz);

	if(IsHandleValid(hSerial.handle)) {
		WriteToSerialPort(hSerial.handle, buf2, bufsz+2);
		while(microsElapsed < 1000000) { // 1 s
			usleep(10000); // 10 ms
			microsElapsed += 10000;
			if(ReadFromSerialPort(hSerial.handle, buf, bufsz) != 0)
				break;
		}

	} else
		if(verbosity >= 1)
			fprintf(stderr,"Not doing an echo request.  The port is not open.\n");

	free(buf2);
}

