.POSIX:
.PHONY: clean run

QEMU=qemu-system-i386

VPATH=src
MKDIR_P=mkdir -p

ASFLAGS   = --32
LDFLAGS   = -m32 -ffreestanding -nostdlib
CXXFLAGS  = -m32 -ffreestanding -Isrc/include -Wall -Wextra -pedantic -Werror -Wno-unused-parameter

SRC_DIR=src
OUT_DIR=out
IMG_DIR=iso

IMG_FILE=fos.img
BOOT_FILE=fos.elf
GRUB_CFG=grub.cfg
BOOT_DIR=$(IMG_DIR)/boot
GRUB_DIR=$(BOOT_DIR)/grub
BOOT_FILE_PATH=$(OUT_DIR)/$(BOOT_FILE)
GRUB_CFG_PATH=$(GRUB_DIR)/$(GRUB_CFG)

ASM_OBJS := $(patsubst %.s, %.asm.o, $(shell find src -name '*.s'))
C_OBJS   := $(patsubst %.cpp, %.cpp.o, $(shell find src -name '*.cpp'))
OBJS     := $(ASM_OBJS) $(C_OBJS)

OUT_OBJS=$(addprefix $(OUT_DIR)/, $(notdir $(OBJS)))

all: outdir imgdir $(IMG_FILE)

outdir:
	$(MKDIR_P) $(OUT_DIR)

imgdir:
	$(MKDIR_P) $(IMG_DIR)

%.asm.o: %.s
	as $(ASFLAGS) -o $(addprefix $(OUT_DIR)/, $(notdir $@)) $<

%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $(addprefix $(OUT_DIR)/, $(notdir $@)) -c $<

$(BOOT_FILE): $(OBJS)
	$(CXX) $(LDFLAGS) -T $(SRC_DIR)/linker.ld -o $@ $(OUT_OBJS)

$(IMG_FILE): $(BOOT_FILE)
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
	$(RM) -f $(IMG_DIR)/* $(IMG_FILE) $(OUT_OBJECTS)
