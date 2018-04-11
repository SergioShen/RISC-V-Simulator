GCC=riscv64-unknown-elf-gcc
OBJDUMP=riscv64-unknown-elf-objdump
GCCFLAGS = --static

all: test qsort

test: test.o lib.o
	$(GCC) $(GCCFLAGS) -o test test.o lib.o

qsort: qsort.o lib.o
	$(GCC) $(GCCFLAGS) -o qsort qsort.o lib.o

test.o: test.c
	$(GCC) $(GCCFLAGS) -c test.c

lib.o: lib.c
	$(GCC) $(GCCFLAGS) -c lib.c

qsort.o: qsort.c
	$(GCC) $(GCCFLAGS) -c qsort.c

dump: test qsort
	$(OBJDUMP) -d test > test.disassm
	$(OBJDUMP) -d qsort > qsort.disassm

clean:
	rm -f *.o *.disassm test qsort
