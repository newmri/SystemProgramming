#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define CREATE_DIRECTORY	1

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR dirRelativePath[] = _T("GoodDirectoryOne");
	TCHAR dirFullPath[] = _T("S:\\KPU\4-Summer\\System\\Practice\\Debug\\GoodDirectoryTwo");

#if CREATE_DIRECTORY
	CreateDirectory(dirRelativePath, NULL);
	CreateDirectory(dirFullPath, NULL);
#else
	RemoveDirectory(dirRelativePath);
	RemoveDirectory(dirFullPath);
#endif

	return 0;
}