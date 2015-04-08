
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "HapticSleeveModel.hh"
#include "SerialPort.h"

HapticSleeveModel::HapticSleeveModel() {
	hSerial.handle = GetInvalidHandle();
	calibrated = false;
	currentPos = 0;
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
	calibrated = false;
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
	} else
		if(verbosity >= 1)
			fprintf(stderr,"Not connected to sleeve.\n");
	goto retFalse;

retTrue:
	free(buf);
	calibrated = true;
	return true;
retFalse:
	free(buf);
	return false;
}

bool HapticSleeveModel::Run(int n) {

	if(!IsHandleValid(hSerial.handle)) {
		if(verbosity >= 1)
			fprintf(stderr,"Not running.  The serial port is not open.\n");
		return false;
	}
	if(!calibrated) {
		if(verbosity >= 1)
			fprintf(stderr,"Not running.  Feedback calibration routine has not been run.\n");
		return false;
	}
	if(n <= 0) {
		if(verbosity >= 1)
			fprintf(stderr,"Not running.  Please enter a positive integer.\n");
		return false;
	}

	int i;
	for(i = 1; i <= n; i++) {
		if(isMotorAtFront()) {
			moveToBack();
			if(!isMotorAtBack()) {
				if(verbosity >= 1)
					fprintf(stderr,"Failed to move to back on run #%d.\n", i);
				return false;
			} else
				if(verbosity >= 2)
					printf("Run #%d done: front to back.", i);

		} else if(isMotorAtBack()) {
			moveToFront();
			if(!isMotorAtFront()) {
				if(verbosity >= 1)
					fprintf(stderr,"Failed to move to front on run #%d.\n", i);
				return false;
			} else
				if(verbosity >= 2)
					printf("Run #%d done: back to front.\n", i);

		} else {
			moveToFront();
			if(!isMotorAtFront()) {
				if(verbosity >= 1)
					fprintf(stderr,"Run #%d: Tried to move from middle to front and failed.", i);
			} else
				if(verbosity >= 2)
					printf("Run #%d done: middle to front.\n", i);
		}
	}

	return true;
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

// Ask the Arduino to set the servo motor's position to pos.
void HapticSleeveModel::setMotorPos(int pos) {

	char buf[6];
	buf[0] = 'p';
	buf[1] = ' ';
	sprintf(buf+2,"%03d", pos);

	if(IsHandleValid(hSerial.handle))
		WriteToSerialPort(hSerial.handle, buf, 6);
	else
		if(verbosity >= 1)
			fprintf(stderr,"Failed to set the motor's position to %d.", pos);
	
}

// Ask the Arduino for the current position of the motor.  Return -100 if no reply.
int HapticSleeveModel::getMotorPos() {

	int microsElapsed = 0;
	char buf[3];
	buf[0] = 'g';
	buf[1] = ' ';

	if(IsHandleValid(hSerial.handle)) {
		WriteToSerialPort(hSerial.handle, buf, 2);
		while(microsElapsed < 100000) { // 100 ms
			if(ReadFromSerialPort(hSerial.handle, buf, 3) != 0)
				break;
			usleep(10000); // 10 ms
			microsElapsed += 10000;
		}
		if(microsElapsed >= 100000) {
			if(verbosity >= 1)
				fprintf(stderr,"Getting position timed out.\n");
			return -1;
		}

		currentPos = atoi(buf);
		return currentPos;
	} else {
		if(verbosity >= 1)
			fprintf(stderr,"Not getting position.  The port is not open.\n");
		return -100;
	}
}

bool HapticSleeveModel::isMotorAtFront() {
	getMotorPos();
	return abs(currentPos - FRONTPOS) <= DEGREESTOLERANCE;
}

bool HapticSleeveModel::isMotorAtBack() {
	getMotorPos();
	return abs(currentPos - BACKPOS) <= DEGREESTOLERANCE;
}

void HapticSleeveModel::moveToFront() {

	setMotorPos(FRONTPOS);

	while(!isMotorAtFront())
		usleep(100000); // 100 ms
}

void HapticSleeveModel::moveToBack() {
	
	setMotorPos(BACKPOS);

	while(!isMotorAtBack())
		usleep(100000); // 100 ms
}

