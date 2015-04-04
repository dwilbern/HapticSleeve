
#include "HapticSleeveModel.hh"
#include "HapticSleeveInterface.hh"

HapticSleeveInterface::HapticSleeveInterface() {
	model = new HapticSleeveModel();
}

HapticSleeveInterface::~HapticSleeveInterface() {
	delete model;
}

