#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

void SortIntData(int* pSortArr, int num);

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hFile = CreateFile(_T("data.dat"), GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) _tprintf(_T("Could not open file."));

	HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 1024 * 10, NULL);
	if (hMapFile == NULL) _tprintf(_T("Could not create map of file. \n"));

	int* pWrite = (int*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (pWrite == NULL) _tprintf(_T("Could not map view of file. \n"));

	pWrite[0] = 1;
	pWrite[1] = 5;
	pWrite[2] = 3;

	SortIntData(pWrite, 3);

	_tprintf(_T("%d %d %d \n"), pWrite[0], pWrite[1], pWrite[2]);

	UnmapViewOfFile(pWrite);
	CloseHandle(hMapFile);

	CloseHandle(hFile);
	_tprintf(_T("End ofprocess \n"));

	return 0;
}

void SortIntData(int* pSortArr, int num)
{
	int temp;

	for (int i = 0; i < num - 1; ++i) {
		for (int j = 1; j < num; ++j) {
			if (pSortArr[j - 1] > pSortArr[j]) {
				temp = pSortArr[j - 1];
				pSortArr[j - 1] = pSortArr[j];
				pSortArr[j] = temp;
			}
		}
	}
}
