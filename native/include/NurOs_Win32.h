#ifndef NUROS_WIN32_H_
#define NUROS_WIN32_H_

#include <windows.h>
#include <tchar.h>

#ifndef in_addr_t
typedef ULONG in_addr_t;
#endif

#ifndef NUR_WIN32
	#define NUR_WIN32 1
#endif

#ifdef UNDER_CE
	// WIN CE
	#define NUR_WINCE 1
	#define NUR_MODULECONTROL 1
	#undef NUR_USB_AUTOCONNECT
	#undef NUR_USB_TRANSPORT
#include <winsock2.h>
#else
	// PC
	#define NUR_USB_AUTOCONNECT 	1
	#define NUR_USB_TRANSPORT 		1
	#define NUR_WINUSB_TRANSPORT	1
	#undef NUR_MODULECONTROL
#endif

#define NUR_SOCKET_TRANSPORT 1
#define NUR_SERVER_TRANSPORT 1
//#define NUR_BLUETOOTH_SOCKET 1
#define NUR_SERIAL_TRANSPORT 1
//#define NUR_NASS_TRANSPORT	1

enum NUR_USB_CONNTYPE
{
	USB_CONNTYPE_INVALID = 0,
	USB_CONNTYPE_USBSER = 1,
	USB_CONNTYPE_WINUSB = 2
};

#define NurAtomicVar
#define NurAtomicSet(ptr, val) InterlockedExchange((volatile LONG*)ptr, val)
#define NurAtomicGet(ptr) InterlockedExchangeAdd((volatile LONG*)ptr, 0)
#define NurAtomicInc(ptr) InterlockedExchangeAdd((volatile LONG*)ptr, 1)
#define NurAtomicDec(ptr) InterlockedExchangeSubtract((volatile LONG*)ptr, 1)
#ifdef NUR_WINCE
#define NurSyncMem() do { } while (0)
#else
#define NurSyncMem() MemoryBarrier()
#endif

#endif /* NUROS_WIN32_H_ */
