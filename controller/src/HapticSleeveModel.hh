
#ifndef __Haptic_Sleeve_Model_H_
#define __Haptic_Sleeve_Model_H_

typedef struct serialPortHandle {
#ifdef WIN32
	HANDLE handle;
#else
	int handle;
#endif
} SerialPortHandle;

class HapticSleeveModel {

	public:
		HapticSleeveModel();
		~HapticSleeveModel();
		bool ConnectToSleeve(const char *portName);
		bool DisconnectFromSleeve();
		bool IsSleeveConnected();

	private:
		bool testConnectionWithEcho();

	private:
		SerialPortHandle hSerial;
};

extern int verbosity;

#endif
