//
// Name: exception
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/9/18
//

#include "machine.h"
#include <cstdio>

#define RISCV_SYSCALL_EXIT 0
#define RISCV_SYSCALL_PCHAR 1
#define RISCV_SYSCALL_PINT 2
#define RISCV_SYSCALL_PLONG 3
#define RISCV_SYSCALL_PSTRING 4
#define RISCV_SYSCALL_RCHAR 5
#define RISCV_SYSCALL_RINT 6
#define RISCV_SYSCALL_RLONG 7
#define RISCV_SYSCALL_RSTRING 8

void Machine::HandleSystemCall() {
    // TODO: System call handler
    int8_t buffer[2048];
    int32_t i;
    union {
        int8_t value_8;
        int32_t value_32;
        int64_t value_64;
    } temp_value;
    switch (registers[REG_a7]) {
        case RISCV_SYSCALL_EXIT:
            this->exit_flag = true;
            printf("Process finished with exit code %d\n", registers[REG_a0]);
            break;
        case RISCV_SYSCALL_PCHAR:
            putchar((int8_t) registers[REG_a0]);
            break;
        case RISCV_SYSCALL_PINT:
            printf("%d", (int32_t) registers[REG_a0]);
            break;
        case RISCV_SYSCALL_PLONG:
            printf("%ld", (int64_t) registers[REG_a0]);
            break;
        case RISCV_SYSCALL_PSTRING:
            // Copy string from the main memory of simulator to buffer
            i = 0;
            do {
                this->main_memory->ReadMemory(registers[REG_a0] + i, 1, &temp_value.value_64);
                buffer[i] = temp_value.value_8;
            } while (buffer[i++] != '\0');

            printf("%s", buffer);
            break;
        case RISCV_SYSCALL_RCHAR:
            temp_value.value_32 = getchar();
            this->main_memory->WriteMemory(registers[REG_a0], 1, temp_value.value_64);
            break;
        case RISCV_SYSCALL_RINT:
            scanf("%d", &temp_value.value_32);
            this->main_memory->WriteMemory(registers[REG_a0], 4, temp_value.value_64);
            break;
        case RISCV_SYSCALL_RLONG:
            scanf("%d", &temp_value.value_64);
            this->main_memory->WriteMemory(registers[REG_a0], 8, temp_value.value_64);
            break;
        case RISCV_SYSCALL_RSTRING:
            // Read string to buffer from stdin
            scanf("%s", buffer);
            // Copy string from buffer to the main memory of simulator
            i = 0;
            do {
                this->main_memory->WriteMemory(registers[REG_a0] + i, 1, (int64_t) buffer[i]);
            } while (buffer[i++] != '\0');
            break;
        default: FATAL("Invalid system call number: %d\n", registers[REG_a7]);
    }
}



