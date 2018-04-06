//
// Name: mem
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/5/18
//

#ifndef RISC_V_SIMULATOR_MEM_H
#define RISC_V_SIMULATOR_MEM_H

#include "utility.h"
#include <set>

#define PageSize 4096

class MemoryPage {
public:
    int64_t start_address;
    char *content;

    MemoryPage(int64_t start_address);

    ~MemoryPage();

    // Read memory, size should be 1, 2, 4 or 8
    bool ReadMemory(int64_t address, int32_t size, int64_t *value) const;

    // Write memory, size should be 1, 2, 4 or 8
    bool WriteMemory(int64_t address, int32_t size, int64_t value) const;

    // Check if accessed address is in this page
    bool AddressInPage(int64_t address) const;
};

bool operator<(const MemoryPage &a, const MemoryPage &b);

class Memory {
private:
    std::set<MemoryPage> memory_page_list;

    // Find the page that contains accessed address
    std::set<MemoryPage>::iterator FindPage(int64_t address);

public:
    // Allocate a page that contains given address
    bool AllocatePage(int64_t address);

    // Deallocate a page that contains given address
    bool DeallocatePage(int64_t address);

    // Read memory, size should be 1, 2, 4 or 8
    bool ReadMemory(int64_t address, int32_t size, int64_t *value);

    // Write memory, size should be 1, 2, 4 or 8
    bool WriteMemory(int64_t address, int32_t size, int64_t value);

};

#endif //RISC_V_SIMULATOR_MEM_H
