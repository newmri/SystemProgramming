#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

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

	SetWaitableTimer(hTimer, &liDueTime, 5000, NULL, NULL, FALSE);

	while (true) {
		WaitForSingleObject(hTimer, INFINITE);
		_tprintf(_T("Timer was signaled. \n"));
		MessageBeep(MB_ICONQUESTION);
	}
	return 0;
}