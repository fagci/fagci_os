CC=g++
AS=nasm
AR=ar

ASFLAGS=-f elf32
CFLAGS=-m32 -O2 -g -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti -c

C_IN=$(wildcard *.cpp)
C_OUT=$(C_IN:.cpp=.o)

A_IN=$(wildcard *.asm)
A_OUT=$(A_IN:.asm=.o)

all: libk.a

%.o: %.S
	@echo Building asm...
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.cpp
	@echo Building cpp...
	$(CC) $(CFLAGS) $< -o $@

libk.a: $(A_OUT) $(C_OUT)
	@echo Building libk...
	$(AR) rcs $@ $^

clean:
	@rm -f *.o libk.a
