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
    if (Decode_imm(instruction->binary_code, 0, 2, 0) == 0x3)
        this->reg_pc += 4;
    else
        this->reg_pc += 2;
    return instruction;
}

void Machine::Execute(Instruction *instruction) {
    // When this step is going to execute, WriteBack step of i-2 instruction has already done
    // And AccessMemory step of i-1 instruction has already done
    // So the value loaded from memory by i-1 instruction which has not been write to registers will cause data hazard
    // And remember: i-1 instruction has been moved from REG_ACC_MEM to REG_WRITE_BACK
    // When jump occurs, there will be ctrl hazard
    // Only the ctrl hazard will make pipeline stall

    bool jump = false;
    if (instruction != NULL) {
        stats->IncreaseInstruction();
        int32_t imm = instruction->imm;
        int64_t value_rs1 = registers[instruction->rs1],
                value_rs2 = registers[instruction->rs2],
                value_rd = registers[instruction->rd],
                value_sp = registers[REG_sp],
                value_a7 = registers[REG_a7],
                value_a0 = registers[REG_a0];

        if (regs_instr[REG_INSTR_WRITE_BACK] != NULL && regs_instr[REG_INSTR_WRITE_BACK]->write_reg) {
            // Data hazard may happen, new value should be used
            if (regs_instr[REG_INSTR_WRITE_BACK]->rd == instruction->rs1) {
                value_rs1 = regs_instr[REG_INSTR_WRITE_BACK]->write_back_value;
                DEBUG("Use forwarded value from AccMem: %s = %16.16lx\n", reg_strings[instruction->rs1], value_rs1);
            }
            if (regs_instr[REG_INSTR_WRITE_BACK]->rd == instruction->rs2) {
                value_rs2 = regs_instr[REG_INSTR_WRITE_BACK]->write_back_value;
                DEBUG("Use forwarded value from AccMem: %s = %16.16lx\n", reg_strings[instruction->rs2], value_rs2);
            }
            if (regs_instr[REG_INSTR_WRITE_BACK]->rd == instruction->rd) {
                value_rd = regs_instr[REG_INSTR_WRITE_BACK]->write_back_value;
                DEBUG("Use forwarded value from AccMem: %s = %16.16lx\n", reg_strings[instruction->rd], value_rd);
            }
            if (regs_instr[REG_INSTR_WRITE_BACK]->rd == REG_sp) {
                value_sp = regs_instr[REG_INSTR_WRITE_BACK]->write_back_value;
                DEBUG("Use forwarded value from AccMem: %s = %16.16lx\n", reg_strings[REG_sp], value_sp);
            }
            if (regs_instr[REG_INSTR_WRITE_BACK]->rd == REG_a7) {
                value_a7 = regs_instr[REG_INSTR_WRITE_BACK]->write_back_value;
                DEBUG("Use forwarded value from AccMem: %s = %16.16lx\n", reg_strings[REG_a7], value_a7);
            }
            if (regs_instr[REG_INSTR_WRITE_BACK]->rd == REG_a0) {
                value_a0 = regs_instr[REG_INSTR_WRITE_BACK]->write_back_value;
                DEBUG("Use forwarded value from AccMem: %s = %16.16lx\n", reg_strings[REG_a0], value_a0);
            }
        }

        switch (instruction->op_type) {
            case OP_ADD:
                instruction->write_back_value = value_rs1 + value_rs2;
                break;
            case OP_MUL:
                instruction->write_back_value = value_rs1 * value_rs2;
                break;
            case OP_SUB:
                instruction->write_back_value = value_rs1 - value_rs2;
                break;
            case OP_SLL:
                instruction->write_back_value = value_rs1 << (value_rs2 & 0b111111);
                break;
            case OP_SLT:
                instruction->write_back_value = value_rs1 < value_rs2 ? 1 : 0;
                break;
            case OP_XOR:
                instruction->write_back_value = value_rs1 ^ value_rs2;
                break;
            case OP_DIV:
                instruction->write_back_value = value_rs1 / value_rs2;
                break;
            case OP_SRL:
                instruction->write_back_value = ((uint64_t) value_rs1) << (value_rs2 & 0b111111);
                break;
            case OP_SRA:
                instruction->write_back_value = ((int64_t) value_rs1) << (value_rs2 & 0b111111);
                break;
            case OP_OR:
                instruction->write_back_value = value_rs1 | value_rs2;
                break;
            case OP_REM:
                instruction->write_back_value = value_rs1 % value_rs2;
                break;
            case OP_AND:
                instruction->write_back_value = value_rs1 & value_rs2;
                break;
            case OP_LB:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_LH:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_LW:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_LD:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_ADDI:
                if (instruction->instr_type == INSTR_CIW) // ADDI4SPN
                    instruction->write_back_value = (value_sp << 1) + imm;
                else
                    instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_SLLI:
                instruction->write_back_value = value_rs1 << (imm & 0b111111);
                break;
            case OP_SLTI:
                instruction->write_back_value = value_rs1 < imm ? 1 : 0;
                break;
            case OP_XORI:
                instruction->write_back_value = value_rs1 ^ imm;
                break;
            case OP_SRLI:
                instruction->write_back_value = ((uint64_t) value_rs1) >> (imm & 0b111111);
                break;
            case OP_SRAI:
                instruction->write_back_value = ((int64_t) value_rs1) >> (imm & 0b111111);
                break;
            case OP_ORI:
                instruction->write_back_value = value_rs1 | imm;
                break;
            case OP_ANDI:
                instruction->write_back_value = value_rs1 & imm;
                break;
            case OP_ADDIW:
                instruction->write_back_value = (int64_t) ((int32_t) (value_rs1 + imm));
                break;
            case OP_JALR:
                if (instruction->instr_type == INSTR_CR) {
                    instruction->write_back_value = instruction->instr_pc + 2;
                    reg_pc = value_rs1;
                    jump = true;
                } else {
                    instruction->write_back_value = instruction->instr_pc + 4;
                    reg_pc = ((value_rs1 + imm) >> 1) << 1;
                    jump = true;
                }
                break;
            case OP_ECALL:
                this->HandleSystemCall(instruction, value_a7, value_a0);
                break;
            case OP_SB:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_SH:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_SW:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_SD:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_BEQ:
                if (value_rs1 == value_rs2) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BNE:
                if (value_rs1 != value_rs2) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BLT:
                if (value_rs1 < value_rs2) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BGE:
                if (value_rs1 >= value_rs2) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_AUIPC:
                instruction->write_back_value = instruction->instr_pc + imm;
                break;
            case OP_LUI:
                instruction->write_back_value = imm;
                break;
            case OP_JAL:
                instruction->write_back_value = instruction->instr_pc + 4;
                reg_pc = instruction->instr_pc + imm;
                jump = true;
                break;
            case OP_LI:
                instruction->write_back_value = imm;
                break;
            case OP_SUBW:
                instruction->write_back_value = (int64_t) ((int32_t) (value_rd - value_rs2));
                break;
            case OP_ADDW:
                instruction->write_back_value = (int64_t) ((int32_t) (value_rd + value_rs2));
                break;
            case OP_J:
                reg_pc = instruction->instr_pc + imm;
                jump = true;
                break;
            case OP_BEQZ:
                if (value_rs1 == 0) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BNEZ:
                if (value_rs1 != 0) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_LWSP:
                instruction->write_back_value = value_sp + imm;
                break;
            case OP_LDSP:
                instruction->write_back_value = value_sp + imm;
                break;
            case OP_SWSP:
                instruction->write_back_value = value_sp + imm;
                break;
            case OP_SDSP:
                instruction->write_back_value = value_sp + imm;
                break;
            case OP_MV:
                instruction->write_back_value = value_rs2;
                break;
            case OP_BLTU:
                if ((uint64_t) value_rs1 < (uint64_t) value_rs2) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_BGEU:
                if ((uint64_t) value_rs1 >= (uint64_t) value_rs2) {
                    reg_pc = instruction->instr_pc + imm;
                    jump = true;
                }
                break;
            case OP_JR:
                reg_pc = value_rs1;
                jump = true;
                break;
            case OP_SLLIW:
                instruction->write_back_value = (int64_t) (((int32_t) value_rs1) << (imm & 0b11111));
                break;
            case OP_SRLIW:
                instruction->write_back_value = (int64_t) (((uint32_t) value_rs1) >> (imm & 0b11111));
                break;
            case OP_SRAIW:
                instruction->write_back_value = (int64_t) (((int32_t) value_rs1) >> (imm & 0b11111));
                break;
            case OP_SLLW:
                instruction->write_back_value = (int64_t) (((int32_t) value_rs1) << (value_rs2 & 0b11111));
                break;
            case OP_SRLW:
                instruction->write_back_value = (int64_t) (((uint32_t) value_rs1) >> (value_rs2 & 0b11111));
                break;
            case OP_SRAW:
                instruction->write_back_value = (int64_t) (((int32_t) value_rs1) >> (value_rs2 & 0b11111));
                break;
            case OP_LBU:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_LHU:
                instruction->write_back_value = value_rs1 + imm;
                break;
            case OP_MULW:
                instruction->write_back_value = (int64_t) ((int32_t) value_rs1 * (int32_t) value_rs2);
                break;
            default: FATAL("Invalid op type %d\n", instruction->op_type);
        }
    }

    // Move instruction of last pipeline step here
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
    // When this step is going to execute, WriteBack step of i-1 instruction has already done
    // So there won't be any hazard. It's okay to directly load value from registers
    if (instruction != NULL) {
        switch (instruction->op_type) {
            case OP_LB:
                this->main_memory->ReadMemory(instruction->write_back_value, 1, &instruction->write_back_value);
                break;
            case OP_LH:
                this->main_memory->ReadMemory(instruction->write_back_value, 2, &instruction->write_back_value);
                break;
            case OP_LW:
            case OP_LWSP:
                this->main_memory->ReadMemory(instruction->write_back_value, 4, &instruction->write_back_value);
                break;
            case OP_LD:
            case OP_LDSP:
                this->main_memory->ReadMemory(instruction->write_back_value, 8, &instruction->write_back_value);
                break;
            case OP_LBU:
                this->main_memory->ReadMemory(instruction->write_back_value, 1, &instruction->write_back_value);
                instruction->write_back_value = (int64_t) ((uint8_t) instruction->write_back_value);
                break;
            case OP_LHU:
                this->main_memory->ReadMemory(instruction->write_back_value, 2, &instruction->write_back_value);
                instruction->write_back_value = (int64_t) ((uint16_t) instruction->write_back_value);
                break;
            case OP_SB:
                this->main_memory->WriteMemory(instruction->write_back_value, 1, registers[instruction->rs2]);
                break;
            case OP_SH:
                this->main_memory->WriteMemory(instruction->write_back_value, 2, registers[instruction->rs2]);
                break;
            case OP_SW:
            case OP_SWSP:
                this->main_memory->WriteMemory(instruction->write_back_value, 4, registers[instruction->rs2]);
                break;
            case OP_SD:
            case OP_SDSP:
                this->main_memory->WriteMemory(instruction->write_back_value, 8, registers[instruction->rs2]);
                break;
            default:
                break;
        }
    }

    // Move instruction of last pipeline step here
    regs_instr[REG_INSTR_ACCESS_MEM] = regs_instr[REG_INSTR_EXECUTE];
}

void Machine::WriteBack(Instruction *instruction) {
    if (instruction != NULL) {
        if (instruction->write_reg)
            registers[instruction->rd] = instruction->write_back_value;
    }

    // Delete current instruction and move instruction of last pipeline step here
    if (instruction != NULL)
        delete instruction;
    regs_instr[REG_INSTR_WRITE_BACK] = regs_instr[REG_INSTR_ACCESS_MEM];
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

    printf("WrBack  : ");
    if (regs_instr[REG_INSTR_WRITE_BACK] != NULL)
        regs_instr[REG_INSTR_WRITE_BACK]->Print();
    else
        printf("NULL\n");
}

void Machine::DumpState() {
    printf("PC: %16.16lx\n", this->reg_pc);
    printf("HeapPointer: %16.16lx\n", this->heap_pointer);
    this->PrintRegisters();
    stats->PrintStats();
}

void Machine::OneCycle() {
    ASSERT(!this->exit_flag);
    stats->IncreaseCycle();

    this->WriteBack(regs_instr[REG_INSTR_WRITE_BACK]);

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

bool Machine::IsExit() {
    return exit_flag;
}

int64_t Machine::NextToExecute() {
    if (regs_instr[REG_INSTR_EXECUTE] == NULL)
        return NULL;
    else
        return regs_instr[REG_INSTR_EXECUTE]->instr_pc;
}
