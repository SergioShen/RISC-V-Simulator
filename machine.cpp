//
// Name: machine
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/6/18
//

#include "machine.h"
#include <cstring>
#include <elf.h>

char op_strings[39][8] = {"ADD", "MUL", "SUB", "SLL", "MULH", "SLT", "XOR", "DIV", "SRL", "SRA",
                          "OR", "REM", "AND", "LB", "LH", "LW", "LD", "ADDI", "SLLI", "SLTI",
                          "XORI", "SRLI", "SRAI", "ORI", "ANDI", "ADDIW", "JALR", "ECALL", "SB",
                          "SH", "SW", "SD", "BEQ", "BNE", "BLT", "BGE", "AUIPC", "LUI", "JAL"};


inline void Instruction::ImmSignExtend(int num_of_bits) {
    int32_t sign = this->imm & (1 << (num_of_bits - 1));
    sign |= sign << 1;
    sign |= sign << 2;
    sign |= sign << 4;
    sign |= sign << 8;
    sign |= sign << 16;
    this->imm |= sign;
}

inline void Instruction::DecodeRInstruction() {
    // R type instruction
    this->rd = Decode_rd(this->binary_code);
    this->funct3 = Decode_funct3(this->binary_code);
    this->rs1 = Decode_rs1(this->binary_code);
    this->rs2 = Decode_rs2(this->binary_code);
    this->funct7 = Decode_funct7(this->binary_code);
}

inline void Instruction::DecodeIInstruction() {
    // I type instruction
    this->rd = Decode_rd(this->binary_code);
    this->funct3 = Decode_funct3(this->binary_code);
    this->rs1 = Decode_rs1(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 20, 12, 0);
    this->ImmSignExtend(12);
}

inline void Instruction::DecodeSInstruction() {
    // S type instruction
    this->funct3 = Decode_funct3(this->binary_code);
    this->rs1 = Decode_rs1(this->binary_code);
    this->rs2 = Decode_rs2(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 7, 5, 0) + Decode_imm(this->binary_code, 25, 7, 5);
    this->ImmSignExtend(12);
}

inline void Instruction::DecodeSBInstruction() {
    this->funct3 = Decode_funct3(this->binary_code);
    this->rs1 = Decode_rs1(this->binary_code);
    this->rs2 = Decode_rs2(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 8, 4, 1) + Decode_imm(this->binary_code, 25, 6, 5) +
                Decode_imm(this->binary_code, 7, 1, 11) + Decode_imm(this->binary_code, 31, 1, 12);
    this->ImmSignExtend(13);
}

inline void Instruction::DecodeUInstruction() {
    // U type instruction
    this->rd = Decode_rd(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 12, 20, 12);
}

inline void Instruction::DecodeUJInstruction() {
    // UJ type instruction
    this->rd = Decode_rd(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 21, 10, 1) + Decode_imm(this->binary_code, 20, 1, 11) +
                Decode_imm(this->binary_code, 12, 8, 12) + Decode_imm(this->binary_code, 31, 1, 20);
    this->ImmSignExtend(21);
}

void Instruction::Decode() {
    this->opcode = Decode_opcode(this->binary_code);
    switch (this->opcode) {
        case 0x33:
            this->DecodeRInstruction();

            // Set op type
            switch (this->funct3) {
                case 0x0:
                    switch (this->funct7) {
                        case 0x00:
                            this->op_type = OP_ADD;
                            break;
                        case 0x01:
                            this->op_type = OP_MUL;
                            break;
                        case 0x20:
                            this->op_type = OP_SUB;
                            break;
                        default: FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                       this->opcode, this->funct3, this->funct7);
                    }
                    break;
                case 0x1:
                    switch (this->funct7) {
                        case 0x00:
                            this->op_type = OP_SLL;
                            break;
                        case 0x01:
                            this->op_type = OP_MULH;
                            break;
                        default: FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                       this->opcode, this->funct3, this->funct7);
                    }
                    break;
                case 0x2:
                    switch (this->funct7) {
                        case 0x00:
                            this->op_type = OP_SLT;
                            break;
                        default: FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                       this->opcode, this->funct3, this->funct7);
                    }
                    break;
                case 0x4:
                    switch (this->funct7) {
                        case 0x00:
                            this->op_type = OP_XOR;
                            break;
                        case 0x01:
                            this->op_type = OP_DIV;
                            break;
                        default: FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                       this->opcode, this->funct3, this->funct7);
                    }
                    break;
                case 0x5:
                    switch (this->funct7) {
                        case 0x00:
                            this->op_type = OP_SRL;
                            break;
                        case 0X20:
                            this->op_type = OP_SRA;
                            break;
                        default: FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                       this->opcode, this->funct3, this->funct7);
                    }
                    break;
                case 0x6:
                    switch (this->funct7) {
                        case 0x00:
                            this->op_type = OP_OR;
                            break;
                        case 0x01:
                            this->op_type = OP_REM;
                            break;
                        default: FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                       this->opcode, this->funct3, this->funct7);
                    }
                    break;
                case 0x7:
                    switch (this->funct7) {
                        case 0x00:
                            this->op_type = OP_AND;
                            break;
                        default: FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                       this->opcode, this->funct3, this->funct7);
                    }
                    break;
                default: FATAL("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
            }
            break;
        case 0x03:
            this->DecodeIInstruction();

            // Set op type
            switch (this->funct3) {
                case 0x0:
                    this->op_type = OP_LB;
                    break;
                case 0x1:
                    this->op_type = OP_LH;
                    break;
                case 0x2:
                    this->op_type = OP_LW;
                    break;
                case 0x3:
                    this->op_type = OP_LD;
                    break;
                default: FATAL("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
            }
            break;
        case 0x13:
            this->DecodeIInstruction();

            // Set op type
            switch (this->funct3) {
                case 0x0:
                    this->op_type = OP_ADDI;
                    break;
                case 0x1:
                    if (this->funct7 == 0x00)
                        this->op_type = OP_SLLI;
                    else {
                        FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                              this->opcode, this->funct3, this->funct7);
                    }
                    break;
                case 0x2:
                    this->op_type = OP_SLTI;
                    break;
                case 0x4:
                    this->op_type = OP_XORI;
                    break;
                case 0x5:
                    switch (this->funct7) {
                        case 0x00:
                            this->op_type = OP_SRLI;
                            break;
                        case 0x01:
                            this->op_type = OP_SRAI;
                            break;
                        default: FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                       this->opcode, this->funct3, this->funct7);
                    }
                    break;
                case 0x6:
                    this->op_type = OP_ORI;
                    break;
                case 0x7:
                    this->op_type = OP_ANDI;
                    break;
                default: FATAL("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
            }
            break;
        case 0x1B:
            this->DecodeIInstruction();

            // Set op type
            if (this->funct3 == 0x0)
                this->op_type = OP_ADDIW;
            else {
                FATAL("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
            }
            break;
        case 0x67:
            this->DecodeIInstruction();

            // Set op type
            if (this->funct3 == 0x0)
                this->op_type = OP_JALR;
            else {
                FATAL("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
            }
            break;
        case 0x73:
            this->DecodeIInstruction();

            // Set op type
            if (this->funct3 == 0x0 && this->funct7 == 0x00)
                this->op_type = OP_ECALL;
            else {
                FATAL("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                      this->opcode, this->funct3, this->funct7);
            }
            break;
        case 0x23:
            this->DecodeSInstruction();

            // Set op type
            switch (this->funct3) {
                case 0x0:
                    this->op_type = OP_SB;
                    break;
                case 0x1:
                    this->op_type = OP_SH;
                    break;
                case 0x2:
                    this->op_type = OP_SW;
                    break;
                case 0x3:
                    this->op_type = OP_SD;
                    break;
                default: FATAL("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
            }
            break;
        case 0x63:
            this->DecodeSBInstruction();

            // Set op type
            switch (this->funct3) {
                case 0x0:
                    this->op_type = OP_BEQ;
                    break;
                case 0x1:
                    this->op_type = OP_BNE;
                    break;
                case 0x4:
                    this->op_type = OP_BLT;
                    break;
                case 0x5:
                    this->op_type = OP_BGE;
                    break;
                default: FATAL("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
            }
            break;
        case 0x17:
            this->DecodeUInstruction();

            //Set op type
            this->op_type = OP_AUIPC;
            break;
        case 0x37:
            this->DecodeUInstruction();

            //Set op type
            this->op_type = OP_LUI;
            break;
        case 0x6F:
            this->DecodeUJInstruction();

            //Set op type
            this->op_type = OP_JAL;
            break;
        default: FATAL("OP Code %x not implemented\n", this->opcode);
    }
}

Instruction *Machine::FetchInstruction() {
    auto *instruction = new Instruction();
    int64_t instruction_value;
    this->main_memory->ReadMemory(this->pc, sizeof(int32_t), &instruction_value);
    instruction->binary_code = (int32_t) instruction_value;
    this->pc += 4;
    return instruction;
}

void Machine::Execute() {

}

void Machine::AcccessMemory() {

}

void Machine::WriteBack() {

}

Machine::Machine() {
    this->main_memory = new Memory();
    memset(this->registers, 0, sizeof(this->registers));
}

Machine::~Machine() {
    delete main_memory;
}

void Machine::PrintRegisters() {
    printf("\nRegister values:\n");
    printf("RegPC: %16.16lx\n", this->pc);
    for (int i = 0; i < 16; i++) {
        printf("Reg%2.2d: %16.16lx    Reg%2.2d: %16.16lx\n", i, this->registers[i], i + 16, this->registers[i + 16]);
    }
}

void Machine::Run() {

}
