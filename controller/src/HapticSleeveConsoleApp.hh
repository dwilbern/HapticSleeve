
#ifndef __Haptic_Sleeve_Console_App_H_
#define __Haptic_Sleeve_Console_App_H_

int main(int argc, char *argv[]);
void PrintHelpDialog();

// 0 means print nothing
// 1 means print error messages only
// 2 means print error messages and status information only
// 3 means print error messages, status information, and debugging information
int verbosity;

#endif

