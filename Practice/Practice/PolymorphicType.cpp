#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

UINT CalDistance(UINT a, UINT b)
{
	return a - b;
}
UINT_PTR CalDistance2(UINT_PTR a, UINT_PTR b)
{
	return a - b;
}

int _tmain()
{
	{
		INT val1 = 10;
		INT val2 = 20;

		_tprintf(_T("Position %u, %u \n"), (UINT)&val1, (UINT)&val2);
		_tprintf(_T("Distance : %u \n"), CalDistance((UINT)&val1, (UINT)&val2));

	}
	{

		INT32 val1 = 10;
		INT32 val2 = 20;
		_tprintf(_T("Position %u, %u \n"), (UINT_PTR)&val1, (UINT_PTR)&val2);
		_tprintf(_T("Distance : %u \n"), CalDistance2((UINT_PTR)&val1, (UINT_PTR)&val2));

	}
	return 0;
}