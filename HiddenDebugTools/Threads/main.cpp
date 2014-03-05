/*
* This project is a playground to familiarize yourself with debugging tools related to threads.
* Additionally to allowing you to play around with debugging tools for threads, this program shows 
* how to programmatically set names for your threads. This is useful when debugging threads because it allows
* you to differentiate threads by their name in the "Threads" Window.
*
* This program spawns two threads that naively determine if the same large integer is prime.
* These two threads run the same function, named "PrimalityTestWorker".
*
* Run this program in the "Debug" configuration and set a breakpoint at the top of the "PrimalityTestWorker".
* Then, go to Debug -> Windows -> Threads.
*
* Observe that al threads have the same Process ID.
*
* Use "Step Over" (F10) and observe how the "Next Statement" arrow (the yellow one) "jumps" from one thread to another.
*
* Play around with "Freeze" and "Thaw" for each thread. Observer how it can suspend a thread and resume it.
*
* Set a breakpoint in PrimalityTestWorker's loop, right click on it and click "Filter". In the "Filter" text box,
* enter "ThreadId = X" where X is the ID of one of the two worker threads that you want the breakpoint to be valid for.
* Resume the program execution and observe that the breakpoint is hit only for this thread, while the other one runs independently.
*
*/

#include <Windows.h>
#include <process.h>

#include <cmath>

#include <string>
#include <sstream>

// max signed int - 1 is the highest prime number that can be represented with an unsigned int
// It will be usefull to generate a bit of load on the cpu when running our dummy "primality test" 
// code on several threads...
#define PRIME_NUMBER (1L << 31L) - 1L

/*
* This data structure is what is passed to each thread worker.
* It contains the data that each thread will use to perform its work.
* "numberToTest" is the number we want to test for primality.
* "threadDebugName" is the name that we set for a given thread, this name appears in the "Threads"
* debug window, which helps debugging a lot.
*/
typedef struct ThreadData_s
{
	long numberToTest;
	const char* threadDebugName;
} ThreadData_t;

//-----------------------------------------------------------------------------
// Returns true if the OS running this program supports the "new" way of getting a thread's id,
// false otherwise
bool OSSupportsGetThreadId()
{
	OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    // Checks if OS version is at least 5.2, aka Windows Server 2003
	return  ( (osvi.dwMajorVersion > 5) ||
			((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 2)) );    
}

//-----------------------------------------------------------------------------
// Wrapper around the "new" way of getting a thread id
// We're loading the library explicitely so that our executable doesn't link against
// the GetThreadId function, otherwise we'd get a runtime exception on OS < Windows server 2003
typedef DWORD (WINAPI *GetThreadIdFuncPtr)(__in  HANDLE Thread);

bool MyGetThreadId(HANDLE threadHandle, unsigned long& outThreadId)
{
	HMODULE hLib = LoadLibrary(L"kernel32.dll");
	if (hLib != 0)
	{
		GetThreadIdFuncPtr funcPtr = (GetThreadIdFuncPtr) GetProcAddress(hLib, "GetThreadId");
		if (funcPtr != 0)
		{
			outThreadId = funcPtr(threadHandle);
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Wrapper around the "old" way of getting a thread id
// We're loading the library explicitely because there's no other way to do it...
typedef long (__stdcall *NtQueryInformationThread_PROC)(HANDLE thread,
														int infoclass, void *buf, long size, long *used);

typedef struct _THREAD_BASIC_INFORMATION {
	unsigned long ExitStatus;
	void *TebBaseAddress;
	ULONG UniqueProcessId;
	ULONG UniqueThreadId;
	unsigned long AffinityMask;
	unsigned long BasePriority;
	unsigned long DiffProcessPriority;
} THREAD_BASIC_INFORMATION;

//-----------------------------------------------------------------------------
bool GetThreadIdForOlderWindowsVersions(HANDLE hThread, unsigned long& outThreadId)
{
	unsigned long threadId = 0;
	THREAD_BASIC_INFORMATION tbi;
	HMODULE hLib = 0;
	NtQueryInformationThread_PROC hProc = 0;
	long used = 0, ret = 0;

	hLib = LoadLibrary(L"ntdll.dll");
	if (hLib != 0)
	{
		hProc = (NtQueryInformationThread_PROC) GetProcAddress(hLib, "NtQueryInformationThread");
		if (hProc != 0)
		{
			ret = hProc(hThread, 0, &tbi, sizeof(THREAD_BASIC_INFORMATION), &used);
			if (ret == 0)
			{
				threadId = tbi.UniqueThreadId;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		FreeLibrary(hLib);
	}
	else
	{
		return false;
	}

	outThreadId = threadId;
	return true;
}

// The implementation of SetThreadName is taken from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx 
//-----------------------------------------------------------------------------
typedef struct tagTHREADNAME_INFO
{
   DWORD	dwType;		// must be 0x1000
   LPCSTR	szName;		// pointer to name (in user addr space)
   DWORD	dwThreadID; // thread ID (-1=caller thread)
   DWORD	dwFlags;	// reserved for future use, must be zero
} THREADNAME_INFO;

void SetThreadName(DWORD dwThreadID, LPCSTR szThreadName)
{
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = szThreadName;
   info.dwThreadID = dwThreadID;
   info.dwFlags = 0;

   __try
   {
      RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
   }
   __except(EXCEPTION_CONTINUE_EXECUTION)
   {
   }
}

//-----------------------------------------------------------------------------
unsigned int PrimalityTestWorker(const long n)
{
	long  sqrtN = static_cast<long>(sqrt(static_cast<long double>(n)));
	for (long i = 2; i < sqrtN; ++i)
	{	
		std::wostringstream debugLog;
		debugLog << "Thread ID: ";
		debugLog << GetCurrentThreadId();
		debugLog << ", testing with factor: ";
		debugLog << i << std::endl;
		
		OutputDebugString(debugLog.str().c_str());
		
		if (n % i == 0)
		{			
			return 0;
		}
	}

	return 1;
}

//-----------------------------------------------------------------------------
unsigned int __stdcall PrimalityTestThread(void* params)
{
	ThreadData_t* threadData = static_cast<ThreadData_t*>(params);
	if (threadData)
	{
		// Setting the thread name in the current thread is the most reliable way,
		// and that's why we have a "threadDebugName" property in our ThreadData_t type.
		// The other way of setting it is from the main thread, unfortunately it doesn't work
		// reliably because we don't know when the thread that we create from the main thread 
		// will actually exist.
		// To verify this yourself, comment out the following SetThreadName statement,
		// run the program again and examine the "Threads" debug window. Most of the time,
		// the two threads won't have their name set correctly.
		if (threadData->threadDebugName)
			SetThreadName(-1, threadData->threadDebugName);

		if (threadData->numberToTest)
			return PrimalityTestWorker(threadData->numberToTest);
	}
}	

//-----------------------------------------------------------------------------
int main()
{
	HANDLE threadHandles[2];

	ThreadData_t firstThreadData = { 0 };
	firstThreadData.numberToTest = PRIME_NUMBER;
	firstThreadData.threadDebugName = "First Primality Test";
	
	ThreadData_t secondThreadData = { 0 };
	secondThreadData.numberToTest = PRIME_NUMBER;
	secondThreadData.threadDebugName = "Second Primality Test";
	
	// We use _beginthreadex instead of _beginthread because the latter can reuse the first thread handle
	// when creating the second thread, which basically...doesn't work, since our WaitForMultipleObjects would
	// then actually only wait for one thread
	threadHandles[0] = (HANDLE) _beginthreadex(0, 0, &PrimalityTestThread, &firstThreadData, 0, 0);
    
	unsigned long threadId;
	bool gotThreadId = false;
		
	if (IsDebuggerPresent())
	{
		if (OSSupportsGetThreadId())
		{
			if (MyGetThreadId(threadHandles[0], threadId))
				gotThreadId = true;
		}
		else
		{		
			if (GetThreadIdForOlderWindowsVersions(threadHandles[0], threadId))
				gotThreadId = true;
		}

		// Be careful, setting the thread name from the main thread doesn't work
		// reliably, that's why we also do it in each thread. We do it also in the main thread just
		// to show that it's not reliable to do it like this.
		// Basically, when we call SetThreadName in the main thread, the thread might or might not have been
		// initialized, and then more often than not the thread name we set is overriden by the initialization
		// process.
		if (gotThreadId)
			SetThreadName(threadId, "First primality test");
	}

	threadHandles[1] = (HANDLE) _beginthreadex(0, 0, &PrimalityTestThread, &secondThreadData, 0, 0);
	
	if (IsDebuggerPresent())
	{
		gotThreadId = false;
		if (OSSupportsGetThreadId())
		{
			if (MyGetThreadId(threadHandles[1], threadId))
				gotThreadId = true;
		}
		else
		{		
			if (GetThreadIdForOlderWindowsVersions(threadHandles[1], threadId))
				gotThreadId = true;
		}

		if (gotThreadId)
			SetThreadName(threadId, "Second primality test");
	}

    // Wait until all threads have terminated.
    WaitForMultipleObjects(2, threadHandles, TRUE, INFINITE);

	CloseHandle(threadHandles[0]);
	CloseHandle(threadHandles[1]);

	return 0;
}
