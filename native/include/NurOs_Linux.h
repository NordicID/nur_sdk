#ifndef _NUROSLINUX_H_
#define _NUROSLINUX_H_ 1

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <wchar.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>

#if defined(__yocto__) || defined(__ANDROID__)
#include <pthread.h>
#endif

#ifndef NUR_LINUX
	#define NUR_LINUX 1
#endif

#ifndef min
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef max
#define max(X,Y) ((X) > (Y) ? (X) : (Y))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#undef NUR_USB_AUTOCONNECT
#undef NUR_USB_TRANSPORT
#undef NUR_MODULECONTROL
#undef NUR_BLUETOOTH_SOCKET

#define NUR_SERIAL_TRANSPORT 1
#define NUR_SOCKET_TRANSPORT 1
#define NUR_SERVER_TRANSPORT 1

//#define NUR_SPI_TRANSPORT 1

#define ERROR_SUCCESS	0
#define MAX_PATH		260

//#define NURAPI_WIDECHARS 1

typedef unsigned long long ULONGLONG;
typedef long LONG;

#if !defined(__ios__) && !defined(__APPLE__) 
    typedef unsigned long ULONG;
    typedef unsigned int BOOL;
#endif

typedef unsigned int DWORD;
typedef unsigned int UINT;
typedef unsigned int UINT32;
typedef DWORD *LPDWORD;

typedef unsigned short WORD;
typedef unsigned short UINT16;
typedef unsigned short USHORT;
typedef short SHORT;

typedef unsigned char BYTE;

typedef void *HANDLE;
typedef void *HMODULE;
typedef void *HINSTANCE;
typedef void *LPVOID;
typedef char *LPCSTR;

typedef int errno_t;
typedef int SOCKET;

//typedef USHORT WCHAR;
typedef wchar_t WCHAR;

#ifndef NURAPI_WIDECHARS
	typedef char TCHAR;
	#define _T(x) x
	#define TEXT _T
	#define _tcscmp strcmp
	#define _tcsncmp strncmp
	#define _tcsicmp strcasecmp
	#define _tcsnicmp strncasecmp
	#define _tcslen strlen
	#define _stprintf_s snprintf
	#define _vstprintf_s vsnprintf
	#define _stprintf sprintf
	#define _tfopen_s fopen_s
	#define _tfopen fopen
	#define _tcscpy_s strcpy_s
	#define _tcscpy strcpy
	#define _tcsncpy_s strncpy_s
	#define _tprintf printf
	#define _ftprintf fprintf
	#define _tscanf scanf
	#define _tcsupr _strupr 
	#define _tcslwr _strlwr
	#define _tcschr	strchr
	#define _tcstok strtok

	#undef UNICODE
	#undef _UNICODE
#else
	typedef WCHAR TCHAR;
	#define _T(x) L##x
	#define TEXT _T
	#define _tcscmp wcscmp
	#define _tcsncmp wcsncmp
	#define _tcslen wcslen
	#define _stprintf_s swprintf_s
	#define _vstprintf_s vswprintf_s
	#define _stprintf(dst,fmt,...) swprintf(dst,128,fmt,## __VA_ARGS__)
	#define _tfopen_s _wfopen_s
	#define _tfopen _wfopen
	#define _tcscpy_s wcscpy_s
	#define _tcscpy wcscpy
	#define _tcsncpy_s wcsncpy_s
	#define _tprintf wprintf
	#define _ftprintf fwprintf
	#define _tscanf wscanf
	#define _tcsupr wcsupr
	#define _tcslwr wcslwr
	#define UNICODE 1
	#define _UNICODE 1
#endif

typedef WCHAR *LPWSTR;
typedef TCHAR *LPTSTR;

#define _isnan isnan

#define INVALID_HANDLE_VALUE ((HANDLE)0xFFFFFFFF)
#define INVALID_SOCKET 		(-1)

#define WINAPI
#define CALLBACK
#define APIENTRY
#define __stdcall
#define __cdecl

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef MAXBYTE
	#define MAXBYTE 	0xFF
#endif

#ifndef MAXUSHORT
	#define MAXUSHORT  	0xFFFF
#endif

#ifndef MAXWORD
	#define MAXWORD 	MAXUSHORT
#endif

#ifndef MAXDWORD
	#define MAXDWORD 	0xFFFFFFFF
#endif

#ifndef _countof
	#define _countof(x) ((DWORD)(sizeof(x)/sizeof((x)[0])))
#endif

#ifdef NUR_EXPOSE_WIN32API

#define sprintf_s snprintf
#define swprintf_s snwprintf

errno_t strcpy_s(char *strDestination, size_t numberOfElements, const char *strSource);
errno_t strncpy_s(char *strDest, size_t numberOfElements, const char *strSource, size_t count);
errno_t wcscpy_s(WCHAR *strDestination, size_t numberOfElements, const WCHAR *strSource);
errno_t wcsncpy_s(WCHAR *strDest, size_t numberOfElements, const WCHAR *strSource, size_t count);
int vswprintf_s(wchar_t *buffer, size_t numberOfElements, const wchar_t *format, va_list argptr);
int swprintf_s(wchar_t *buffer, size_t sizeOfBuffer, const wchar_t *format, ...);

errno_t fopen_s(FILE** pFile, const char *filename, const char *mode);
errno_t _wfopen_s(FILE** pFile, const WCHAR *filename, const WCHAR *mode);

#define GetProcAddress(a,b) NULL
#define GetProcAddressW(a,b) NULL

typedef DWORD (*LPTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);

#define INFINITE 0xFFFFFFFF

#define _TRUNCATE   (-1)
#define STRUNCATE 80

#define STILL_ACTIVE 		((DWORD)0x00000103L)
#define EVENT_ALL_ACCESS	(0x1f0003)
#define CP_UTF8 			(65001)

typedef struct
{
	DWORD sig;
	pthread_t locker;
	int lockCount;
	pthread_cond_t cond;
	pthread_mutex_t lock;
} NURMUTEX;

typedef NURMUTEX CRITICAL_SECTION;
typedef NURMUTEX* LPCRITICAL_SECTION;

void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
DWORD EnterCriticalSectionTimed(LPCRITICAL_SECTION lpCriticalSection, DWORD dwMilliseconds);
BOOL EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

BOOL DeleteFile(const TCHAR *lpFileName);

#define ZeroMemory(a,b) memset(a, 0, b)

DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

HANDLE CreateEvent(LPVOID lpEventAttributes, BOOL bManualReset, BOOL InitialState, const TCHAR *lpName);
BOOL SetEvent(HANDLE hEvent);
BOOL ResetEvent(HANDLE hEvent);

HANDLE CreateMutex(LPVOID lpMutexAttributes, BOOL bInitialOwner, const TCHAR *lpName);  
BOOL ReleaseMutex(HANDLE hMutex);

HANDLE CreateThread(LPVOID lpThreadAttributes, DWORD dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);   
BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode);
BOOL GetExitCodeThread(HANDLE hThread, LPDWORD lpExitCode);

BOOL CloseHandle(HANDLE hObject);

DWORD GetTickCount();

int MultiByteToWideChar(UINT CodePage, DWORD dwFlags, 
  LPCSTR lpMultiByteStr, int cbMultiByte,
  LPWSTR lpWideCharStr, int cchWideChar);

#define Sleep(x) usleep((x)*1000)

// Sockets
#define closesocket close
#define AF_BT AF_BLUETOOTH
#define BTHPROTO_RFCOMM BTPROTO_RFCOMM

#define GetLastError() errno
#define SetLastError(a) errno = (a)
#define WSAGetLastError GetLastError

#define WSAEWOULDBLOCK 	EINPROGRESS
#define SOCKET_ERROR	-1
#define WAIT_ERROR 		1000
#define WAIT_TIMEOUT 	258
#define WAIT_OBJECT_0 	0

#endif

#ifdef __cplusplus
}
#endif

#endif
