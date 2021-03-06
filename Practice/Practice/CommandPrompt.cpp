#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>
#include <TlHelp32.h>

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
void ShowProcessList();
DWORD GetProcessID(TCHAR*);
void KillProcess();
void Sort();
void Type();
BOOL CopyDirectoryFiles(WIN32_FIND_DATA, TCHAR*, TCHAR*);
void XCopy(TCHAR*, TCHAR*);
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
	else if (!_tcscmp(cmdTokenList[0], _T("lp"))) ShowProcessList();
	else if (!_tcscmp(cmdTokenList[0], _T("kp"))) KillProcess();
	else if (!_tcscmp(cmdTokenList[0], _T("sort"))) Sort();
	else if (!_tcscmp(cmdTokenList[0], _T("type"))) Type();
	else if (!_tcscmp(cmdTokenList[0], _T("xcopy"))) XCopy(cmdTokenList[1], cmdTokenList[2]);

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

void ShowProcessList()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot error! \n"));
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		_tprintf(_T("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return;
	}

	do {
		_tprintf(_T("%25s %5d \n"), pe32.szExeFile, pe32.th32ProcessID);
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

}
DWORD GetProcessID(TCHAR* a_fileName)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot error! \n"));
		return 0;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		_tprintf(_T("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return 0;
	}
	do {
		if (!_tcscmp(pe32.szExeFile, a_fileName)) {
			CloseHandle(hProcessSnap);
			return pe32.th32ProcessID;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return 0;

}
void KillProcess()
{
	DWORD processID;
	processID = GetProcessID(cmdTokenList[1]);
	if (processID != 0) {
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
		TerminateProcess(hProcess, -1);
	}
	else {
		_fputts(_T("Killing process is Faild. \n"), stdout);
		_tprintf(_T("May be %s is doesen't exist. Scrutinize name again \n"), cmdTokenList[1]);
	}
}

void Sort()
{
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	bool ret;
	if (!_tcscmp(cmdTokenList[1], _T(">"))) {
		SECURITY_ATTRIBUTES fileSec = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };

		HANDLE hFile = CreateFile(cmdTokenList[2], GENERIC_WRITE, FILE_SHARE_READ, &fileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		si.hStdOutput = hFile;
		si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
		si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		si.dwFlags |= STARTF_USESTDHANDLES;

		ret = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(hFile);

	}

	else {
		ret = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (!ret) { _tprintf(ERROR_CMD, cmdTokenList[0]); return; }
}

void Type()
{
	TCHAR command[STR_LEN]{ 0, };
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	bool ret;

	if (!_tcscmp(cmdTokenList[2], _T("|"))) {
		HANDLE hReadPipe, hWritePipe;

		SECURITY_ATTRIBUTES pipeSA = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };

		CreatePipe(&hReadPipe, &hWritePipe, &pipeSA,0);

		si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
		si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		si.hStdOutput = hWritePipe;
		si.dwFlags |= STARTF_USESTDHANDLES;

		_stprintf(command, _T("%s %s"), _T("Type.exe"), cmdTokenList[1]);
		ret = CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		CloseHandle(hWritePipe);

		STARTUPINFO siSort = { 0, };
		PROCESS_INFORMATION piSort;
		siSort.cb = sizeof(siSort);

		siSort.hStdInput = hReadPipe;
		siSort.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		siSort.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		siSort.dwFlags |= STARTF_USESTDHANDLES;

		ret = CreateProcess(NULL, cmdTokenList[3], NULL, NULL, TRUE, 0, NULL, NULL, &siSort, &piSort);

		CloseHandle(piSort.hThread);
		CloseHandle(hReadPipe);

		WaitForSingleObject(pi.hProcess, INFINITE);
		WaitForSingleObject(piSort.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(piSort.hProcess);
	}
	else {
		_stprintf(command, _T("%s %s"), _T("Type.exe"), cmdTokenList[1]);
		ret = CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	if (!ret) { _tprintf(ERROR_CMD, cmdTokenList[0]); return; }
}

BOOL CopyDirectoryFiles(WIN32_FIND_DATA a_fileData, TCHAR* a_original, TCHAR* a_target)
{
	BOOL IsSuccess = NULL;

	if (!_tcscmp(a_fileData.cFileName, _T(".")) || !_tcscmp(a_fileData.cFileName, _T("..")));
	else if (a_fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		TCHAR subSourceDir[MAX_PATH];
		TCHAR subDestDir[MAX_PATH];

		_stprintf(subSourceDir, _T("%s\\%s"), a_original, a_fileData.cFileName);
		_stprintf(subDestDir, _T("%s\\%s"), a_target, a_fileData.cFileName);
		CreateDirectory(subDestDir, NULL);
		XCopy(subSourceDir, subDestDir);
	}
	else {
		TCHAR sourceFile[MAX_PATH];
		TCHAR destFile[MAX_PATH];

		_tcscpy(sourceFile, a_original);
		_tcscpy(destFile, a_target);

		_stprintf(sourceFile, _T("%s\\%s"), sourceFile, a_fileData.cFileName);
		_stprintf(destFile, _T("%s\\%s"), destFile, a_fileData.cFileName);

		IsSuccess = CopyFile(sourceFile, destFile, FALSE);
		if (IsSuccess == 0) return FALSE;
	}

	return TRUE;
}
void XCopy(TCHAR* a_original, TCHAR* a_target)
{

	WIN32_FIND_DATA fileData;
	BOOL IsSuccess = NULL;

	TCHAR firstFFStr[MAX_PATH];
	_stprintf(firstFFStr, _T("%s\\%s"), a_original, _T("*"));

	HANDLE searchHandle = FindFirstFile(firstFFStr, &fileData);
	if (searchHandle == INVALID_HANDLE_VALUE) return;
	else CopyDirectoryFiles(fileData, a_original, a_target);

	while (true) {
		if (!FindNextFile(searchHandle, &fileData)) break;
		else {
			IsSuccess = CopyDirectoryFiles(fileData, a_original, a_target);
			if (IsSuccess == FALSE) break;
		}
	}
	FindClose(searchHandle);
	_fputts(_T("Copy Success! \n"),stdout);
	return;
}