.POSIX:
.PHONY: clean run

CC=g++
AS=as
AR=ar
QEMU=qemu-system-i386

ARCH ?= X86

MKDIR_P=mkdir -p

ASFLAGS=--32
NASMFLAGS=-f elf32
#CFLAGS=-m32 -Wall -O4 -D$(ARCH) -fno-omit-frame-pointer -Wextra -fno-builtin -ffreestanding -Isrc/include -g3
CFLAGS=-m32 -Wall -Wextra -pedantic -ffreestanding -fno-builtin -fno-exceptions -fno-rtti -Isrc/include
LDFLAGS=-m32 -nostdlib -nodefaultlibs

SRC_DIR=src
X86_DIR=src/arch/x86
OUT_DIR=out
IMG_DIR=iso

BOOT_FILE=fos.elf
GRUB_CFG=grub.cfg
BOOT_DIR=$(IMG_DIR)/boot
GRUB_DIR=$(BOOT_DIR)/grub
BOOT_FILE_PATH=$(OUT_DIR)/$(BOOT_FILE)
GRUB_CFG_PATH=$(GRUB_DIR)/$(GRUB_CFG)


X86_FILE=x86.a
X86_FILE_PATH=$(X86_DIR)/$(X86_FILE)

C_IN=$(wildcard $(SRC_DIR)/base/kernel.cpp $(X86_DIR)/*.cpp $(X86_DIR)/*/*.cpp)
C_OUT=$(C_IN:.cpp=.o)

A_IN=$(wildcard $(SRC_DIR)/base/boot.S $(X86_DIR)/*/*.S)
A_OUT=$(A_IN:.S=.asm.o)


CRTI_OBJ:=$(OUT_DIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(OUT_DIR)/crtn.o

OBJS=$(A_OUT) $(C_OUT)

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \

IMG_FILE=fos.img

all: directories $(IMG_FILE)

directories: $(OUT_DIR)

%.asm.o: %.S
	$(AS) $(ASFLAGS) -o $@ $<

.cpp.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(BOOT_FILE): $(OBJ_LINK_LIST)
	@echo Make boot kernel...
	$(CC) $(LDFLAGS) -T src/linker.ld -o $@ $^

$(OUT_DIR):
	$(MKDIR_P) $(OUT_DIR)

$(IMG_FILE): $(BOOT_FILE)
	@echo Make image...
	$(MKDIR_P) $(GRUB_DIR)
	@echo 'menuentry "myos" {' > $(GRUB_CFG_PATH)
	@echo '	'multiboot /boot/$(BOOT_FILE) >> $(GRUB_CFG_PATH)
	@echo '}' >> $(GRUB_CFG_PATH)
	@cp $< $(BOOT_DIR)
	@grub-mkrescue -o $@ $(IMG_DIR) 2> /dev/null

run: all
	$(QEMU) -hda $(IMG_FILE) -serial file:serial.log

run-term: all
	$(QEMU) -hda $(IMG_FILE) -curses

clean:
	rm -rf $(IMG_DIR)/ $(IMG_FILE) $(OBJS)

