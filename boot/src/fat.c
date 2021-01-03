#include "fat.h"

int fat_init(BPB* bpb)
{
	int ret = false;
	if (strncmp(bpb->ebpbFSType, "FAT32", 5) == 0)
	{
		ret = true;
	}
	if (bpb->ebpbExtendedBootSign == 0x29)
	{
		ret = true;
	}
	return ret;
}