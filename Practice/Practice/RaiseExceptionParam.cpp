#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STATUS_DEFAULT_USER_DEFINED_EXPT ((DWORD)0xE0000008L)

void SoftwareException();
DWORD FilterFunction(LPEXCEPTION_POINTERS exptPtr);

int _tmain(int argc, TCHAR* argv[])
{
	SoftwareException();
	_tprintf(_T("End of the _tmain \n"));

	return 0;
}

void SoftwareException()
{

	TCHAR* exptString[] = { _T("One"),_T("Two"),_T("Three") };

	__try
	{
		RaiseException(STATUS_DEFAULT_USER_DEFINED_EXPT, 0, 3, (ULONG_PTR*)exptString);
	}
	__except (FilterFunction(GetExceptionInformation()))
	{
		DWORD exptType = GetExceptionCode();
		_tprintf(_T("Recv: exception code: 0x%x \n"), exptType);
	}

}

DWORD FilterFunction(LPEXCEPTION_POINTERS exptPtr)
{
	PEXCEPTION_RECORD exptRecord = exptPtr->ExceptionRecord;

	switch (exptRecord->ExceptionCode) {
	case STATUS_DEFAULT_USER_DEFINED_EXPT:
		_tprintf(_T("exception \n"));
		_tprintf(_T("Code: 0x%x \n"), exptRecord->ExceptionCode);
		_tprintf(_T("flags: %d \n"), exptRecord->ExceptionFlags);
		_tprintf(_T("param num: %d \n"), exptRecord->NumberParameters);
		_tprintf(_T("string one: %s \n"), (TCHAR*)exptRecord->ExceptionInformation[0]);
		break;

	default:
		break;
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
