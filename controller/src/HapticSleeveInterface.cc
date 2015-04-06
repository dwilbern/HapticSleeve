
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "HapticSleeveModel.hh"
#include "HapticSleeveInterface.hh"

HapticSleeveInterface::HapticSleeveInterface() {
	model = new HapticSleeveModel();
}

HapticSleeveInterface::~HapticSleeveInterface() {
	delete model;
}

void HapticSleeveInterface::Connect(const char *portName) {

	bool success = model->ConnectToSleeve(portName);

	if(!success) {
		if(verbosity >= 1)
			fprintf(stderr,"Couldn't connect to %s.\n", portName);
	} else
		if(verbosity >= 2)
			printf("Connected to %s successfully.\n", portName);
}

void HapticSleeveInterface::Disconnect() {

	bool success = model->DisconnectFromSleeve();

	if(!success) {
		if(verbosity >= 1)
			fprintf(stderr,"Did not disconnect from port.  Were you already not connected?\n");
	} else
		if(verbosity >= 2)
			printf("Disconnected from sleeve.\n");
}

void HapticSleeveInterface::Calibrate() {

	bool success = model->CalibrateSleeve();

	if(!success) {
		if(verbosity >= 1)
			fprintf(stderr,"Feedback calibration unsuccessful.\n");
	} else
		if(verbosity >= 2)
			printf("Feedback calibration successful.\n");
}

void HapticSleeveInterface::RunForNumberOfTimes(int n) {

	bool success = model->Run(n);

	if(!success) {
		if(verbosity >= 1)
			fprintf(stderr,"Attempted to run %d times and was unsuccessful.\n", n);
	} else
		if(verbosity >= 2)
			fprintf(stderr,"Successfully ran %d times.\n", n);
}

void HapticSleeveInterface::Echo(const char *str) {

	int strLen = strlen(str);
	char *buf = (char *)malloc(strLen);
	strcpy(buf,str);

	model->EchoFromSleeve(buf,strLen);
	if(buf[0] == '\0') {
		if(verbosity >= 1)
			printf("Arduino did not reply.\n");
	} else
		printf("Arduino's reply: %s\n",buf);

	free(buf);
}

