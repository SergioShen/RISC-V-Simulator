//
// Name: lib
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/7/18
//

#include "lib.h"

void exit(int exit_code) {
    asm("mv a0, %0\n\t": : "r"(exit_code):"a0");
    asm("addi a7, zero, 0\n\t");
    asm("ecall\n\t");
}

void print_char(char value) {
    asm("mv a0, %0\n\t": : "r"(value):"a0");
    asm("addi a7, zero, 1\n\t");
    asm("ecall\n\t");
}

void print_int(int value) {
    asm("mv a0, %0\n\t": : "r"(value):"a0");
    asm("addi a7, zero, 2\n\t");
    asm("ecall\n\t");
}

void print_long(long value) {
    asm("mv a0, %0\n\t": : "r"(value):"a0");
    asm("addi a7, zero, 3\n\t");
    asm("ecall\n\t");
}

void print_string(char *value) {
    asm("mv a0, %0\n\t": : "r"(value):"a0");
    asm("addi a7, zero, 4\n\t");
    asm("ecall\n\t");
}

void read_char(char *address) {
    asm("mv a0, %0\n\t": : "r"(address):"a0");
    asm("addi a7, zero, 5\n\t");
    asm("ecall\n\t");
}

void read_int(int *address) {
    asm("mv a0, %0\n\t": : "r"(address):"a0");
    asm("addi a7, zero, 6\n\t");
    asm("ecall\n\t");
}

void read_long(long *address) {
    asm("mv a0, %0\n\t": : "r"(address):"a0");
    asm("addi a7, zero, 7\n\t");
    asm("ecall\n\t");
}

void read_string(char *address) {
    asm("mv a0, %0\n\t": : "r"(address):"a0");
    asm("addi a7, zero, 8\n\t");
    asm("ecall\n\t");
}