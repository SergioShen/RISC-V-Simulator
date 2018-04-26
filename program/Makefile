GCC=riscv64-unknown-elf-gcc
OBJDUMP=riscv64-unknown-elf-objdump
GCCFLAGS = --static
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%,$(SRCS))
DUMPS = $(patsubst %.c,%.disassm,$(SRCS))


all: $(OBJS)

lib.o: ../lib.c
	$(GCC) $(GCCFLAGS) -c ../lib.c

%: %.c lib.o
	$(GCC) $(GCCFLAGS) -o bin/$@ $< lib.o

dump: $(DUMPS)

%.disassm: %
	$(OBJDUMP) -d bin/$< > bin/$@

clean:

	rm lib.o; cd bin; rm -f $(OBJS) *.o *.disassm
