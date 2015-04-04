
#ifndef __Haptic_Sleeve_Interface_H_
#define __Haptic_Sleeve_Interface_H_

class HapticSleeveModel;

class HapticSleeveInterface {

	public:
		HapticSleeveInterface();
		~HapticSleeveInterface();

	private:
		HapticSleeveModel *model;
};

#endif

