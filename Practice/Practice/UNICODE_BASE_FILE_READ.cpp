#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LEN 100

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fileName[] = _T("data.txt");
	TCHAR fileData[STRING_LEN];

	HANDLE hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		_tprintf(_T("File open fault! \n"));
		return -1;
	}

	DWORD numOfByteRead = 0;
	ReadFile(hFile, fileData, sizeof(fileData), &numOfByteRead, NULL);

	fileData[numOfByteRead / sizeof(TCHAR)] = 0;
	_tprintf(_T("read data size: %u \n"), numOfByteRead);
	_tprintf(_T("read string: %s \n"), fileData);
	CloseHandle(hFile);

	return 0;
}