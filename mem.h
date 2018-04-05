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
    int64 start_address;
    char *content;

    MemoryPage(int64 start_address);

    ~MemoryPage();

    // Read memory, size should be 1, 2, 4 or 8
    bool ReadMemory(int64 address, int32 size, int64 *value) const;

    // Write memory, size should be 1, 2, 4 or 8
    bool WriteMemory(int64 address, int32 size, int64 value) const;

    // Check if accessed address is in this page
    bool AddressInPage(int64 address) const;
};

bool operator<(const MemoryPage &a, const MemoryPage &b);

class Memory {
private:
    std::set<MemoryPage> memory_page_list;

    // Find the page that contains accessed address
    std::set<MemoryPage>::iterator FindPage(int64 address);

public:
    // Allocate a page that contains given address
    bool AllocatePage(int64 address);

    // Deallocate a page that contains given address
    bool DeallocatePage(int64 address);

    // Read memory, size should be 1, 2, 4 or 8
    bool ReadMemory(int64 address, int32 size, int64 *value);

    // Write memory, size should be 1, 2, 4 or 8
    bool WriteMemory(int64 address, int32 size, int64 value);

};

#endif //RISC_V_SIMULATOR_MEM_H
