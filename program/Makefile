GCC=riscv64-unknown-elf-gcc
OBJDUMP=riscv64-unknown-elf-objdump
GCCFLAGS = --static

all: test

test: test.o lib.o
	$(GCC) $(GCCFLAGS) -o test test.o lib.o

test.o: test.c
	$(GCC) $(GCCFLAGS) -c test.c

lib.o: lib.c
	$(GCC) $(GCCFLAGS) -c lib.c

dump: test
	$(OBJDUMP) -d test > test.disassm

clean:
	rm -f *.o *.disassm test
