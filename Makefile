GCC = g++
GCCFLAGS = -O2 -w

all: rsv-sim

rsv-sim: mem.o machine.o elf_reader.o exception.o main.o
	$(GCC) $(GCCFLAGS) -o rsv-sim main.o machine.o mem.o elf_reader.o exception.o

mem.o: utility.h mem.h mem.cpp
	$(GCC) $(GCCFLAGS) -c mem.cpp

machine.o: utility.h mem.h machine.h machine.cpp
	$(GCC) $(GCCFLAGS) -c machine.cpp

elf_reader.o: utility.h machine.h elf_reader.h elf_reader.cpp
	$(GCC) $(GCCFLAGS) -c elf_reader.cpp

exception.o: machine.h exception.cpp
	$(GCC) $(GCCFLAGS) -c exception.cpp

main.o: utility.h machine.h main.cpp
	$(GCC) $(GCCFLAGS) -c main.cpp

clean:
	rm -f *.o rsv-sim