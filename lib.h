//
// Name: lib
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/7/18
//

#ifndef LIB_H
#define LIB_H

#define RISCV_SYSCALL_EXIT 0
#define RISCV_SYSCALL_PCHAR 1
#define RISCV_SYSCALL_PINT 2
#define RISCV_SYSCALL_PLONG 3
#define RISCV_SYSCALL_PSTRING 4
#define RISCV_SYSCALL_RCHAR 5
#define RISCV_SYSCALL_RINT 6
#define RISCV_SYSCALL_RLONG 7
#define RISCV_SYSCALL_RSTRING 8
#define RISCV_SYSCALL_SRAND 9
#define RISCV_SYSCALL_RAND 10
#define RISCV_SYSCALL_MALLOC 11
#define RISCV_SYSCALL_TIME 12

#define malloc mem_alloc
#define rand rand_int
#define srand set_rand_seed

void exit(int exit_code);

void print_char(char value);

void print_int(int value);

void print_long(long value);

void print_string(char *value);

void read_char(char *address);

void read_int(int *address);

void read_long(long *address);

void read_string(char *address);

void set_rand_seed(int seed);

int rand_int();

void *mem_alloc(long size);

long time();

#endif //LIB_H
