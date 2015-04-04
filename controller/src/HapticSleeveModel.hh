
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
		void SetVerbosity(int v);
		bool ConnectToSleeve(char *portName);
		void DisconnectFromSleeve();
		bool IsSleeveConnected();

	private:
		bool testConnectionWithEcho();

	private:
		SerialPortHandle hSerial;
		int verbosity;
};

#endif
