
#include <cstring>

#include "HapticSleeveModel.hh"
#include "SerialPort.h"

HapticSleeveModel::HapticSleeveModel() {
	hSerial.handle = GetInvalidHandle();
}

HapticSleeveModel::~HapticSleeveModel() {
	if(IsHandleValid(hSerial.handle))
		CloseSerialPort(hSerial.handle);
}

// Given the name of the serial port to open (e.g. "COM5),
// Open a serial connection and do any necessary initialization.
// Return true on success, false on failure.
bool HapticSleeveModel::ConnectToSleeve(const char *portName) {
	hSerial.handle = OpenSerialPort(portName);
	if(IsHandleValid(hSerial.handle))
		return true;
	//	return testConnectionWithEcho();
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
	ReadFromSerialPort(hSerial.handle, (uint8_t *)buf, bufsz-1);

	return (strcmp(buf," testing") == 0);
}

