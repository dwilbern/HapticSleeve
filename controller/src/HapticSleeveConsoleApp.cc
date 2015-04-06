
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>

#include "HapticSleeveConsoleApp.hh"
#include "HapticSleeveInterface.hh"

int main(int argc, char *argv[]) {

	HapticSleeveInterface *sleeve = new HapticSleeveInterface();
	std::string s;

#ifdef DEBUG
	verbosity = 3;
#else
	verbosity = 2;
#endif

	if(verbosity >= 1)
		printf("Haptic Sleeve CLI Controller App by Daniel Wilbern, dwilbern@nmu.edu\n");	

	while(std::cin) {
		std::cout << "? ";
		getline(std::cin,s);
		std::istringstream iss(s);

		while(iss) {
			iss >> s;

			if(s == "")
				break;
			else if(s == "exit" || s == "quit" || s == "q")
				goto end;
			else if(s == "help" || s == "h")
				PrintHelpDialog();
			else if(s == "verbosity" || s == "v") {
				iss >> verbosity;
			} else if(s == "connect" || s == "c") {
				iss >> s;
			//	sleeve->Connect("/dev/ttyACM0");
				sleeve->Connect(s.c_str());
			} else if(s == "disconnect" || s == "d")
				sleeve->Disconnect();
			else if(s == "calibrate" || s == "l")
				sleeve->Calibrate();
			else if(s == "echo" || s == "e") {
				iss >> s;
				sleeve->Echo(s.c_str());
			}
			else {
				std::cout << "Invalid command " << s << '.' << std::endl;	
				PrintHelpDialog();
			}
			s = "";
		}
	}

end:
	delete sleeve;
	return 0;
}

void PrintHelpDialog() {

	printf("Haptic Sleeve CLI Controller App by Daniel Wilbern, dwilbern@nmu.edu\n");	
	printf("Commands:\n");
	printf("\t(h)elp: print this dialog\n");
	printf("\t(c)onnect <port name>: open a connection to the haptic sleeve at the given serial port location (e.g. \"COM3\")\n");
	printf("\t(d)isconnect: close the serial connection to the haptic sleeve\n");
	printf("\tca(l)ibrate: calibrate the sleeve's maximum and minimum feedback values\n");
	printf("\t(v)erbosity <level>: change the verbosity of output\n");
	printf("\t(e)cho <string>: test the serial connection with an echo\n");
	printf("\texit, (q)uit: close this program\n");
}

