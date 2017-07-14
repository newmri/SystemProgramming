#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

VOID CALLBACK TimerAPCProc(LPVOID lpArg, DWORD timerLowVal, DWORD timerHighVal);

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -100000000;

	hTimer = CreateWaitableTimer(NULL, FALSE, _T("WaitableTimer"));

	if (!hTimer) {
		_tprintf(_T("CreateWaitableTimer failed (%d) \n"), GetLastError());
		return -1;
	}

	_tprintf(_T("Waiting for 10 seconds... \n"));

	SetWaitableTimer(hTimer, &liDueTime, 5000, TimerAPCProc, _T("Timer was signaled. \n"), FALSE);

	while (true) SleepEx(INFINITE, TRUE);
	
	return 0;
}

VOID CALLBACK TimerAPCProc(LPVOID lpArg, DWORD timerLowVal, DWORD timerHighVal)
{
	_tprintf(_T("%s"), (TCHAR*)lpArg);
	MessageBeep(MB_ICONEXCLAMATION);
}