
#include <cstring>

#include "HapticSleeveModel.hh"
#include "SerialPort.h"

HapticSleeveModel::HapticSleeveModel() {
	hSerial.handle = GetInvalidHandle();
	verbosity = 2;
}

HapticSleeveModel::~HapticSleeveModel() {
	if(IsHandleValid(hSerial.handle))
		CloseSerialPort(hSerial.handle);
}

// 0 means don't print any error messages, status information, or debugging info.
// 1 means print error messages only.
// 2 means print error messages and status information only.
// 3 means print error messages, status information, and debugging information.
void HapticSleeveModel::SetVerbosity(int v) {
	verbosity = v;
}

// Given the name of the serial port to open (e.g. "COM5),
// Open a serial connection and do any necessary initialization.
// Return true on success, false on failure.
bool HapticSleeveModel::ConnectToSleeve(char *portName) {
	hSerial.handle = OpenSerialPort(portName);
	return IsHandleValid(hSerial.handle);
}

// Close the serial port connection.
void HapticSleeveModel::DisconnectFromSleeve() {
	if(IsHandleValid(hSerial.handle)) {
		CloseSerialPort(hSerial.handle);
		hSerial.handle = GetInvalidHandle();
	}
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

// Send an echo request to the arduino.  Return true if we get a valid reply.
bool HapticSleeveModel::testConnectionWithEcho() {

	char *buf = "e testing";
	int bufsz = strlen(buf);

	WriteToSerialPort(hSerial.handle, (uint8_t *)buf, bufsz);
	memset(buf,0,bufsz);
	ReadFromSerialPort(hSerial.handle, (uint8_t *)buf, bufsz-2);

	return (strcmp(buf,"testing") == 0);
}

