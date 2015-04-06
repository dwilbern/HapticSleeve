
#ifndef __Haptic_Sleeve_Interface_H_
#define __Haptic_Sleeve_Interface_H_

class HapticSleeveModel;

class HapticSleeveInterface {

	public:
		HapticSleeveInterface();
		~HapticSleeveInterface();
		void Connect(const char *portName);
		void Disconnect();
		void Calibrate();
		void Echo(const char *str);

	private:
		HapticSleeveModel *model;
};

extern int verbosity;

#endif

