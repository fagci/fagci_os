.POSIX:
.PHONY: clean run

CC=g++
AS=as
QEMU=qemu-system-i386

MKDIR_P=mkdir -p

ASFLAGS=--32 #-f elf32
CFLAGS=-m32 -O2 -g -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti -c
LDFLAGS=-m32 -nostdlib -nodefaultlibs -T src/linker.ld

SRC_DIR=src
OUT_DIR=out
IMG_DIR=iso

BOOT_FILE=fos.elf
GRUB_CFG=grub.cfg
BOOT_DIR=$(IMG_DIR)/boot
GRUB_DIR=$(BOOT_DIR)/grub
BOOT_FILE_PATH=$(OUT_DIR)/$(BOOT_FILE)
GRUB_CFG_PATH=$(GRUB_DIR)/$(GRUB_CFG)

KLIB_DIR=src/klib
KLIB_FILE=libk.a
KLIB_FILE_PATH=$(KLIB_DIR)/$(KLIB_FILE)

CRTI_OBJ:=$(OUT_DIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(OUT_DIR)/crtn.o

OBJS=$(OUT_DIR)/boot.o $(OUT_DIR)/kernel.o $(KLIB_FILE_PATH)

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \

IMG_FILE=fos.img

all: directories $(IMG_FILE)

directories: $(OUT_DIR)

$(OUT_DIR):
	@echo Create directory
	$(MKDIR_P) $(OUT_DIR)

$(OUT_DIR)/boot.o: $(SRC_DIR)/boot.S
	@echo Make boot.o...
	$(AS) $(ASFLAGS) $< -o $@

$(OUT_DIR)/kernel.o: $(SRC_DIR)/kernel.cpp
	@echo Make kernel.o...
	$(CC) $(CFLAGS) $< -o $@

$(OUT_DIR)/%.o: $(SRC_DIR)/%.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(KLIB_FILE_PATH):
	@echo Make klib...
	@cd $(KLIB_DIR) && $(MAKE) $(KLIB_FILE)

$(BOOT_FILE_PATH): $(OBJ_LINK_LIST)
	@echo Make boot kernel...
	gcc $^ $(LDFLAGS) -o $@ 

$(IMG_FILE): $(BOOT_FILE_PATH)
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
	rm -rf $(IMG_DIR)/ $(OUT_DIR)/ $(IMG_FILE)
	cd $(KLIB_DIR) && $(MAKE) clean
