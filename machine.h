//
// Name: machine
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/6/18
//

#ifndef RISC_V_SIMULATOR_MACHINE_H
#define RISC_V_SIMULATOR_MACHINE_H

#include "utility.h"
#include "mem.h"
#include "instruction.h"

#define REG_INSTR_DECODE 0
#define REG_INSTR_EXECUTE 1
#define REG_INSTR_ACCESS_MEM 2
#define REG_INSTR_WRITE_BACK 3
#define SIZE_REG_INSTR 4

class Machine {
private:
    bool exit_flag;
    Memory *main_memory;
    Instruction *regs_instr[SIZE_REG_INSTR];
    int64_t registers[32];
    int64_t reg_pc;
    int64_t heap_pointer;

    Instruction *FetchInstruction();

    void Execute(Instruction *instruction);

    void AccessMemory(Instruction *instruction);

    void WriteBack(Instruction *instruction);

    void HandleSystemCall(Instruction *instruction, int64_t system_call_number, int64_t system_call_arg);

    void SetHeapPointer(int64_t address);

public:

    Machine();

    ~Machine();

    bool LoadExecutableFile(const char *file_name);

    void PrintRegisters();

    void PrintPipeLineInstructions();

    void OneCycle();

    void DumpState();

    void ReadMemory(int64_t address, int32_t size, int64_t *value);

    void WriteMemory(int64_t address, int32_t size, int64_t value);

    bool IsExit() {
        return exit_flag;
    }

    int64_t NextToExecute() {
        if (regs_instr[REG_INSTR_EXECUTE] == NULL)
            return NULL;
        else
            return regs_instr[REG_INSTR_EXECUTE]->instr_pc;
    }
};

#endif //RISC_V_SIMULATOR_MACHINE_H
