#ifndef NUROS_H_
#define NUROS_H_

#ifdef __linux__
	#include "NurOs_Linux.h"
#else
	#include "NurOs_Win32.h"
#endif

#endif /* NUROS_H_ */
