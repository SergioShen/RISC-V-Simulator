GCC = g++
GCCFLAGS = -O2 -w

all: riscv-sim
	cd program; make;

riscv-sim: mem.o stats.o instruction.o machine.o elf_reader.o exception.o cache.o config.o memory.o main.o
	$(GCC) $(GCCFLAGS) -o riscv-sim main.o stats.o instruction.o machine.o mem.o elf_reader.o exception.o cache.o config.o memory.o

mem.o: utility.h mem.h mem.cpp
	$(GCC) $(GCCFLAGS) -c mem.cpp

stats.o: utility.h stats.h stats.cpp
	$(GCC) $(GCCFLAGS) -c stats.cpp

instruction.o: utility.h instruction.h instruction.cpp
	$(GCC) $(GCCFLAGS) -c instruction.cpp

machine.o: utility.h mem.h memory.h cache.h config.h machine.h machine.cpp
	$(GCC) $(GCCFLAGS) -c machine.cpp

elf_reader.o: utility.h machine.h elf_reader.h elf_reader.cpp
	$(GCC) $(GCCFLAGS) -c elf_reader.cpp

exception.o: machine.h exception.cpp
	$(GCC) $(GCCFLAGS) -c exception.cpp

cache.o: utility.h storage.h cache.h cache.cpp
	$(GCC) $(GCCFLAGS) -c cache.cpp

config.o: utility.h cache.h config.h config.cpp
	$(GCC) $(GCCFLAGS) -c config.cpp

memory.o: utility.h storage.h memory.h memory.cpp
	$(GCC) $(GCCFLAGS) -c memory.cpp

main.o: utility.h machine.h main.cpp
	$(GCC) $(GCCFLAGS) -c main.cpp

clean:
	rm -f *.o riscv-sim
	cd program; make clean;
