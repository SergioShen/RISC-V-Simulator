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
#include "memory.h"
#include "cache.h"

#define REG_INSTR_DECODE 0
#define REG_INSTR_EXECUTE 1
#define REG_INSTR_ACCESS_MEM 2
#define REG_INSTR_WRITE_BACK 3
#define SIZE_REG_INSTR 4

class Machine {
private:
    bool exit_flag;                             // exit flag to indicate if the program should exit
    Memory *main_memory;                        // memory with memory management
    Instruction *regs_instr[SIZE_REG_INSTR];    // Save instructions for every pipeline stage
    int64_t registers[32];                      // register file
    int64_t reg_pc;                             // pc register
    int64_t heap_pointer;                       // points to top of heap
    MemoryForCache *memory;                     // memory for cache use
    Cache *l1;                                  // L1 cache
    Cache *l2;                                  // L1 cache
    Cache *l3;                                  // L1 cache

    // Fetch stage of pipeline
    Instruction *FetchInstruction();

    // Execute stage of pipeline
    void Execute(Instruction *instruction);

    // Access Memory stage of pipeline
    void AccessMemory(Instruction *instruction);

    // Write Back stage of pipeline
    void WriteBack(Instruction *instruction);

    // System call handler
    void HandleSystemCall(Instruction *instruction, int64_t system_call_number, int64_t system_call_arg);

    // Initialize the heap pointer
    void SetHeapPointer(int64_t address);

public:

    Machine();

    ~Machine();

    // Load executable file into memory
    bool LoadExecutableFile(const char *file_name);

    // Print values of all registers
    void PrintRegisters();

    // Print instructions of all pipeline stage
    void PrintPipeLineInstructions();

    // Run a cycle
    void OneCycle();

    // Print machine status
    void DumpState();

    // Read memory
    void ReadMemory(int64_t address, int32_t size, int64_t *value);

    // Write memory
    void WriteMemory(int64_t address, int32_t size, int64_t value);

    // Is machine going to exit?
    bool IsExit();

    // Get next instruction which is going to be processed in Execute stage
    int64_t NextToExecute();

    void PrintCacheStats();
};

#endif //RISC_V_SIMULATOR_MACHINE_H
