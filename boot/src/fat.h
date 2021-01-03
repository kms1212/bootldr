#ifndef __FAT_H__
#define __FAT_H__

#include "type.h"
#include "memory.h"
#include "string.h"

#define FAT_SUCCESS             0;
#define FAT_INVALID_FSTYPE      1;
#define FAT_FILE_NOT_FOUND      1;

#pragma pack (push, 1)
typedef struct fatBPBStruct {
	BYTE bpbJumpCode[3];
	BYTE bpbOEMName[8];
	WORD bpbBytesPerSector;
	BYTE bpbSectorPerCluster;
	WORD bpbReservedSectors;
	BYTE bpbFATCount;
	WORD bpbRootEntries;
	WORD bpbTotalSectors;
	BYTE bpbMedia;
	WORD bpbSectorsPerFAT16;
	WORD bpbSectorsPerTrack;
	WORD bpbDiskHeads;
	DWORD bpbHiddenSectors;
	DWORD bpbTotalLargeSectors;
	
	DWORD ebpbSectorsPerFAT32;
	WORD ebpbFlags;
	WORD ebpbFSVersion;
	DWORD ebpbRootDirectoryCluster;
	WORD ebpbFSInfoSector;
	WORD ebpbBSBackupSector;
	BYTE ebpbReserved1[12];
	BYTE ebpbPhysicalDriveNum;
	BYTE ebpbReserved2;
	BYTE ebpbExtendedBootSign;
	DWORD ebpbVolumeSerial;
	BYTE ebpbVolumeLabel[11];
	BYTE ebpbFSType[8];
} BPB;
#pragma pack (pop)

int fat_init(BPB* bpb);

#endif
