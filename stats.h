//
// Name: stats
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/26/18
//

#ifndef RISC_V_SIMULATOR_STATS_H
#define RISC_V_SIMULATOR_STATS_H

#include "utility.h"

class Stats {
private:
    int64_t num_of_instructions;
    int64_t num_of_cycles;
    int64_t num_of_stalls_by_ctrl;
    int64_t num_of_stalls_by_data;
    int64_t num_of_stalls_by_memory;

public:
    Stats();

    // Print all the statistics
    void PrintStats();

    // Add one to instruction number
    void IncreaseInstruction();

    // Add one to cycle number
    void IncreaseCycle();

    // Add one to ctrl stall number
    void IncreaseStallByCtrl();

    // Add one to data stall number
    void IncreaseStallByData();

    // Add to cycle number
    void AddCycle(int cycles);

    // Add to memory stall number
    void AddStallByMemory(int32_t stalls);
};

#endif //RISC_V_SIMULATOR_STATS_H
