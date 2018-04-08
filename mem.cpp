//
// Name: mem
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/5/18
//

#include "mem.h"

#include <cstdio>
#include <cstring>

MemoryPage::MemoryPage(int64_t start_address) : start_address(start_address) {
    this->content = new char[PageSize];
    memset(this->content, 0, PageSize);
}

MemoryPage::~MemoryPage() {
    delete content;
}

bool MemoryPage::ReadMemory(int64_t address, int32_t size, int64_t *value) const {
    // Check if accessed address is in this block
    ASSERT(this->AddressInPage(address) && this->AddressInPage(address + size - 1));

    int64_t index_in_block = address - start_address;
    DEBUG("Read memory, address %16.16lx, size %d\n", address, size);

    // Read memory
    switch (size) {
        case 1:
            *value = this->content[index_in_block];
            break;
        case 2:
            *value = *(int16_t *) (&this->content[index_in_block]);
            break;
        case 4:
            *value = *(int32_t *) (&this->content[index_in_block]);
            break;
        case 8:
            *value = *(int64_t *) (&this->content[index_in_block]);
            break;
        default:
            ASSERT(false);
    }
    DEBUG("\tRead value = %16.16lx\n", *value);

    return true;
}

bool MemoryPage::WriteMemory(int64_t address, int32_t size, int64_t value) const {
    // Check if accessed address is in this block
    ASSERT(this->AddressInPage(address) && this->AddressInPage(address + size - 1));

    int64_t index_in_block = address - start_address;
    DEBUG("Write memory, address %16.16lx, size %d\n", address, size);

    // Write memory
    switch (size) {
        case 1:
            this->content[index_in_block] = (int8_t) value;
            break;
        case 2:
            *(int16_t *) (&this->content[index_in_block]) = (int16_t) value;
            break;
        case 4:
            *(int32_t *) (&this->content[index_in_block]) = (int32_t) value;
            break;
        case 8:
            *(int64_t *) (&this->content[index_in_block]) = value;
            break;
        default:
            ASSERT(false);
    }
    DEBUG("\tWrite value = %16.16lx\n", value);

    return true;
}

bool MemoryPage::AddressInPage(int64_t address) const {
    return address >= this->start_address && address < this->start_address + PageSize;
}

bool operator<(const MemoryPage &a, const MemoryPage &b) {
    return a.start_address < b.start_address;
}

std::set<MemoryPage>::iterator Memory::FindPage(int64_t address) {
    for (auto it = this->memory_page_list.begin(); it != this->memory_page_list.end(); it++) {
        if (it->AddressInPage(address))
            return it;
    }

    // Accessed address is not allocated
    return this->memory_page_list.end();
}

bool Memory::AllocatePage(int64_t address) {
    auto it = this->FindPage(address);

    // We have to insure this address is not allocated
    ASSERT(it == this->memory_page_list.end());

    int64_t start_address = address & (~0xFFF);
    auto *new_page = new MemoryPage(start_address);
    this->memory_page_list.insert(*new_page);

    return true;
}

bool Memory::DeallocatePage(int64_t address) {
    auto it = this->FindPage(address);

    // We have to insure this address is allocated
    ASSERT(it != this->memory_page_list.end());

    this->memory_page_list.erase(it);

    return true;
}


bool Memory::ReadMemory(int64_t address, int32_t size, int64_t *value) {
    auto it = this->FindPage(address);

    // We have to insure this address is allocated
    ASSERT(it != this->memory_page_list.end());

    return it->ReadMemory(address, size, value);
}


bool Memory::WriteMemory(int64_t address, int32_t size, int64_t value) {
    auto it = this->FindPage(address);

    // If address is not allocated, allocate a page
    if (it == this->memory_page_list.end()) {
        bool result = this->AllocatePage(address);
        if (!result) {
            FATAL("Cannot allocate page of address %lx\n", address);
        }
        it = this->FindPage(address);
    }

    return it->WriteMemory(address, size, value);
}