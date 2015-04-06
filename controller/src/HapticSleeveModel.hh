
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
		bool CalibrateSleeve();
		bool Run(int n);
		bool IsSleeveConnected();
		void EchoFromSleeve(char *buf, int bufsz);

	private:
		bool testConnectionWithEcho();
		int getMotorPos();

	private:
		SerialPortHandle hSerial;
		bool calibrated;
};

extern int verbosity;

#endif

