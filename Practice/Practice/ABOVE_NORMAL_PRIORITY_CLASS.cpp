#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	fgetchar();

	STARTUPINFO siNORMAL = { 0, };
	PROCESS_INFORMATION piNORMAL;
	TCHAR commandOne[] = _T("NORMAL_PRIORITY_CLASS.exe");

	STARTUPINFO siBELOW = { 0, };
	PROCESS_INFORMATION piBELOW;
	TCHAR commandTwo[] = _T("BELOW_NORMAL_PRIORITY_CLASS.exe");

	siNORMAL.cb = sizeof(siNORMAL);
	siBELOW.cb = sizeof(siBELOW);


	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	CreateProcess(NULL, commandOne, NULL, NULL, TRUE, 0, NULL, NULL, &siNORMAL, &piNORMAL);
	CreateProcess(NULL, commandTwo, NULL, NULL, TRUE, 0, NULL, NULL, &siBELOW, &piBELOW);

	while (true) {
		//for (DWORD i = 0; i < 10000; ++i) for (DWORD i = 0; i < 10000; ++i) // Busy Waiting!!
		Sleep(10);

			_fputts(_T("ABOVE_NORMAL_PRIORITY_CLASS Process \n"), stdout);
	}

	return 0;
}