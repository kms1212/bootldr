#ifndef __TYPE_H__
#define __TYPE_H__

#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned long
#define QWORD unsigned long long

#define true 1
#define false 0
#define NULL 0

#pragma pack (push, 1)
typedef struct fbCharacterStruct {
	BYTE fb_ch;
	BYTE fb_attr;
} fbChar;
#pragma pack (pop)

#endif
