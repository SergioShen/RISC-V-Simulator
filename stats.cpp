//
// Name: stats
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/26/18
//

#include "stats.h"

Stats::Stats() {
    num_of_instructions = 0;
    num_of_cycles = 0;
    num_of_stalls_by_ctrl = 0;
    num_of_stalls_by_data = 0;
}

void Stats::PrintStats() {
    printf("\n****************\n");
    printf("Number of instructions: %ld\n", num_of_instructions);
    printf("Number of cycles: %ld\n", num_of_cycles);
    double cpi = num_of_instructions == 0 ? 0 : ((double) num_of_cycles) / ((double) num_of_instructions);
    printf("CPI: %.6lf\n", cpi);
    printf("Stalls caused by ctrl hazard: %ld\n", num_of_stalls_by_ctrl);
    printf("Stalls caused by data hazard: %ld\n", num_of_stalls_by_data);
}

void Stats::IncreaseInstruction() {
    num_of_instructions++;
}

void Stats::IncreaseCycle() {
    num_of_cycles++;
}

void Stats::IncreaseStallByCtrl() {
    num_of_stalls_by_ctrl++;
}

void Stats::IncreaseStallByData() {
    num_of_stalls_by_data++;
}
