#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUFSIZE 1024

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR value[BUFSIZE];

	if (GetEnvironmentVariable(_T("Good"), value, BUFSIZE) > 0) _tprintf(_T("[%s = %s] \n"), _T("Good"), value);

	Sleep(10000);
	return 0;
}