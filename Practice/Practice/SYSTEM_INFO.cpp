#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	SYSTEM_INFO si;
	DWORD allocGranularity;
	DWORD pageSize;

	GetSystemInfo(&si);
	pageSize = si.dwPageSize;

	allocGranularity = si.dwAllocationGranularity;

	_tprintf(_T("Page size: %u KBytes \n"), pageSize / 1024);
	_tprintf(_T("allocation granularity: %u KBytes \n"), allocGranularity / 1024);

	return 0;
}