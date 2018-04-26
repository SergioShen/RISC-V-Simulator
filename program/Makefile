GCC=riscv64-unknown-elf-gcc
OBJDUMP=riscv64-unknown-elf-objdump
GCCFLAGS = --static -w
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%,$(SRCS))
DUMPS = $(patsubst %.c,%.disassm,$(SRCS))
BUILD_PATH=bin


all:
	mkdir -p $(BUILD_PATH)
	make $(OBJS)

lib.o: ../lib.c
	$(GCC) $(GCCFLAGS) -c -o $(BUILD_PATH)/lib.o ../lib.c

%: %.c lib.o
	$(GCC) $(GCCFLAGS) -o $(BUILD_PATH)/$@ $< $(BUILD_PATH)/lib.o

dump: $(DUMPS)

%.disassm: $(BUILD_PATH)/%
	$(OBJDUMP) -d $< > $(BUILD_PATH)/$@

clean:
	rm -rf $(BUILD_PATH)
