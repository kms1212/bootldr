#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "memory.h"
#include "drivers.h"

#define GDT_SEGFLAG_CODE  0x0A
#define GDT_SEGFLAG_DATA  0x02
#define GDT_SEGFLAG_TSS   0x09 
#define GDT_FLAG_LOW_S    0x10
#define GDT_FLAG_LOW_DPL0 0x00
#define GDT_FLAG_LOW_DPL1 0x20
#define GDT_FLAG_LOW_DPL2 0x40
#define GDT_FLAG_LOW_DPL3 0x60
#define GDT_FLAG_LOW_P    0x80
#define GDT_FLAG_HIGH_L   0x20
#define GDT_FLAG_HIGH_DB  0x40
#define GDT_FLAG_HIGH_G   0x80

#define GDT_FLAG_LO_LOADERCODE (GDT_SEGFLAG_CODE | GDT_FLAG_LOW_S | GDT_FLAG_LOW_DPL0 | GDT_FLAG_LOW_P)
#define GDT_FLAG_LO_LOADERDATA (GDT_SEGFLAG_DATA | GDT_FLAG_LOW_S | GDT_FLAG_LOW_DPL0 | GDT_FLAG_LOW_P)

#define GDT_FLAG_LO_MODULECODE (GDT_SEGFLAG_CODE | GDT_FLAG_LOW_S | GDT_FLAG_LOW_DPL3 | GDT_FLAG_LOW_P)
#define GDT_FLAG_LO_MODULEDATA (GDT_SEGFLAG_DATA | GDT_FLAG_LOW_S | GDT_FLAG_LOW_DPL3 | GDT_FLAG_LOW_P)

#define GDT_FLAG_LO_TSS        (GDT_FLAG_LOW_DPL0 | GDT_FLAG_LOW_P)

#define GDT_FLAG_HI_CODE       (GDT_FLAG_HIGH_G | GDT_FLAG_HIGH_L)
#define GDT_FLAG_HI_DATA       (GDT_FLAG_HIGH_G | GDT_FLAG_HIGH_L)
#define GDT_FLAG_HI_TSS        (GDT_FLAG_HIGH_G)

#define GDT_SEGMENT_LOADERCODE 0x08
#define GDT_SEGMENT_LOADERDATA 0x10
#define GDT_SEGMENT_TSS        0x18

#define GDTR_BASEADDR          0x142000
#define GDT_ENTRY8COUNT        3
#define GDT_ENTRY16COUNT       1

#define GDT_SIZE               (( sizeof( GDTENTRY8 ) * GDT_ENTRY8COUNT ) + ( sizeof( GDTENTRY16 ) * GDT_ENTRY16COUNT ))

#define TSS_SIZE               sizeof(TSS)

#define IDT_TYPE_INTERRUPT     0x0E
#define IDT_TYPE_TRAP          0x0F
#define IDT_FLAG_DPL0          0x00
#define IDT_FLAG_DPL1          0x20
#define IDT_FLAG_DPL2          0x40
#define IDT_FLAG_DPL3          0x60
#define IDT_FLAG_P             0x80
#define IDT_FLAG_IST0          0x00
#define IDT_FLAG_IST1          0x01

#define IDT_FLAG_LOADER        ( IDT_FLAG_DPL0 | IDT_FLAG_P )
#define IDT_FLAG_MODULE        ( IDT_FLAG_DPL3 | IDT_FLAG_P )

#define IDT_MAXENTRY           100

#define IDTR_BASEADDR          ( GDTR_BASEADDR + sizeof( GDTR ) + GDT_SIZE + TSS_SIZE )
#define IDT_BASEADDR           ( IDTR_BASEADDR + sizeof( IDTR ) )
#define IDT_SIZE               ( IDT_MAXENTRY * sizeof( IDTENTRY ) )

#define IST_BASEADDR           0x210000
#define IST_SIZE               0x100000

#pragma pack(push, 1)
typedef struct gdtr_idtrStruct {
	WORD  dtrLimit;
	PNTR  dtrBaseAddr;
	WORD  dtrPadding1;
	DWORD dtrPadding2;
}GDTR, IDTR;

typedef struct gdtrEntry8Struct {
	WORD entryLoLimit;
	WORD entryLoBaseAddr;
	BYTE entryHiBaseAddr1;
	BYTE entryLowerFlag;
	BYTE entryHiLimit_Flag;
	BYTE entryHiBaseAddr2;
}GDTENTRY8;

typedef struct gdtrEntry16Struct {
	WORD  entryLoLimit;
	WORD  entryLoBaseAddr;
	BYTE  entryMidBaseAddr1;
	BYTE  entryLowerFlag;
	BYTE  entryHiLimit_Flag;
	BYTE  entryMidBaseAddr2;
	DWORD entryHiBaseAddr;
	DWORD entryReserved;
}GDTENTRY16;

typedef struct tssDataStruct {
	DWORD tssReserved1;
	QWORD tssRsp[3];
	QWORD tssReserved2;
	QWORD tssIST[7];
	QWORD tssReserved3;
	WORD  tssReserved4;
	WORD  tssIOMapBaseAddr;
}TSS;

typedef struct idtEntryStruct {
	WORD  entryLoBaseAddr1;
	WORD  entrySegment;
	BYTE  entryIST;
	BYTE  entryFlag;
	WORD  entryLoBaseAddr2;
	DWORD entryHiBaseAddr;
	DWORD entryReserved;
}IDTENTRY;

#pragma pack(pop)

void initTables(void);
void setGDTEntry8(GDTENTRY8* gdtEntry, DWORD baseAddr, DWORD limit, BYTE upperFlag, BYTE lowerFlag, BYTE segmentType);
void setGDTEntry16(GDTENTRY16* gdtEntry, void* baseAddr, DWORD limit, BYTE upperFlag, BYTE lowerFlag, BYTE segmentType);
void initTSS(TSS* tss);
void initIDT(void);
void setIDTEntry(IDTENTRY* idtEntry, void* handler, WORD selector, BYTE ist, BYTE flags, BYTE type);
void tmpHandler(void);

void loadGDTR(void* gdtr);
void loadIDTR(void* idtr);
void loadTR(WORD tr);

#endif