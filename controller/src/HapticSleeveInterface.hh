
#ifndef __Haptic_Sleeve_Interface_H_
#define __Haptic_Sleeve_Interface_H_

class HapticSleeveModel;

class HapticSleeveInterface {

	public:
		HapticSleeveInterface();
		~HapticSleeveInterface();
		void Connect(const char *portName);
		void Disconnect();

	private:
		HapticSleeveModel *model;
};

extern int verbosity;

#endif

