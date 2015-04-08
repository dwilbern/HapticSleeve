
#ifndef __Haptic_Sleeve_Model_H_
#define __Haptic_Sleeve_Model_H_

#ifdef WIN32
#include <windows.h>
#endif

#define FRONTPOS 0
#define BACKPOS 120
#define DEGREESTOLERANCE 20

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
		void setMotorPos(int pos);
		int getMotorPos();
		bool isMotorAtFront();
		bool isMotorAtBack();
		void moveToFront();
		void moveToBack();

	private:
		SerialPortHandle hSerial;
		bool calibrated;
		int currentPos;
};

extern int verbosity;

#endif

