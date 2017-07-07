#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

#define MAX_BUFFER_SIZE 1024

void Type(TCHAR* fileName)
{
	TCHAR StringBuffer[MAX_BUFFER_SIZE];

	FILE* filePtr = _tfopen(fileName, _T("rt"));
	if (filePtr == nullptr) {
		_tprintf(_T("%s does not exist.. \n"), fileName);
		return;
	}
	while (_fgetts(StringBuffer, MAX_BUFFER_SIZE, filePtr)) _fputts(StringBuffer, stdout);

	_fputts(_T("\n"), stdout);
}

int _tmain(int argc, TCHAR* argv[])
{
	if (argc < 2) return -1;
	Type(argv[1]);

	return 0;
}
