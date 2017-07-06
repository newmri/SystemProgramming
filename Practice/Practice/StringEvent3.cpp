#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam);
unsigned int WINAPI CountThreadFunction(LPVOID lpParam);

typedef struct _SynchString {
	TCHAR string[100];
	HANDLE hEvent;
	HANDLE hMutex;
}SynchString;

SynchString gSyncString;
int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hThread[2];
	DWORD dwThreadID[2];

	gSyncString.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	gSyncString.hMutex = CreateMutex(NULL, FALSE, NULL);
	if (gSyncString.hEvent == NULL || gSyncString.hMutex == NULL) {
		_fputts(_T("Kernel object creation error \n"), stdout);
		return -1;
	}

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, OutputThreadFunction, NULL, 0, (unsigned*)&dwThreadID[0]);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, CountThreadFunction, NULL, 0, (unsigned*)&dwThreadID[1]);

	if (hThread[0] == 0 || hThread[1] == 0) {
		_fputts(_T("Thread creation error \n"), stdout);
		return -1;
	}

	_fputts(_T("Insert string: "), stdout);
	_fgetts(gSyncString.string, 30, stdin);

	SetEvent(gSyncString.hEvent);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	CloseHandle(gSyncString.hEvent);
	CloseHandle(gSyncString.hMutex);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	return 0;
}


unsigned int WINAPI OutputThreadFunction(LPVOID lpParam)
{
	WaitForSingleObject(gSyncString.hEvent, INFINITE);
	WaitForSingleObject(gSyncString.hMutex, INFINITE);

	_fputts(_T("Output string: "), stdout);
	_fputts(gSyncString.string, stdout);

	ReleaseMutex(gSyncString.hMutex);
	return 0;
}


unsigned int WINAPI CountThreadFunction(LPVOID lpParam)
{
	WaitForSingleObject(gSyncString.hEvent, INFINITE);
	WaitForSingleObject(gSyncString.hMutex, INFINITE);

	_tprintf(_T("output string length: %d \n"), _tcslen(gSyncString.string) - 1);

	ReleaseMutex(gSyncString.hMutex);
	return 0;
}