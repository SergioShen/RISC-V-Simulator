//
// Name: machine
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/6/18
//

#include "machine.h"
#include <cstring>
#include <elf.h>
#include "stats.h"

extern Stats *stats;
extern char reg_strings[32][8];

Instruction *Machine::FetchInstruction() {
    Instruction *instruction = new Instruction();
    int64_t instruction_value;
    this->main_memory->ReadMemory(this->reg_pc, sizeof(int32_t), &instruction_value);
    instruction->binary_code = (int32_t) instruction_value;
    instruction->instr_pc = reg_pc;
    instruction->decoded = false;
    this->reg_prev_pc = reg_pc;
    if (Decode_imm(instruction->binary_code, 0, 2, 0) == 0x3)
        this->reg_pc += 4;
    else
        this->reg_pc += 2;
    return instruction;
}

void Machine::Execute(Instruction *instruction) {
    bool jump = false;
    if (instruction != NULL) {
        stats->IncreaseInstruction();
        int8_t rs1 = instruction->rs1, rs2 = instruction->rs2, rd = instruction->rd;
        int32_t imm = instruction->imm;
        switch (instruction->op_type) {
            case OP_ADD:
                registers[rd] = registers[rs1] + registers[rs2];
                break;
            case OP_MUL:
                registers[rd] = registers[rs1] * registers[rs2];
                break;
            case OP_SUB:
                registers[rd] = registers[rs1] - registers[rs2];
                break;
            case OP_SLL:
                registers[rd] = registers[rs1] << (registers[rs2] & 0b111111);
                break;
            case OP_SLT:
                registers[rd] = registers[rs1] < registers[rs2] ? 1 : 0;
                break;
            case OP_XOR:
                registers[rd] = registers[rs1] ^ registers[rs2];
                break;
            case OP_DIV:
                registers[rd] = registers[rs1] / registers[rs2];
                break;
            case OP_SRL:
                registers[rd] = ((uint64_t) registers[rs1]) << (registers[rs2] & 0b111111);
                break;
            case OP_SRA:
                registers[rd] = ((int64_t) registers[rs1]) << (registers[rs2] & 0b111111);
                break;
            case OP_OR:
                registers[rd] = registers[rs1] | registers[rs2];
                break;
            case OP_REM:
                registers[rd] = registers[rs1] % registers[rs2];
                break;
            case OP_AND:
                registers[rd] = registers[rs1] & registers[rs2];
                break;
            case OP_LB:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_LH:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_LW:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_LD:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_ADDI:
                if (instruction->instr_type == INSTR_CIW) // ADDI4SPN
                    registers[rd] = (registers[REG_sp] << 1) + imm;
                else
                    registers[rd] = registers[rs1] + imm;
                break;
            case OP_SLLI:
                registers[rd] = registers[rs1] << (imm & 0b111111);
                break;
            case OP_SLTI:
                registers[rd] = registers[rs1] < imm ? 1 : 0;
                break;
            case OP_XORI:
                registers[rd] = registers[rs1] ^ imm;
                break;
            case OP_SRLI:
                registers[rd] = ((uint64_t) registers[rs1]) >> (imm & 0b111111);
                break;
            case OP_SRAI:
                registers[rd] = ((int64_t) registers[rs1]) >> (imm & 0b111111);
                break;
            case OP_ORI:
                registers[rd] = registers[rs1] | imm;
                break;
            case OP_ANDI:
                registers[rd] = registers[rs1] & imm;
                break;
            case OP_ADDIW:
                registers[rd] = (int64_t) ((int32_t) (registers[rs1] + imm));
                break;
            case OP_JALR:
                if (instruction->instr_type == INSTR_CR) {
                    registers[1] = instruction->instr_pc + 2;
                    reg_pc = registers[rs1];
                    jump = true;
                } else {
                    registers[rd] = instruction->instr_pc + 4;
                    reg_pc = ((registers[rs1] + imm) >> 1) << 1;
                    jump = true;
                }
                break;
            case OP_ECALL:
                this->HandleSystemCall();
            case OP_SB:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_SH:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_SW:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_SD:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_BEQ:
                if (registers[rs1] == registers[rs2]) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BNE:
                if (registers[rs1] != registers[rs2]) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BLT:
                if (registers[rs1] < registers[rs2]) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BGE:
                if (registers[rs1] >= registers[rs2]) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_AUIPC:
                registers[rd] = instruction->instr_pc + imm;
                break;
            case OP_LUI:
                registers[rd] = imm;
                break;
            case OP_JAL:
                registers[rd] = instruction->instr_pc + 4;
                reg_pc = instruction->instr_pc + imm;
                jump = true;
                break;
            case OP_LI:
                registers[rd] = imm;
                break;
            case OP_SUBW:
                registers[rd] = (int64_t) ((int32_t) (registers[rd] - registers[rs2]));
                break;
            case OP_ADDW:
                registers[rd] = (int64_t) ((int32_t) (registers[rd] + registers[rs2]));
                break;
            case OP_J:
                reg_pc = instruction->instr_pc + imm;
                jump = true;
                break;
            case OP_BEQZ:
                if (registers[rs1] == 0) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BNEZ:
                if (registers[rs1] != 0) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_LWSP:
                reg_addr = registers[REG_sp] + imm;
                break;
            case OP_LDSP:
                reg_addr = registers[REG_sp] + imm;
                break;
            case OP_SWSP:
                reg_addr = registers[REG_sp] + imm;
                break;
            case OP_SDSP:
                reg_addr = registers[REG_sp] + imm;
                break;
            case OP_MV:
                registers[rd] = registers[rs2];
                break;
            case OP_BLTU:
                if ((uint64_t) registers[rs1] < (uint64_t) registers[rs2]) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BGEU:
                if ((uint64_t) registers[rs1] >= (uint64_t) registers[rs2]) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_JR:
                reg_pc = registers[rs1];
                jump = true;
                break;
            case OP_SLLIW:
                registers[rd] = (int64_t) (((int32_t) registers[rs1]) << (imm & 0b11111));
                break;
            case OP_SRLIW:
                registers[rd] = (int64_t) (((uint32_t) registers[rs1]) >> (imm & 0b11111));
                break;
            case OP_SRAIW:
                registers[rd] = (int64_t) (((int32_t) registers[rs1]) >> (imm & 0b11111));
                break;
            case OP_SLLW:
                registers[rd] = (int64_t) (((int32_t) registers[rs1]) << (registers[rs2] & 0b11111));
                break;
            case OP_SRLW:
                registers[rd] = (int64_t) (((uint32_t) registers[rs1]) >> (registers[rs2] & 0b11111));
                break;
            case OP_SRAW:
                registers[rd] = (int64_t) (((int32_t) registers[rs1]) >> (registers[rs2] & 0b11111));
                break;
            case OP_LBU:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_LHU:
                reg_addr = registers[rs1] + imm;
                break;
            case OP_MULW:
                registers[rd] = (int64_t) ((int32_t) registers[rs1] * (int32_t) registers[rs2]);
                break;
            default: FATAL("Invalid op type %d\n", instruction->op_type);
        }
    }

    // Delete current instruction and move instruction of last pipeline step here
    if (jump) {
        if (regs_instr[REG_INSTR_DECODE] != NULL) {
            delete regs_instr[REG_INSTR_DECODE];
            regs_instr[REG_INSTR_DECODE] = NULL;
        }
        regs_instr[REG_INSTR_EXECUTE] = NULL;
        stats->IncreaseStallByCtrl();
    } else
        regs_instr[REG_INSTR_EXECUTE] = regs_instr[REG_INSTR_DECODE];
}

void Machine::AccessMemory(Instruction *instruction) {
    if (instruction != NULL) {
        switch (instruction->op_type) {
            case OP_LB:
                this->main_memory->ReadMemory(reg_addr, 1, &registers[instruction->rd]);
                break;
            case OP_LH:
                this->main_memory->ReadMemory(reg_addr, 2, &registers[instruction->rd]);
                break;
            case OP_LW:
            case OP_LWSP:
                this->main_memory->ReadMemory(reg_addr, 4, &registers[instruction->rd]);
                break;
            case OP_LD:
            case OP_LDSP:
                this->main_memory->ReadMemory(reg_addr, 8, &registers[instruction->rd]);
                break;
            case OP_LBU:
                this->main_memory->ReadMemory(reg_addr, 1, &registers[instruction->rd]);
                registers[instruction->rd] = (int64_t) ((uint8_t) registers[instruction->rd]);
                break;
            case OP_LHU:
                this->main_memory->ReadMemory(reg_addr, 2, &registers[instruction->rd]);
                registers[instruction->rd] = (int64_t) ((uint16_t) registers[instruction->rd]);
                break;
            case OP_SB:
                this->main_memory->WriteMemory(reg_addr, 1, registers[instruction->rs2]);
                break;
            case OP_SH:
                this->main_memory->WriteMemory(reg_addr, 2, registers[instruction->rs2]);
                break;
            case OP_SW:
            case OP_SWSP:
                this->main_memory->WriteMemory(reg_addr, 4, registers[instruction->rs2]);
                break;
            case OP_SD:
            case OP_SDSP:
                this->main_memory->WriteMemory(reg_addr, 8, registers[instruction->rs2]);
                break;
            default:
                break;
        }
    }

    // Delete current instruction and move instruction of last pipeline step here
    if (instruction != NULL)
        delete instruction;
    regs_instr[REG_INSTR_ACCESS_MEM] = regs_instr[REG_INSTR_EXECUTE];
}

void Machine::SetHeapPointer(int64_t address) {
    this->heap_pointer = address;
}

Machine::Machine() {
    this->main_memory = new Memory();
    memset(this->registers, 0, sizeof(this->registers));
    for (int i = 0; i < SIZE_REG_INSTR; i++)
        this->regs_instr[i] = NULL;
}

Machine::~Machine() {
    delete main_memory;
    for (int i = 0; i < SIZE_REG_INSTR; i++)
        if (this->regs_instr[i] != NULL)
            delete regs_instr[i];
}

void Machine::PrintRegisters() {
    printf("\nRegister values:\n");
    printf("RegPC: %16.16lx\n", this->reg_pc);
    for (int i = 0; i < 16; i++) {
        printf("Reg %4s: %16.16lx    Reg %4s: %16.16lx\n", reg_strings[i], this->registers[i], reg_strings[i + 16],
               this->registers[i + 16]);
    }
}

void Machine::PrintPipeLineInstructions() {
    printf("\n");
    printf("Decode  : ");
    if (regs_instr[REG_INSTR_DECODE] != NULL)
        regs_instr[REG_INSTR_DECODE]->Print();
    else
        printf("NULL\n");

    printf("Execute : ");
    if (regs_instr[REG_INSTR_EXECUTE] != NULL)
        regs_instr[REG_INSTR_EXECUTE]->Print();
    else
        printf("NULL\n");

    printf("AccMem  : ");
    if (regs_instr[REG_INSTR_ACCESS_MEM] != NULL)
        regs_instr[REG_INSTR_ACCESS_MEM]->Print();
    else
        printf("NULL\n");

}

void Machine::DumpState() {
    printf("PC: %16.16lx\n", this->reg_pc);
    printf("PrevPC: %16.16lx\n", this->reg_prev_pc);
    printf("HeapPointer: %16.16lx\n", this->heap_pointer);
    this->PrintRegisters();
    stats->PrintStats();
}

void Machine::OneCycle() {
    ASSERT(!this->exit_flag);
    stats->IncreaseCycle();

    this->AccessMemory(regs_instr[REG_INSTR_ACCESS_MEM]);

    this->Execute(regs_instr[REG_INSTR_EXECUTE]);

    if (this->exit_flag)
        return;

    if (regs_instr[REG_INSTR_DECODE] != NULL)
        if (!regs_instr[REG_INSTR_DECODE]->Decode()) {
            this->DumpState();
            FATAL("Decode error, machine state dumped\n");
        }

    regs_instr[REG_INSTR_DECODE] = this->FetchInstruction();

    registers[REG_zero] = 0;

    if (debug_enabled) {
        this->PrintPipeLineInstructions();
    }
}

void Machine::ReadMemory(int64_t address, int32_t size, int64_t *value) {
    if (!main_memory->ReadMemory(address, size, value)) {
        FATAL("Unable to read memory at %lx", address);
    }
}

void Machine::WriteMemory(int64_t address, int32_t size, int64_t value) {
    if (!main_memory->WriteMemory(address, size, value)) {
        FATAL("Unable to write memory at %lx", address);
    }
}
