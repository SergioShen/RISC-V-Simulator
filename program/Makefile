GCC=riscv64-unknown-elf-gcc
OBJDUMP=riscv64-unknown-elf-objdump
GCCFLAGS = --static

all: test qsort ackermann matrix

test: test.o lib.o
	$(GCC) $(GCCFLAGS) -o test test.o lib.o

qsort: qsort.o lib.o
	$(GCC) $(GCCFLAGS) -o qsort qsort.o lib.o

ackermann: ackermann.o lib.o
	$(GCC) $(GCCFLAGS) -o ackermann ackermann.o lib.o

matrix: matrix.o lib.o
	$(GCC) $(GCCFLAGS) -o matrix matrix.o lib.o

test.o: test.c
	$(GCC) $(GCCFLAGS) -c test.c

lib.o: lib.c
	$(GCC) $(GCCFLAGS) -c lib.c

qsort.o: qsort.c
	$(GCC) $(GCCFLAGS) -c qsort.c

ackermann.o: ackermann.c
	$(GCC) $(GCCFLAGS) -c ackermann.c

matrix.o: matrix.c
	$(GCC) $(GCCFLAGS) -c matrix.c

dump: test qsort
	$(OBJDUMP) -d test > test.disassm
	$(OBJDUMP) -d qsort > qsort.disassm
	$(OBJDUMP) -d ackermann > ackermann.disassm
	$(OBJDUMP) -d matrix > matrix.disassm

clean:
	rm -f *.o *.disassm test qsort ackermann matrix
