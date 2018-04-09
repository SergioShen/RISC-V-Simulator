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

void exit(int exit_code);

void print_char(char value);

void print_int(int value);

void print_long(long value);

void print_string(char *value);

void read_char(char *address);

void read_int(int *address);

void read_long(long *address);

void read_string(char *address);

#endif //LIB_H
