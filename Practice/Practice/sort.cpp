#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define MAX_STR 100
#define MAX_STR_LEN 256

void SortString();

int _tmain(int argc, TCHAR* argv[])
{
	SortString();
	return 0;
}

void SortString()
{
	TCHAR Str[MAX_STR][MAX_STR_LEN];
	int nStr{};
	for (nStr; nStr < MAX_STR; ++nStr) {
		TCHAR* IsEOF = _fgetts(Str[nStr], MAX_STR_LEN, stdin);
		if (IsEOF == NULL) break;
	}

	TCHAR TempStr[MAX_STR_LEN];

	for (int i = 0; i < nStr; ++i) {
		for (int j = nStr - 1; j > i; --j) {
			if (_tcscmp(Str[j - 1], Str[j]) > 0) {
				_tcscpy(TempStr, Str[j - 1]);
				_tcscpy(Str[j - 1], Str[j]);
				_tcscpy(Str[j], TempStr);
			}
		}
	}

	for (int i = 0; i < nStr; ++i) _fputts(Str[i], stdout);
	system("pause");
}

