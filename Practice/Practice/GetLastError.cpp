#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain()
{
	HANDLE hFile = CreateFile(_T("ABC.DAT"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) _tprintf(_T("Error Code: %d \n", GetLastError()));

	return 0;
}