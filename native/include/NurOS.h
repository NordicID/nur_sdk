#ifndef NUROS_H_
#define NUROS_H_

#ifdef __linux__
	#include "NurOs_Linux.h"
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
        #include "NurOs_iOS.h"
    #elif TARGET_OS_IPHONE
        // iOS device
        #include "NurOs_iOS.h"
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
        #include "NurOs_macOS.h"
    #else
        #error "Unknown Apple platform"
    #endif
#else
	#include "NurOs_Win32.h"
#endif

#endif /* NUROS_H_ */
