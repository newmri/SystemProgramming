#include <stdio.h>
#include <tchar.h>

#include "Calculator.h"

#pragma comment(lib,"DLL.lib")

int _tmain(int argc, TCHAR* argv[])
{
	_tprintf(_T("%d \n"), Add(1, 2));
}
