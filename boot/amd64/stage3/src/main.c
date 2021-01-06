#include "type.h"
#include "string.h"
#include "memory.h"
#include "drivers.h"
#include "fat.h"
#include "descriptor.h"

int main(void) {
	ttyPrint("--Stage 3 bootloader--\n");
	
	/*ttyPrint("Initializing GDT/TSS/IDT...");
	initTables();
	initIDT();
	ttyPrint("OK\n");
	
	ttyPrint("Loading GDT...");
	loadGDTR((void*)GDTR_BASEADDR);
	ttyPrint("OK\n");
	
	ttyPrint("Loading TSS...");
	loadTR(GDT_SEGMENT_TSS);
	ttyPrint("OK\n");
	
	ttyPrint("Loading IDT...");
	loadIDTR((void*)IDTR_BASEADDR);
	ttyPrint("OK\n");*/
	
	ttyPrint("Activating keyboard...");
	if (!ps2kbdActivate())
		ttyPrint("OK\n");
	else
	{
		ttyPrint("FAIL\n");
		while (0);
	}
	while (1)
	{
		if (!ps2kbdCheckOutputBuffer())
		{
			char temp = ps2kbdGetCode();
			char outTemp;
			uint8_t keyFlags;
			
			if (ps2kbdConvertToAscii(temp, &outTemp, &keyFlags))
			{
				if (keyFlags & PS2KBD_KEYSTAT_DOWN)
				{
					ttyPutchar(outTemp);
				}
			}
		}
	}
}