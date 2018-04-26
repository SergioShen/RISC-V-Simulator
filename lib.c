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

void set_rand_seed(int seed) {
    asm("mv a0, %0\n\t": : "r"(seed):"a0");
    asm("addi a7, zero, 9\n\t");
    asm("ecall\n\t");
}

int rand_int() {
    int value;
    asm("addi a7, zero, 10\n\t");
    asm("ecall\n\t");
    asm("mv %0, a7\n\t":"=r"(value)::);
    return value;
}

void *mem_alloc(long size) {
    void *address;
    asm("mv a0, %0\n\t": : "r"(size):"a0");
    asm("addi a7, zero, 11\n\t");
    asm("ecall\n\t");
    asm("mv %0, a7\n\t":"=r"(address)::);
    return address;
}

long time() {
    long time;
    asm("addi a7, zero, 12\n\t");
    asm("ecall\n\t");
    asm("mv %0, a7\n\t":"=r"(time)::);
    return time;
}