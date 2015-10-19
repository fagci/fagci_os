.POSIX:
.PHONY: clean run

CC=g++
AS=nasm
QEMU=qemu-system-i386

MKDIR_P=mkdir -p

ASFLAGS=-f elf32
CFLAGS=-m32 -O2 -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti -c
LDFLAGS=-m32 -nostdlib -T src/linker.ld

SRC_DIR=src
OUT_DIR=out
IMG_DIR=iso

BOOT_FILE=fos.elf
GRUB_CFG=grub.cfg
BOOT_DIR=$(IMG_DIR)/boot
GRUB_DIR=$(BOOT_DIR)/grub
BOOT_FILE_PATH=$(OUT_DIR)/$(BOOT_FILE)
GRUB_CFG_PATH=$(GRUB_DIR)/$(GRUB_CFG)

IMG_FILE=fos.img

all: directories $(IMG_FILE)

directories: $(OUT_DIR)

$(OUT_DIR):
	$(MKDIR_P) $(OUT_DIR)

$(OUT_DIR)/boot.o: $(SRC_DIR)/boot.asm
	$(AS) $(ASFLAGS) $< -o $@

$(OUT_DIR)/kernel.o: $(SRC_DIR)/kernel.cpp
	$(CC) $(CFLAGS) $< -o $@

$(BOOT_FILE_PATH): $(OUT_DIR)/boot.o $(OUT_DIR)/kernel.o
	$(CC) $(LDFLAGS) -o $@ $^

$(IMG_FILE): $(BOOT_FILE_PATH)
	$(MKDIR_P) $(GRUB_DIR)
	echo 'menuentry "myos" {' > $(GRUB_CFG_PATH)
	echo '	'multiboot /boot/$(BOOT_FILE) >> $(GRUB_CFG_PATH)
	echo '}' >> $(GRUB_CFG_PATH)
	cp $< $(BOOT_DIR)
	grub-mkrescue -o $@ $(IMG_DIR)

run: $(IMG_FILE)
	$(QEMU) -hda $<

clean:
	rm -rf $(IMG_DIR)/ $(OUT_DIR)/ $(IMG_FILE)
