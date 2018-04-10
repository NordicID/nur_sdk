#include <NurAPI.h>

// Example: Read user memory from tag, singluted by EPC memory
int ReadTagUserMemoryByEPC(HANDLE hApi, BYTE *epc, int epcLength, BYTE *userMem, int userMemLen)
{
	return NurApiReadTagByEPC(hApi, 0, FALSE, epc, epcLength, NUR_BANK_USER, 0, userMemLen, userMem);
}

// Example: Read user memory from tag, singluted by EPC memory
int ReadTagUserMemory(HANDLE hApi, BYTE *epc, int epcLength, BYTE *userMem, int userMemLen)
{
	return NurApiReadSingulatedTag32(hApi, 0, FALSE, NUR_BANK_EPC, 0x20, epcLength*8, epc, NUR_BANK_USER, 0, userMemLen, userMem);
}

// Example: Read user memory from any tag
int ReadAnyTagUserMemory(HANDLE hApi, BYTE *epc, int epcLength, BYTE *userMem, int userMemLen)
{
	return NurApiReadTag32(hApi, 0, FALSE, NUR_BANK_USER, 0, userMemLen, userMem);
}

// Example: Write user memory to tag, singluted by EPC memory
int WriteTagUserMemoryByEPC(HANDLE hApi, BYTE *epc, int epcLength, BYTE *userMem, int userMemLen)
{
	return NurApiWriteTagByEPC(hApi, 0, FALSE, epc, epcLength, NUR_BANK_USER, 0, userMemLen, userMem);
}

// Example: Write user memory to tag, singluted by EPC memory
int WriteTagUserMemory(HANDLE hApi, BYTE *epc, int epcLength, BYTE *userMem, int userMemLen)
{
	return NurApiWriteSingulatedTag32(hApi, 0, FALSE, NUR_BANK_EPC, 0x20, epcLength*8, epc, NUR_BANK_USER, 0, userMemLen, userMem);
}

// Example: Write user memory to any tag
int WriteAnyTagUserMemory(HANDLE hApi, BYTE *epc, int epcLength, BYTE *userMem, int userMemLen)
{
	return NurApiWriteTag32(hApi, 0, FALSE, NUR_BANK_USER, 0, userMemLen, userMem);
}
