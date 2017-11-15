#include <NurApi.h>

// Example: Set module TX level
int SetTxLevel(HANDLE hApi, int level)
{
	struct NUR_MODULESETUP setup;
	setup.txLevel = level;
	return NurApiSetModuleSetup(hApi, NUR_SETUP_TXLEVEL, &setup, sizeof(setup));
}

// Example: Get module TX level
int GetTxLevel(HANDLE hApi, int *level)
{
	struct NUR_MODULESETUP setup;
	int ret = NurApiGetModuleSetup(hApi, NUR_SETUP_TXLEVEL, &setup, sizeof(setup));
	if (ret == NUR_NO_ERROR) {
		*level = setup.txLevel;
	}
	return ret;
}
