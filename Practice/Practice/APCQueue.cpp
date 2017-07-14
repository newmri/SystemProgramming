#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

VOID CALLBACK APCProc(ULONG_PTR);

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hThread = GetCurrentThread();
	for (ULONG_PTR i = 1; i <= 5; ++i) QueueUserAPC(APCProc, hThread, i);

	Sleep(5000);
	SleepEx(INFINITE, TRUE);
	return 0;
}

VOID CALLBACK APCProc(ULONG_PTR dwParam)
{
	_tprintf(_T("Aynchronous procedure call num %u \n"), (DWORD)dwParam);
}
