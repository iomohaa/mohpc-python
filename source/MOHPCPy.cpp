// MOHPCPy.cpp : Defines the entry point for the application.
//

#include "MOHPCPy.h"
#include <Windows.h>
#include <iostream>

int main()
{
	STARTUPINFO StartupInfo{0};
	PROCESS_INFORMATION ProcessInformation{0};

	CreateProcess(
		NULL,
		TEXT("python.exe test.py"),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInformation
	);

	// Wait for the process to exit
	WaitForSingleObject(ProcessInformation.hProcess, INFINITE);

	std::cout << "Python script done." << std::endl;
	// Wait for enter
	getchar();

	return 0;
}
