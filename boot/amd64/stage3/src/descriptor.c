#include "descriptor.h"

void initTables(void)
{
	GDTR* gdtr;
	GDTENTRY8* gdtEntry;
	TSS* tss;
	
	gdtr = (GDTR*)GDTR_BASEADDR;
	gdtEntry = (GDTENTRY8*)(uint64_t)(GDTR_BASEADDR + sizeof(GDTR));
	gdtr->dtrLimit = GDT_SIZE - 1;
	gdtr->dtrBaseAddr = (void*)gdtEntry;
	
	tss = (TSS*)((void*)gdtr + GDT_SIZE);
	
	setGDTEntry8(&(gdtEntry[0]), 0, 0, 0, 0, 0);
	setGDTEntry8(&(gdtEntry[1]), 0, 0xFFFFF, GDT_FLAG_HI_CODE, GDT_FLAG_LO_LOADERCODE, GDT_SEGFLAG_CODE);
	setGDTEntry8(&(gdtEntry[2]), 0, 0xFFFFF, GDT_FLAG_HI_DATA, GDT_FLAG_LO_LOADERDATA, GDT_SEGFLAG_DATA);
	setGDTEntry16((GDTENTRY16*)&(gdtEntry[3]), (void*)tss, TSS_SIZE - 1, GDT_FLAG_HI_TSS, GDT_FLAG_LO_TSS, GDT_SEGFLAG_TSS);
	
	initTSS(tss);
}

void setGDTEntry8(GDTENTRY8* gdtEntry, DWORD baseAddr, DWORD limit, BYTE upperFlag, BYTE lowerFlag, BYTE segmentType)
{
	gdtEntry->entryLoLimit = limit & 0xFFFF;
	
	gdtEntry->entryLoBaseAddr = baseAddr & 0xFFFF;
	gdtEntry->entryHiBaseAddr1 = (baseAddr >> 16) & 0xFF;
	gdtEntry->entryHiBaseAddr2 = (baseAddr >> 24) & 0xFF;
	
	gdtEntry->entryLowerFlag = lowerFlag | segmentType;
	gdtEntry->entryHiLimit_Flag = ((limit >> 16) & 0xFF) | upperFlag;
}

void setGDTEntry16(GDTENTRY16* gdtEntry, void* baseAddr, DWORD limit, BYTE upperFlag, BYTE lowerFlag, BYTE segmentType)
{
	gdtEntry->entryLoLimit = limit & 0xFFFF;
	
	gdtEntry->entryLoBaseAddr = (uint64_t)baseAddr & 0xFFFF;
	gdtEntry->entryMidBaseAddr1 = ((uint64_t)baseAddr >> 16) & 0xFF;
	gdtEntry->entryMidBaseAddr2 = ((uint64_t)baseAddr >> 24) & 0xFF;
	gdtEntry->entryHiBaseAddr = (uint64_t)baseAddr >> 32;
	
	gdtEntry->entryLowerFlag = lowerFlag | segmentType;
	gdtEntry->entryHiLimit_Flag = ((limit >> 16) & 0xFF) | upperFlag;
}

void initTSS(TSS* tss)
{
    memset(tss, 0, sizeof(TSS));
    tss->tssIST[ 0 ] = IST_BASEADDR + IST_SIZE;
    tss->tssIOMapBaseAddr = 0xFFFF;
}

void initIDT(void)
{
	IDTR* idtr;
	IDTENTRY* idtEntry;
	
	idtr = (IDTR*)IDTR_BASEADDR;
	idtEntry = (IDTENTRY*)(IDTR_BASEADDR + sizeof(IDTR));
	idtr->dtrBaseAddr = (void*)idtEntry;
	idtr->dtrLimit = IDT_SIZE - 1;
	
	for (int i = 0; i < IDT_MAXENTRY; i++)
	{
		setIDTEntry(&(idtEntry[i]), tmpHandler, 0x08, IDT_FLAG_IST1, IDT_FLAG_LOADER, IDT_TYPE_INTERRUPT);
	}
}

void setIDTEntry(IDTENTRY* idtEntry, void* handler, WORD selector, BYTE ist, BYTE flags, BYTE type)
{
	idtEntry->entryLoBaseAddr1 = (uint64_t)handler & 0xFFFF;
	idtEntry->entryLoBaseAddr2 = ((uint64_t)handler >> 16) & 0xFFFF;
	idtEntry->entryHiBaseAddr  = (uint64_t)handler >> 32;
	
	idtEntry->entrySegment = selector;
	idtEntry->entryIST = ist & 0x03;
	idtEntry->entryFlag = type | flags;
}

void tmpHandler(void)
{
	ttyPrint("Interrupt\n");
	
	while(1);
}