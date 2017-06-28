#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] = _T("'%s' is not excutable program. \n");

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");

int ReadToken();
int CmdProcessing(int);
TCHAR* StrLower(TCHAR*);
void Echo(int);
void Start(int);
int _tmain(int argc, TCHAR* argv[])
{
	// To input Korean language
	_tsetlocale(LC_ALL, _T("Korean"));

	if (argc > 1) {
		for (int i = 1; i < argc; ++i) _tcscpy(cmdTokenList[i - 1], argv[i]);
		CmdProcessing(argc - 1);
	}
	DWORD isExit;
	while (true) {
		int tokenNum = ReadToken();
		if (tokenNum == 0) continue;

		isExit = CmdProcessing(tokenNum);
		if (isExit) {
			_fputts(_T("Exit command. \n"), stdout); break;
		}
	}
	return 0;
}


int ReadToken()
{
	_fputts(_T("Best command prompt>> "), stdout);
	_getts_s(cmdString, sizeof(cmdString));

	TCHAR* token = _tcstok(cmdString, seps);

	int tokenNum = 0;
	while (token != NULL) {
		//_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		_tcscpy(cmdTokenList[tokenNum++], token);
		token = _tcstok(NULL, seps);
	}

	return tokenNum;
}

int CmdProcessing(int a_tokenNum)
{
	if (!_tcscmp(cmdTokenList[0], _T("exit"))) return TRUE;
	else if (!_tcscmp(cmdTokenList[0], _T("echo"))) Echo(a_tokenNum);
	else if (!_tcscmp(cmdTokenList[0], _T("start"))) Start(a_tokenNum);
	else {
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi;
		si.cb = sizeof(si);
		BOOL ret = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (!ret) { _tprintf(ERROR_CMD, cmdTokenList[0]); return 0; }
	}

	return 0;
}

TCHAR* StrLower(TCHAR* pStr)
{
	TCHAR* ret = pStr;
	while (*pStr) {
		if (_istupper(*pStr)) *pStr = _totlower(*pStr);
		pStr++;
	}

	return ret;
}

void Echo(int a_tokenNum)
{
	TCHAR str[STR_LEN]{ 0, };
	for (int i = 1; i < a_tokenNum; ++i) {
		if (i == 1) _stprintf(str, _T("%s"), cmdTokenList[i]);
		else _stprintf(str, _T("%s %s"), str, cmdTokenList[i]);
	}
	_tprintf(_T("%s \n"), str);
}

TCHAR* GetEcho(int a_tokenNum)
{
	TCHAR str[STR_LEN]{ 0, };
	TCHAR command[STR_LEN]{ 0, };
	for (int i = 1; i < a_tokenNum; ++i) {
		if (i == 1) _stprintf(str, _T("%s"), cmdTokenList[i]);
		else _stprintf(str, _T("%s %s"), str, cmdTokenList[i]);
	}

	_stprintf(command, _T("%s %s"), _T("CommandPrompt.exe"), str);

	return command;
}
void Start(int a_tokenNum)
{
	TCHAR command[STR_LEN]{ 0, };
	bool ret{ false };
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	if (a_tokenNum > 1) _tcscpy(command, GetEcho(a_tokenNum));
	else _stprintf(command, _T("%s"), _T("CommandPrompt.exe"));

	ret = CreateProcess(NULL, command, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (!ret) { _tprintf(ERROR_CMD, cmdTokenList[0]); return; }

}