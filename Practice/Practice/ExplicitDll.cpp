#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include "SwapDll.h"

typedef void(*SWAP_FUNC) (int *, int *);

int _tmain(int argc, TCHAR* argv[])
{
	HMODULE hinstLib;
	SWAP_FUNC SwapFunction;
	hinstLib = LoadLibrary(_T("DLL"));
	if (hinstLib == NULL) {
		_tprintf(_T("LoadLibrary fail! \n"));
		return -1;
	}
	SwapFunction = (SWAP_FUNC)GetProcAddress(hinstLib, "swap");
	if (SwapFunction == NULL) {
		_tprintf(_T("GetProcAddress fail! \n"));
		return -1;
	}

	BOOL IsSuccess = FreeLibrary(hinstLib);

	int a = 10;
	int b = 20;
	swap(&a, &b);

	_tprintf(_T("swap %d %d \n"), a, b);
}

