#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUF_SIZE 1024
int CommToClient(HANDLE);

int _tmain(int argc, TCHAR* argv[])
{
	LPTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");
	HANDLE hPipe;

	while (true) {
		hPipe = CreateNamedPipe(
			pipeName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			BUF_SIZE / 2, // Output buffer size
			BUF_SIZE / 2, // Input buffer size
			20000, NULL);
		if (hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(_T("CreateNamedPipe failed"));
			return -1;
		}

		BOOL IsSuccess;
		IsSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		if (IsSuccess) CommToClient(hPipe);
		else CloseHandle(hPipe);
	}
	return 1;
}

int CommToClient(HANDLE hPipe)
{
	TCHAR fileName[MAX_PATH];
	TCHAR dataBuf[BUF_SIZE];
	BOOL IsSuccess;
	DWORD fileNameSize;

	IsSuccess = ReadFile(hPipe, fileName, MAX_PATH * sizeof(TCHAR), &fileNameSize, NULL);
	if (!IsSuccess || fileNameSize == 0) {
		_tprintf(_T("Pipe read message error! \n"));
		return -1;
	}

	FILE* filePtr = _tfopen(fileName, _T("r"));
	if (filePtr == NULL) {
		_tprintf(_T("File open fault! \n"));
		return -1;
	}

	OVERLAPPED overlappedInst;
	memset(&overlappedInst, 0, sizeof(overlappedInst));
	overlappedInst.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	DWORD bytesWritten = 0, bytesRead = 0;
	DWORD bytesWrite = 0, bytesTransfer = 0;

	while (!feof(filePtr)) {
		bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);
		bytesWrite = bytesRead;
		IsSuccess = WriteFile(hPipe, dataBuf, bytesWrite, &bytesWritten, &overlappedInst);
		if (!IsSuccess && GetLastError() != ERROR_IO_PENDING) {
			_tprintf(_T("Pipe write message error! \n"));
			break;
		}

		// Chance to do something else
		WaitForSingleObject(overlappedInst.hEvent, INFINITE);
		GetOverlappedResult(hPipe, &overlappedInst, &bytesTransfer, FALSE);
		_tprintf(_T("Transferred data size: %u \n"), bytesTransfer);
	}

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
	return 1;
	
}