TARGET =	bootpart.img
BINPATH =	../buildutils/cross/bin/

AS = 		nasm
ASFLAG = 	-I${CURDIR}/src 
CC = 		$(BINPATH)/i686-elf-gcc
CCFLAG = 	-c -m32 -ffreestanding
LD =		$(BINPATH)/i686-elf-ld
LDFLAG = 	-melf_i386 -T$(CURDIR)/ldscripts/i686-elf.x -nostdlib -e main -Ttext 0x8000
OBJCOPY = 	$(BINPATH)/i686-elf-objcopy 
OBJCOPYFLAG = 	-j .text -j .data -j .rodata -j .bss -S -O binary

ASFLAG64 = 	    -I${CURDIR}/src/64bit
CC64 = 			$(BINPATH)/x86_64-elf-gcc
CCFLAG64 = 		-c -m64 -ffreestanding
LD64 =			$(BINPATH)/x86_64-elf-ld
LDFLAG64 = 		-melf_x86_64 -T$(CURDIR)/ldscripts/x86_64-elf.x -nostdlib -e_start -Ttext 0x200000
OBJCOPY64 = 	$(BINPATH)/x86_64-elf-objcopy 
OBJCOPYFLAG64 = -j .text -j .data -j .rodata -j .bss -S -O binary

BSMERGE = 	$(BINPATH)/../../utils/bsmerge
OSNAME := $(shell uname -s)
ifeq ($(OSNAME), Ubuntu)
	MKFSFAT = mkfs.msdos
	MKFSARG = -n "INSTALL" -b 2 -R 32 -s 1 -F 32
endif
ifeq ($(OSNAME), FreeBSD)
	MKFSFAT = newfs_msdos
	MKFSARG = -L "INSTALL" -k 2 -i 1 -r 32 -c 1 -F 32
endif

S1BOOTLDRIMG =	  tmp/s1boot.img
S2BOOTLDRBIN =	  tmp/s2boot.bin
S2BOOTLDRELF =    tmp/s2boot.elf
S2BOOTLDRRAW =    tmp/s2boot.elf.bin
S2BOOTLDRIMG =    tmp/s2boot.elf.img
S2BOOTLDRELF64 =  tmp/64bit/s2boot64.elf
S2BOOTLDRRAW64 =  tmp/64bit/s2boot64.elf.bin
S2BOOTLDRIMG64 =  tmp/64bit/s2boot64.elf.img
FATIMG =	tmp/fat.img

CSOURCES = $(wildcard src/*.c)
CENTRYOBJECT = tmp/main.o
COBJECTS = $(subst $(CENTRYOBJECT), , $(patsubst src/%.c, tmp/%.o, $(CSOURCES)))
ASMSOURCES = $(subst src/s1boot.asm src/s2boot.asm, , $(wildcard src/*.asm))
ASMOBJECTS = $(patsubst src/%.asm, tmp/%.o, $(ASMSOURCES))

FILES = \
	tmp/* \
	dep/* \
	$(FATIMG) \
	$(TARGET)

all : $(TARGET)

mkimage : $(FATIMG)

$(FATIMG) : $(TARGET)
	dd if=/dev/zero of="$(FATIMG)" bs=1024 count=102400
	$(MKFSFAT) $(MKFSARG) "$(FATIMG)"
	#mcopy -i $@ $(S2BOOTLDRBIN) ::STARTUP.BIN
	$(BSMERGE) $@ $(S1BOOTLDRIMG) 0 $(FATIMG)
	$(BSMERGE) $@ $(S2BOOTLDRIMG) 1000 $@
	$(BSMERGE) $@ $(S2BOOTLDRIMG64) 1A00 $@
	mv -f $@ $<

$(TARGET) : buildDirectoryStructure $(S1BOOTLDRIMG) $(S2BOOTLDRIMG) $(S2BOOTLDRIMG64)

# BOOTLDR16/32
$(S1BOOTLDRIMG) : $(patsubst tmp/%.img, src/%.asm, $(S1BOOTLDRIMG))
	$(AS) $(AMFLAG) -o $@ $<

$(S2BOOTLDRIMG) : $(S2BOOTLDRBIN) $(S2BOOTLDRELF)
	$(OBJCOPY) $(OBJCOPYFLAG) $(S2BOOTLDRELF) $(S2BOOTLDRRAW)
	cat $(S2BOOTLDRBIN) $(S2BOOTLDRRAW) > $@

$(S2BOOTLDRELF) : $(CENTRYOBJECT) $(COBJECTS) $(ASMOBJECTS)
	$(CC) $(CCFLAG) -MM $(CSOURCES) > dep/dependency.dep
	$(LD) $(LDFLAG) -o $@ $^
	
$(S2BOOTLDRBIN) : $(patsubst tmp/%.bin, src/%.asm, $(S2BOOTLDRBIN))
	$(AS) $(ASFLAG) -o $@ $<

tmp/%.o : src/%.asm
	$(AS) $(ASFLAG) -felf32 $< -o $@

tmp/%.o : src/%.c
	$(CC) $(CCFLAG) -c $< -o $@
	
# BOOTLDR64
CSOURCES64 = $(wildcard src/64bit/*.c)
COBJECTS64 = $(patsubst src/64bit/%.c, tmp/64bit/%.o, $(CSOURCES64))
ASMENTRYOBJECT64 = tmp/64bit/s2boot64.o
ASMSOURCES64 = $(wildcard src/64bit/*.asm)
ASMOBJECTS64 = $(subst $(ASMENTRYOBJECT64), , $(patsubst src/64bit/%.asm, tmp/64bit/%.o, $(ASMSOURCES64)))

$(S2BOOTLDRIMG64) : $(S2BOOTLDRELF64)
	$(OBJCOPY64) $(OBJCOPYFLAG64) $^ $@

$(S2BOOTLDRELF64) : $(ASMENTRYOBJECT64) $(COBJECTS64) $(ASMOBJECTS64)
	$(CC64) $(CCFLAG64) -MM $(CSOURCES64) > dep/dependency64.dep
	$(LD64) $(LDFLAG64) -o $@ $^

tmp/64bit/%.o : src/64bit/%.asm
	$(AS) $(ASFLAG64) -felf64 $< -o $@

tmp/64bit/%.o : src/64bit/%.c
	$(CC64) $(CCFLAG64) -c $< -o $@


buildDirectoryStructure : 
	mkdir -p tmp
	mkdir -p tmp/64bit
	mkdir -p dep

clean :
	rm -rf $(FILES)

ifeq (dep/dependency.dep, $(wildcard dep/dependency.dep))
include dep/dependency.dep
endif
ifeq (dep/dependency64.dep, $(wildcard dep/dependency64.dep))
include dep/dependency64.dep
endif
