#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define WORK_MAX 10000
#define THREAD_MAX 50

typedef void (*WORK) (void);

DWORD AddWorkToPool(WORK work);
WORK GetWorkFromPool(void);
DWORD MakeThreadToPool(DWORD numOfThread);
void WorkerThreadFunction(LPVOID pParam);

typedef struct __WorkerThread
{
	HANDLE hThread;
	DWORD idThread;
}WorkerThread;

struct __ThreadPool
{
	WORK workList[WORK_MAX];

	WorkerThread workerThreadList[THREAD_MAX];
	HANDLE workerEventList[THREAD_MAX];

	DWORD idxOfCurrentWork;
	DWORD idxOfLastAddedWork;

	DWORD threadIdx; // Num of thread in pool
}gThreadPool;

static HANDLE mutex = NULL;

void InitMutex(void)
{
	mutex = CreateMutex(NULL, FALSE, NULL);
}

void DeInitMutex(void)
{
	BOOL ret = CloseHandle(mutex);
}

void AcquireMutex(void)
{
	DWORD ret = WaitForSingleObject(mutex, INFINITE);
	if (ret == WAIT_FAILED) _tprintf(_T("Error Occur! \n"));
}

void ReleaseMutex(void)
{
	BOOL ret = ReleaseMutex(mutex);
	if (ret == 0) _tprintf(_T("Error Occur! \n"));
}

DWORD AddWorkToPool(WORK work)
{
	AcquireMutex();
	if (gThreadPool.idxOfLastAddedWork >= WORK_MAX) {
		_tprintf(_T("AddWorkToPool fail! overflow \n"));
		return NULL;
	}

	// Add Work
	gThreadPool.workList[gThreadPool.idxOfLastAddedWork++] = work;

	// After Adding work, wake up all threads waiting in pool
	for (DWORD i = 0; i < gThreadPool.threadIdx; ++i) SetEvent(gThreadPool.workerEventList[i]);

	ReleaseMutex();
	return 1;
}

WORK GetWorkFromPool(void)
{
	WORK work = NULL;

	AcquireMutex();

	if (!(gThreadPool.idxOfCurrentWork < gThreadPool.idxOfLastAddedWork)) {
		ReleaseMutex();
		return NULL;
	}

	work = gThreadPool.workList[gThreadPool.idxOfCurrentWork++];
	ReleaseMutex();

	return work;
}

DWORD MakeThreadToPool(DWORD numOfThread)
{
	InitMutex();
	DWORD capacity = WORK_MAX - (gThreadPool.threadIdx);

	if (capacity < numOfThread) numOfThread = capacity;

	for (DWORD i = 0; i < numOfThread; ++i) {
		DWORD idThread;
		HANDLE hThread;

		gThreadPool.workerEventList[gThreadPool.threadIdx] = CreateEvent(NULL, FALSE, FALSE, NULL);

		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThreadFunction, (LPVOID)gThreadPool.threadIdx, 0, &idThread);

		gThreadPool.workerThreadList[gThreadPool.threadIdx].hThread = hThread;
		gThreadPool.workerThreadList[gThreadPool.threadIdx].idThread = idThread;

		gThreadPool.threadIdx++;
	}
	return numOfThread;
}

void WorkerThreadFunction(LPVOID pParam)
{
	WORK workFunction;
	HANDLE event = gThreadPool.workerEventList[(DWORD)pParam];

	while (true) {
		workFunction = GetWorkFromPool();
		if (workFunction == NULL) {
			WaitForSingleObject(event, INFINITE);
			continue;
		}
		workFunction();
	}
}

void TestFunction()
{
	static int i = 0; // Sync skip
	++i;

	_tprintf(_T("Good Test --%d : Processing thread: %d--\n\n"), i, GetCurrentThreadId());
}

int _tmain(int argc, TCHAR* argv[])
{
	MakeThreadToPool(3);

	for (int i = 0; i < 100; ++i) AddWorkToPool(TestFunction);

	Sleep(50000); // to avoid to be deleted main thread first
	return 0;
}
