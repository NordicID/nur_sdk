
#ifndef NURAPI_IOS_H
#define NURAPI_IOS_H

#include <pthread.h>
#include <objc/objc.h>

//typedef signed char BOOL;
typedef unsigned long ULONG;

#define __linux__
#define NUR_LINUX 1
#define NUR_APPLEBLE_TRANSPORT 1

#include "NurOs_Linux.h"

#undef BOOL

/**
 * Adds potentially missing baudrate definitions for OSX.
 */
#ifndef B460800
#define B460800 460800
#endif

#ifndef B500000
#define B500000 500000
#endif

#ifndef B576000
#define B576000 576000
#endif

#ifndef B921600
#define B921600 921600
#endif

#ifndef B1000000
#define B1000000 1000000
#endif

#endif // NURAPI_IOS_H
