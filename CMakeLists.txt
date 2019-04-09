cmake_minimum_required(VERSION 3.5)
project(fos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(src/include)
set(LINK_SCRIPT ${PROJECT_SOURCE_DIR}/src/linker.ld)
set(CMAKE_EXE_LINKER_FLAGS "${DEBUG_FLAGS} -T${LINK_SCRIPT}")
set(SOURCE_FILES
    src/arch/x86/devices/dev.cpp
    src/arch/x86/devices/ps2.cpp
    src/arch/x86/devices/serial.cpp
    src/arch/x86/devices/tty.cpp
    src/arch/x86/interrupts/gdt.cpp
    src/arch/x86/interrupts/idt.cpp
    src/arch/x86/lib/stdio.cpp
    src/arch/x86/lib/string.cpp
    src/arch/x86/sys/hal.cpp
    src/arch/x86/sys/memory.cpp
    src/arch/x86/utils/assert.cpp
    src/arch/x86/utils/debugger.cpp
    src/arch/x86/utils/panic.cpp
    src/base/crti.s
    src/base/crtn.s
    src/include/assert.h
    src/include/debugger.h
    src/include/dev.h
    src/include/gdt.h
    src/include/hal.h
    src/include/idt.h
    src/include/memory.h
    src/include/panic.h
    src/include/ps2.h
    src/include/serial.h
    src/include/stdio.h
    src/include/stdlib.h
    src/include/string.h
    src/include/tty.h
    src/include/vga.h
    src/boot.s
    src/kernel.cpp
    src/linker.ld)

add_executable(fos ${SOURCE_FILES})