#include <stdio.h>
#include <tchar.h>
#include "swap.h"

#pragma comment(lib,"StaticLibrary.lib")

int _tmain(int argc, TCHAR* argv[])
{
	int a = 10;
	int b = 20;
	swap(&a, &b);

	_tprintf(_T("swap %d %d \n"), a, b);
}
