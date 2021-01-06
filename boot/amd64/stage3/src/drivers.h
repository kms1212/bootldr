#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "type.h"
#include "string.h"
#include "memory.h"
#include "port.h"

/////////////////////////////////////////
// iotty.h
/////////////////////////////////////////

#define TAB_LENGTH 8

#pragma pack (push, 1)
typedef struct fbCharacterStruct {
	BYTE fb_ch;
	BYTE fb_attr;
} ttyFBChar;
#pragma pack (pop)

void ttyClearScreen();
void ttyPrint(const char* str);
void ttyPrinti(int val, int base);
void ttyPrintn(const char* str, size_t s);
int ttyPutchar(int ch);
void ttyEnableCursor(uint8_t start, uint8_t end);
void ttyDisableCursor();
void ttySetCursorPosition(coord pos);
coord ttyGetCursorPosition();

/////////////////////////////////////////
// ioatapio.h
/////////////////////////////////////////

#define ATAPIO_STATUS_BSY    0x80
#define ATAPIO_STATUS_RDY    0x40
#define ATAPIO_STATUS_DRQ    0x08
#define ATAPIO_STATUS_DF     0x20
#define ATAPIO_STATUS_ERR    0x01

void* ATAPIORead(void* dest, uint32_t lba, uint8_t s_s);
void* ATAPIOWrite(uint32_t lba, uint8_t s_s, void* src);
static void ATAPIOWaitBusy();
static void ATAPIOWaitDRQ();

/////////////////////////////////////////
// iokbd.h
/////////////////////////////////////////

#define PS2KBD_KEYSTAT_UP        0x00
#define PS2KBD_KEYSTAT_DOWN      0x01
#define PS2KBD_KEYSTAT_EXTENDED  0x02

#define PS2KBD_STATUS_CAPSLOCK   0x04
#define PS2KBD_STATUS_NUMLOCK    0x02
#define PS2KBD_STATUS_SCRLLOCK   0x01

#define PS2KBD_SPECIALKEY_LALT   0x08
#define PS2KBD_SPECIALKEY_LCTRL  0x04
#define PS2KBD_SPECIALKEY_LMETA  0x02
#define PS2KBD_SPECIALKEY_LSHIFT 0x01

#define PS2KBD_SPECIALKEY_RALT   0x80
#define PS2KBD_SPECIALKEY_RCTRL  0x40
#define PS2KBD_SPECIALKEY_RMETA  0x20
#define PS2KBD_SPECIALKEY_RSHIFT 0x10

#define PS2KBD_KEYMAP_MAXCOUNT   0x89

#define PS2KBD_KEY_NONE          0x00
#define PS2KBD_KEY_ENTER         0x0A
#define PS2KBD_KEY_TAB           0x09
#define PS2KBD_KEY_ESC           0x1B
#define PS2KBD_KEY_BACKSPACE     0x08
#define PS2KBD_KEY_CTRL          0x81
#define PS2KBD_KEY_LSHIFT        0x82
#define PS2KBD_KEY_RSHIFT        0x83
#define PS2KBD_KEY_PRINTSCREEN   0x84
#define PS2KBD_KEY_LALT          0x85
#define PS2KBD_KEY_CAPSLOCK      0x86
#define PS2KBD_KEY_F1            0x87
#define PS2KBD_KEY_F2            0x88
#define PS2KBD_KEY_F3            0x89
#define PS2KBD_KEY_F4            0x8A
#define PS2KBD_KEY_F5            0x8B
#define PS2KBD_KEY_F6            0x8C
#define PS2KBD_KEY_F7            0x8D
#define PS2KBD_KEY_F8            0x8E
#define PS2KBD_KEY_F9            0x8F
#define PS2KBD_KEY_F10           0x90
#define PS2KBD_KEY_NUMLOCK       0x91
#define PS2KBD_KEY_SCROLLLOCK    0x92
#define PS2KBD_KEY_HOME          0x93
#define PS2KBD_KEY_UP            0x94
#define PS2KBD_KEY_PAGEUP        0x95
#define PS2KBD_KEY_LEFT          0x96
#define PS2KBD_KEY_CENTER        0x97
#define PS2KBD_KEY_RIGHT         0x98
#define PS2KBD_KEY_END           0x99
#define PS2KBD_KEY_DOWN          0x9A
#define PS2KBD_KEY_PAGEDOWN      0x9B
#define PS2KBD_KEY_INS           0x9C
#define PS2KBD_KEY_DEL           0x9D
#define PS2KBD_KEY_F11           0x9E
#define PS2KBD_KEY_F12           0x9F
#define PS2KBD_KEY_PAUSE         0xA0

typedef struct ps2kbdKeyMapCharStruct {
	BYTE kbdAsciiNom;
	BYTE kbdAsciiAlt;
}ps2kbdKeyMap;

bool ps2kbdCheckOutputBuffer();
bool ps2kbdCheckInputBuffer();
bool ps2kbdWaitInput();
bool ps2kbdWaitOutput();
bool ps2kbdActivate();
uint8_t ps2kbdGetCode();
bool ps2kbdSetLED(uint8_t led);
bool ps2kbdCheckUseAltKey(uint8_t code);
bool ps2kbdIsAlphabetCode(uint8_t code);
bool ps2kbdIsNumberSymbolCode(uint8_t code);
bool ps2kbdIsNumpadCode(uint8_t code);
void ps2kbdUpdateLockStatus(uint8_t code);
bool ps2kbdConvertToAscii(uint8_t scanCode, uint8_t* asciiCode, uint8_t* flags);

#endif