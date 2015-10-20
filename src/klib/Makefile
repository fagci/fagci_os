CC=g++
AS=nasm

ASFLAGS=-f elf32
CFLAGS=-m32 -O2 -Wall -Wextra -ffreestanding -fbuiltin -fno-exceptions -fno-rtti -c
LDFLAGS=-m32 -Wall -Wextra -nostdlib -lgcc -static -shared

C_IN=$(wildcard *.cpp)
C_OUT=$(C_IN:.cpp=.o)

A_IN=$(wildcard *.asm)
A_OUT=$(A_IN:.asm=.o)

all: klib.o

%.o: %.asm
	echo Building asm...
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.cpp
	echo Building cpp...
	$(CC) $(CFLAGS) $< -o $@

klib.o: $(A_OUT) $(C_OUT)
	echo Building klib...
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o klib.o
