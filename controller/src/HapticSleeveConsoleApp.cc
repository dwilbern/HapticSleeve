
#include <iostream>
#include <string>
#include <cstdio>

#include "HapticSleeveConsoleApp.hh"
#include "HapticSleeveInterface.hh"

int main(int argc, char *argv[]) {

	HapticSleeveInterface *sleeve = new HapticSleeveInterface();
	std::string s;

	while(true) {
		std::cout << "? ";
		getline(std::cin,s);

		if(s == "") continue;
		else if (s == "exit" || s == "quit") break;
		else if(s == "help") PrintHelpDialog();
		else {
			std::cout << "Invalid command " << s << '.' << std::endl;	
			PrintHelpDialog();
		}
	}

	delete sleeve;
	return 0;
}

void PrintHelpDialog() {

	printf("Haptic Sleeve CLI Controller App by Daniel Wilbern, dwilbern@nmu.edu\n");	
	printf("Commands:\n");
	printf("\thelp: print this dialog\n");
	printf("\tconnect <port name>: open a connection to the haptic sleeve at the given serial port location (e.g. \"COM3\")\n");
	printf("\tdisconnect: close the serial connection to the haptic sleeve\n");
	printf("\texit: close this program\n");
}

