
#include <cstdio>

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

