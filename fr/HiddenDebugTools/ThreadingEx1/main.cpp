#include <Windows.h>
#include <process.h>
#include <cmath>

// max signed int - 1 is the highest prime number that can be represented with an unsigned int
// It will be usefull to generate a bit of load on the cpu when running our dummy "primality test" 
// code on several threads...
#define PRIME_NUMBER (1L << 31L) - 1L

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
		if (n % i == 0)
		{
			return 1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
unsigned int __stdcall PrimalityTestThread(void* params)
{
	if (!params)
		return 0;

	long* numberToTest = static_cast<long*>(params);
	return PrimalityTestWorker(*numberToTest);		
}	

//-----------------------------------------------------------------------------
int main()
{
	HANDLE threadHandles[2];

	long firstThreadData = PRIME_NUMBER;
	long secondThreadData = PRIME_NUMBER;
	
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
