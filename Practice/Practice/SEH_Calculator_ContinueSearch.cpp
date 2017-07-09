#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

enum { DIVE = 1, MUL, ADD, MIN, EXIT };

DWORD ShowMenu();
BOOL Calculator();
DWORD FilterFunction(DWORD);

int num1, num2, result;

int _tmain(int argc, TCHAR* argv[])
{
	BOOL state;

	__try
	{
		do {
			state = Calculator();
		} while (state == TRUE);
	}

	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		_tprintf(_T("End of excution! \n"));
	}
	return 0;
}

DWORD ShowMenu()
{
	DWORD sel;

	_fputts(_T("-----Menu----- \n"), stdout);
	_fputts(_T("num 1: Divide \n"), stdout);
	_fputts(_T("num 2: Multiple \n"), stdout);
	_fputts(_T("num 3: Add \n"), stdout);
	_fputts(_T("num 4: Minus \n"), stdout);
	_fputts(_T("num 5: Exit \n"), stdout);
	_fputts(_T("SELECTION >>"), stdout);
	_tscanf(_T("%d"), &sel);

	return sel;
}

BOOL Calculator()
{
	DWORD sel;

	sel = ShowMenu();
	if (sel == EXIT) return FALSE;

	_fputts(_T("Input num1 num2: "), stdout);
	_tscanf(_T("%d %d"), &num1, &num2);

	__try
	{
		switch (sel) {
		case DIVE:
			result = num1 / num2;
			_tprintf(_T("%d / %d = %d \n\n"), num1, num2, result);
			break;
		case MUL:
			result = num1 * num2;
			_tprintf(_T("%d * %d = %d \n\n"), num1, num2, result);
			break;
		case ADD:
			result = num1 + num2;
			_tprintf(_T("%d + %d = %d \n\n"), num1, num2, result);
			break;
		case MIN:
			result = num1 - num2;
			_tprintf(_T("%d - %d = %d \n\n"), num1, num2, result);
			break;
		}
	}

	__except (FilterFunction(GetExceptionCode()))
	{
		_tprintf(_T("__except block... \n\n"));
	}

	return TRUE;
}

DWORD FilterFunction(DWORD exptType)
{
	switch (exptType) {
	case EXCEPTION_ACCESS_VIOLATION:
		_tprintf(_T("Access violation \n\n"));
		return EXCEPTION_EXECUTE_HANDLER;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		_tprintf(_T("Wrong number inserted. \n"));
		_tprintf(_T("Input second number again: "));
		_tscanf(_T("%d"), &num2);
		return EXCEPTION_CONTINUE_SEARCH;

	default:
		return EXCEPTION_EXECUTE_HANDLER;
	}
}