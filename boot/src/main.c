#include "type.h"

void printString(int x, int y, const char* str);

void _start(void)
{
	const char* str = "CLANG";
	fbChar* charCurs = (fbChar*) 0xB8000;
	charCurs[0].fb_ch = 'a';
	for (int i = 0; str[i] != 0; i++) {
		charCurs[i].fb_ch = 'a';
	}

	while (1);
}

void printString(int x, int y, const char* str)
{
	fbChar* charCurs = (fbChar*) 0xB8000;
	int i;

	charCurs += (y * 80) + x;
	for (i = 0; str[i] != 0; i++)
	{
		charCurs[i].fb_ch = str[i];
	}
}
