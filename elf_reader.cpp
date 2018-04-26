//
// Name: elf_reader
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/6/18
//

#include "elf_reader.h"
#include "machine.h"
#include <fstream>
#include <elf.h>

bool Machine::LoadExecutableFile(const char *file_name) {
    // Local variables
    std::ifstream executable_file;
    Elf64_Ehdr elf_header;
    Elf64_Phdr program_header;
    int64_t value64;

    // Open executable file
    executable_file.open(file_name, std::ios::in | std::ios::binary);

    // Read elf header and check if header info is compatible
    executable_file.read((char *) &elf_header, sizeof(Elf64_Ehdr));
    ASSERT(elf_header.e_ident[EI_MAG0] == 0x7F
           && elf_header.e_ident[EI_MAG1] == 'E'
           && elf_header.e_ident[EI_MAG2] == 'L'
           && elf_header.e_ident[EI_MAG3] == 'F');
    ASSERT(elf_header.e_type == ET_EXEC);
    ASSERT(elf_header.e_machine == EM_RISCV);

    // Set PC as program entry
    this->reg_pc = elf_header.e_entry;
    this->registers[REG_sp] = (int64_t) (1) << 48;
    DEBUG("Number of program headers: %d\n", elf_header.e_phnum);
    DEBUG("Offset to program header table: %ld\n", elf_header.e_phoff);

    // Read each segment and load into memory
    DEBUG("\nSegment table:\n")
    for (int i = 0; i < elf_header.e_phnum; i++) {
        // Read program header
        executable_file.seekg(elf_header.e_phoff + elf_header.e_phentsize * i, std::ios::beg);
        executable_file.read((char *) &program_header, sizeof(Elf64_Phdr));
        DEBUG("Segment %d, address: %16.16lx, offset: %16.16lx\n", i, program_header.p_vaddr, program_header.p_offset);
        DEBUG("\t\tfile size: %16.16lx, memory size: %16.16lx\n", program_header.p_filesz, program_header.p_memsz);

        // Load segment into memory
        executable_file.seekg(program_header.p_offset, std::ios::beg);
        int vi;
        for (vi = 0; vi < program_header.p_filesz / 8; vi++) {
            executable_file.read((char *) &value64, 8);
            this->main_memory->WriteMemory(program_header.p_vaddr + 8 * vi, 8, value64);
        }

        if (vi * 8 < program_header.p_filesz) {
            // p_filesz is not a multiplier of 8
            // There are several bytes that has not been load into memory
            for (vi = vi * 8; vi < program_header.p_filesz; vi++) {
                executable_file.read((char *) &value64, 1);
                this->main_memory->WriteMemory(program_header.p_vaddr + vi, 1, value64);
            }
        }

        if (i == elf_header.e_phnum - 1) {
            // This is the last segment, the heap pointer should be initialized
            int64_t heap_address = RoundUp(program_header.p_vaddr + program_header.p_memsz, PageSizeBitsNum);
            this->SetHeapPointer(heap_address);
        }
    }
}
