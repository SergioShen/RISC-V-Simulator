//
// Name: instruction
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/26/18
//

#include "instruction.h"

char op_strings[][8] = {"ADD", "MUL", "SUB", "SLL", "MULH", "SLT", "XOR", "DIV", "SRL", "SRA",
                        "OR", "REM", "AND", "LB", "LH", "LW", "LD", "ADDI", "SLLI", "SLTI",
                        "XORI", "SRLI", "SRAI", "ORI", "ANDI", "ADDIW", "JALR", "ECALL", "SB",
                        "SH", "SW", "SD", "BEQ", "BNE", "BLT", "BGE", "AUIPC", "LUI", "JAL",
                        "LI", "SUBW", "ADDW", "J", "BEQZ", "BNEZ", "LWSP", "LDSP", "SWSP", "SDSP",
                        "MV", "BLTU", "BGEU", "JR", "SLLIW", "SRLIW", "SRAIW", "SLLW", "SRLW",
                        "SRAW", "LBU", "LHU", "MULW"

};

char reg_strings[32][8] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
                           "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
                           "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
                           "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",
};

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
    this->instr_type = INSTR_R;
    this->rd = Decode_rd(this->binary_code);
    this->funct3 = Decode_funct3(this->binary_code);
    this->rs1 = Decode_rs1(this->binary_code);
    this->rs2 = Decode_rs2(this->binary_code);
    this->funct7 = Decode_funct7(this->binary_code);
}

inline void Instruction::DecodeIInstruction() {
    // I type instruction
    this->instr_type = INSTR_I;
    this->rd = Decode_rd(this->binary_code);
    this->funct3 = Decode_funct3(this->binary_code);
    this->rs1 = Decode_rs1(this->binary_code);
    this->funct7 = Decode_funct7(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 20, 12, 0);
    this->ImmSignExtend(12);
}

inline void Instruction::DecodeSInstruction() {
    // S type instruction
    this->instr_type = INSTR_S;
    this->funct3 = Decode_funct3(this->binary_code);
    this->rs1 = Decode_rs1(this->binary_code);
    this->rs2 = Decode_rs2(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 7, 5, 0) + Decode_imm(this->binary_code, 25, 7, 5);
    this->ImmSignExtend(12);
}

inline void Instruction::DecodeSBInstruction() {
    // SB type instruction
    this->instr_type = INSTR_SB;
    this->funct3 = Decode_funct3(this->binary_code);
    this->rs1 = Decode_rs1(this->binary_code);
    this->rs2 = Decode_rs2(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 8, 4, 1) + Decode_imm(this->binary_code, 25, 6, 5) +
                Decode_imm(this->binary_code, 7, 1, 11) + Decode_imm(this->binary_code, 31, 1, 12);
    this->ImmSignExtend(13);
}

inline void Instruction::DecodeUInstruction() {
    // U type instruction
    this->instr_type = INSTR_U;
    this->rd = Decode_rd(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 12, 20, 12);
}

inline void Instruction::DecodeUJInstruction() {
    // UJ type instruction
    this->instr_type = INSTR_UJ;
    this->rd = Decode_rd(this->binary_code);
    this->imm = Decode_imm(this->binary_code, 21, 10, 1) + Decode_imm(this->binary_code, 20, 1, 11) +
                Decode_imm(this->binary_code, 12, 8, 12) + Decode_imm(this->binary_code, 31, 1, 20);
    this->ImmSignExtend(21);
}

bool Instruction::Decode() {
    bool return_value = true;
    // Test if instruction is compressed type or not
    if (Decode_c_opcode(this->binary_code) == 3) {
        // This is not a compressed instruction
        this->opcode = Decode_opcode(this->binary_code);
        switch (this->opcode) {
            case 0x33:
                this->DecodeRInstruction();
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
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                      this->opcode, this->funct3, this->funct7);
                                return_value = false;
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
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                      this->opcode, this->funct3, this->funct7);
                                return_value = false;
                        }
                        break;
                    case 0x2:
                        switch (this->funct7) {
                            case 0x00:
                                this->op_type = OP_SLT;
                                break;
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                      this->opcode, this->funct3, this->funct7);
                                return_value = false;
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
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                      this->opcode, this->funct3, this->funct7);
                                return_value = false;
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
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                      this->opcode, this->funct3, this->funct7);
                                return_value = false;
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
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                      this->opcode, this->funct3, this->funct7);
                                return_value = false;
                        }
                        break;
                    case 0x7:
                        switch (this->funct7) {
                            case 0x00:
                                this->op_type = OP_AND;
                                break;
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                      this->opcode, this->funct3, this->funct7);
                                return_value = false;
                        }
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x3B:
                this->DecodeRInstruction();
                switch (this->funct3) {
                    case 0x0:
                        if (this->funct7 == 0x00) {
                            this->op_type = OP_ADDW;
                        } else if (this->funct7 == 0x01) {
                            this->op_type = OP_MULW;
                        } else if (this->funct7 == 0x20) {
                            this->op_type = OP_SUBW;
                        } else {
                            DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                  this->opcode, this->funct3, this->funct7);
                            return_value = false;
                        }
                        break;
                    case 0x1:
                        if (this->funct7 == 0x00) {
                            this->op_type = OP_SLLW;
                        } else {
                            DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                  this->opcode, this->funct3, this->funct7);
                            return_value = false;
                        }
                        break;
                    case 0x5:
                        if (this->funct7 == 0x00) {
                            this->op_type = OP_SRLW;
                        } else if (this->funct7 == 0x20) {
                            this->op_type = OP_SRAW;
                        } else {
                            DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                  this->opcode, this->funct3, this->funct7);
                            return_value = false;
                        }
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x03:
                this->DecodeIInstruction();
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
                    case 0x4:
                        this->op_type = OP_LBU;
                        break;
                    case 0x5:
                        this->op_type = OP_LHU;
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x13:
                this->DecodeIInstruction();
                switch (this->funct3) {
                    case 0x0:
                        this->op_type = OP_ADDI;
                        break;
                    case 0x1:
                        if (this->funct7 == 0x00)
                            this->op_type = OP_SLLI;
                        else {
                            DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                  this->opcode, this->funct3, this->funct7);
                            return_value = false;
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
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                      this->opcode, this->funct3, this->funct7);
                                return_value = false;
                        }
                        break;
                    case 0x6:
                        this->op_type = OP_ORI;
                        break;
                    case 0x7:
                        this->op_type = OP_ANDI;
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x1B:
                this->DecodeIInstruction();
                switch (this->funct3) {
                    case 0x0:
                        this->op_type = OP_ADDIW;
                        break;
                    case 0x1:
                        this->op_type = OP_SLLIW;
                        break;
                    case 0x5:
                        if (this->funct7 == 0x00) {
                            this->op_type = OP_SRLIW;
                        } else if (this->funct7 == 0x20) {
                            this->op_type = OP_SRAIW;
                        } else {
                            DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                                  this->opcode, this->funct3, this->funct7);
                            return_value = false;
                        }
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x67:
                this->DecodeIInstruction();
                if (this->funct3 == 0x0)
                    this->op_type = OP_JALR;
                else {
                    DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                    return_value = false;
                }
                break;
            case 0x73:
                this->DecodeIInstruction();
                if (this->funct3 == 0x0 && this->funct7 == 0x00)
                    this->op_type = OP_ECALL;
                else {
                    DEBUG("OP Code %x, Funct3 %x, Funct7 %x not implemented\n",
                          this->opcode, this->funct3, this->funct7);
                    return_value = false;
                }
                break;
            case 0x23:
                this->DecodeSInstruction();
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
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x63:
                this->DecodeSBInstruction();
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
                    case 0x6:
                        this->op_type = OP_BLTU;
                        break;
                    case 0x7:
                        this->op_type = OP_BGEU;
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x17:
                this->DecodeUInstruction();
                this->op_type = OP_AUIPC;
                break;
            case 0x37:
                this->DecodeUInstruction();
                this->op_type = OP_LUI;
                break;
            case 0x6F:
                this->DecodeUJInstruction();
                this->op_type = OP_JAL;
                break;
            default:
                DEBUG("OP Code %x not implemented\n", this->opcode);
                return_value = false;
        }
    } else {
        // This is a compressed instruction
        // Since compressed instructions are not uniform (compared to normal instructions)
        // We don't use uniform decode functions for each CXX type
        this->opcode = Decode_c_opcode(this->binary_code);
        this->funct3 = Decode_c_funct3(this->binary_code);
        switch (this->opcode) {
            case 0x0:
                switch (this->funct3) {
                    case 0x0:
                        this->instr_type = INSTR_CIW;
                        this->rd = Decode_cc_rd(this->binary_code);
                        this->rs1 = REG_sp;
                        this->op_type = OP_ADDI; // ADDI4SPN
                        this->imm = Decode_imm(this->binary_code, 6, 1, 2) + Decode_imm(this->binary_code, 5, 1, 3) +
                                    Decode_imm(this->binary_code, 11, 2, 4) + Decode_imm(this->binary_code, 7, 4, 6);
                        break;
                    case 0x2:
                        this->instr_type = INSTR_CL;
                        this->rs1 = Decode_cc_rs1(this->binary_code);
                        this->rd = Decode_cc_rd(this->binary_code);
                        this->op_type = OP_LW;
                        this->imm = Decode_imm(this->binary_code, 6, 1, 2) + Decode_imm(this->binary_code, 10, 3, 3) +
                                    Decode_imm(this->binary_code, 5, 1, 6);
                        break;
                    case 0x3:
                        this->instr_type = INSTR_CL;
                        this->rs1 = Decode_cc_rs1(this->binary_code);
                        this->rd = Decode_cc_rd(this->binary_code);
                        this->op_type = OP_LD;
                        this->imm = Decode_imm(this->binary_code, 10, 3, 3) + Decode_imm(this->binary_code, 5, 2, 6);
                        break;
                    case 0x6:
                        this->instr_type = INSTR_CS;
                        this->rs1 = Decode_cc_rs1(this->binary_code);
                        this->rs2 = Decode_cc_rd(this->binary_code);
                        this->op_type = OP_SW;
                        this->imm = Decode_imm(this->binary_code, 6, 1, 2) + Decode_imm(this->binary_code, 10, 3, 3) +
                                    Decode_imm(this->binary_code, 5, 1, 6);
                        break;
                    case 0x7:
                        this->instr_type = INSTR_CS;
                        this->rs1 = Decode_cc_rs1(this->binary_code);
                        this->rs2 = Decode_cc_rd(this->binary_code);
                        this->op_type = OP_SD;
                        this->imm = Decode_imm(this->binary_code, 10, 3, 3) + Decode_imm(this->binary_code, 5, 2, 6);
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x1:
                switch (this->funct3) {
                    case 0x0:
                        this->instr_type = INSTR_CI;
                        this->rd = Decode_c_rd(this->binary_code);
                        this->rs1 = Decode_c_rs1(this->binary_code);
                        this->op_type = OP_ADDI;
                        this->imm = Decode_imm(this->binary_code, 2, 5, 0) + Decode_imm(this->binary_code, 12, 1, 5);
                        this->ImmSignExtend(6);
                        break;
                    case 0x1:
                        this->instr_type = INSTR_CI;
                        this->rd = Decode_c_rd(this->binary_code);
                        this->rs1 = Decode_c_rs1(this->binary_code);
                        this->op_type = OP_ADDIW;
                        this->imm = Decode_imm(this->binary_code, 2, 5, 0) + Decode_imm(this->binary_code, 12, 1, 5);
                        this->ImmSignExtend(6);
                        break;
                    case 0x2:
                        this->instr_type = INSTR_CI;
                        this->rd = Decode_c_rd(this->binary_code);
                        this->op_type = OP_LI;
                        this->imm = Decode_imm(this->binary_code, 2, 5, 0) + Decode_imm(this->binary_code, 12, 1, 5);
                        this->ImmSignExtend(6);
                        break;
                    case 0x3:
                        this->instr_type = INSTR_CI;
                        this->rd = Decode_c_rd(this->binary_code);
                        if (this->rd == 2) {
                            this->op_type = OP_ADDI; // ADDI16SP
                            this->rs1 = REG_sp;
                            this->imm =
                                    Decode_imm(this->binary_code, 6, 1, 4) + Decode_imm(this->binary_code, 2, 1, 5) +
                                    Decode_imm(this->binary_code, 5, 1, 6) + Decode_imm(this->binary_code, 3, 2, 7) +
                                    Decode_imm(this->binary_code, 12, 1, 9);
                            this->ImmSignExtend(10);
                        } else {
                            this->op_type = OP_LUI;
                            this->imm =
                                    Decode_imm(this->binary_code, 2, 5, 12) + Decode_imm(this->binary_code, 12, 1, 17);
                            this->ImmSignExtend(18);
                        }
                        break;
                    case 0x4:
                        switch (Decode_imm(this->binary_code, 10, 2, 0)) {
                            case 0x0:
                                this->instr_type = INSTR_CB;
                                this->rs1 = Decode_cc_rs1(this->binary_code);
                                this->rd = Decode_cc_rs1(this->binary_code);
                                this->op_type = OP_SRLI;
                                this->imm = Decode_imm(this->binary_code, 2, 5, 0) +
                                            Decode_imm(this->binary_code, 12, 1, 5);
                                break;
                            case 0x1:
                                this->instr_type = INSTR_CB;
                                this->rs1 = Decode_cc_rs1(this->binary_code);
                                this->rd = Decode_cc_rs1(this->binary_code);
                                this->op_type = OP_SRAI;
                                this->imm = Decode_imm(this->binary_code, 2, 5, 0) +
                                            Decode_imm(this->binary_code, 12, 1, 5);
                                break;
                            case 0x2:
                                this->instr_type = INSTR_CB;
                                this->rs1 = Decode_cc_rs1(this->binary_code);
                                this->rd = Decode_cc_rs1(this->binary_code);
                                this->op_type = OP_ANDI;
                                this->imm = Decode_imm(this->binary_code, 2, 5, 0) +
                                            Decode_imm(this->binary_code, 12, 1, 5);
                                this->ImmSignExtend(6);
                                break;
                            case 0x3:
                                this->instr_type = INSTR_CS;
                                this->rd = Decode_cc_rs1(this->binary_code);
                                this->rs1 = Decode_cc_rs1(this->binary_code);
                                this->rs2 = Decode_cc_rd(this->binary_code);
                                switch (Decode_imm(this->binary_code, 12, 1, 0)) {
                                    case 0x0:
                                        switch (Decode_imm(this->binary_code, 5, 2, 0)) {
                                            case 0x0:
                                                this->op_type = OP_SUB;
                                                break;
                                            case 0x1:
                                                this->op_type = OP_XOR;
                                                break;
                                            case 0x2:
                                                this->op_type = OP_OR;
                                                break;
                                            case 0x3:
                                                this->op_type = OP_AND;
                                                break;
                                            default:
                                                DEBUG("OP Code %x, Funct6 %x, Funct %x not implemented\n",
                                                      Decode_imm(this->binary_code, 10, 6, 0),
                                                      Decode_imm(this->binary_code, 5, 2, 0));
                                                return_value = false;
                                        }
                                        break;
                                    case 0x1:
                                        switch (Decode_imm(this->binary_code, 5, 2, 0)) {
                                            case 0x0:
                                                this->op_type = OP_SUBW;
                                                break;
                                            case 0x1:
                                                this->op_type = OP_ADDW;
                                                break;
                                            default:
                                                DEBUG("OP Code %x, Funct6 %x, Funct %x not implemented\n",
                                                      Decode_imm(this->binary_code, 10, 6, 0),
                                                      Decode_imm(this->binary_code, 5, 2, 0));
                                                return_value = false;
                                        }
                                        break;
                                    default:
                                        DEBUG("OP Code %x, Funct6 %x not implemented\n",
                                              Decode_imm(this->binary_code, 10, 6, 0));
                                        return_value = false;
                                }
                                break;
                            default:
                                DEBUG("OP Code %x, Funct3 %x, Funct2 %x not implemented\n",
                                      this->opcode, this->funct3, Decode_imm(this->binary_code, 10, 2, 0));
                                return_value = false;
                        }
                        break;
                    case 0x5:
                        this->instr_type = INSTR_CB;
                        this->op_type = OP_J;
                        this->imm = Decode_imm(this->binary_code, 3, 3, 1) + Decode_imm(this->binary_code, 11, 1, 4) +
                                    Decode_imm(this->binary_code, 2, 1, 5) + Decode_imm(this->binary_code, 7, 1, 6) +
                                    Decode_imm(this->binary_code, 6, 1, 7) + Decode_imm(this->binary_code, 9, 2, 8) +
                                    Decode_imm(this->binary_code, 8, 1, 10) + Decode_imm(this->binary_code, 12, 1, 11);
                        this->ImmSignExtend(11);
                        break;
                    case 0x6:
                        this->instr_type = INSTR_CB;
                        this->rs1 = Decode_cc_rs1(this->binary_code);
                        this->op_type = OP_BEQZ;
                        this->imm = Decode_imm(this->binary_code, 3, 2, 1) + Decode_imm(this->binary_code, 10, 2, 3) +
                                    Decode_imm(this->binary_code, 2, 1, 5) + Decode_imm(this->binary_code, 5, 2, 6) +
                                    Decode_imm(this->binary_code, 12, 1, 8);
                        break;
                    case 0x7:
                        this->instr_type = INSTR_CB;
                        this->rs1 = Decode_cc_rs1(this->binary_code);
                        this->op_type = OP_BNEZ;
                        this->imm = Decode_imm(this->binary_code, 3, 2, 1) + Decode_imm(this->binary_code, 10, 2, 3) +
                                    Decode_imm(this->binary_code, 2, 1, 5) + Decode_imm(this->binary_code, 5, 2, 6) +
                                    Decode_imm(this->binary_code, 12, 1, 8);
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            case 0x2:
                switch (this->funct3) {
                    case 0x0:
                        this->instr_type = INSTR_CI;
                        this->rd = Decode_c_rd(this->binary_code);
                        this->rs1 = Decode_c_rs1(this->binary_code);
                        this->op_type = OP_SLLI;
                        this->imm = Decode_imm(this->binary_code, 2, 5, 0) +
                                    Decode_imm(this->binary_code, 12, 1, 5);
                        break;
                    case 0x2:
                        this->instr_type = INSTR_CI;
                        this->rd = Decode_c_rd(this->binary_code);
                        this->op_type = OP_LWSP;
                        this->imm = Decode_imm(this->binary_code, 4, 3, 2) +
                                    Decode_imm(this->binary_code, 12, 1, 5) +
                                    Decode_imm(this->binary_code, 2, 2, 6);
                        break;
                    case 0x3:
                        this->instr_type = INSTR_CI;
                        this->rd = Decode_c_rd(this->binary_code);
                        this->op_type = OP_LDSP;
                        this->imm = Decode_imm(this->binary_code, 5, 2, 3) +
                                    Decode_imm(this->binary_code, 12, 1, 5) +
                                    Decode_imm(this->binary_code, 2, 3, 6);
                        break;
                    case 0x4:
                        this->instr_type = INSTR_CB;
                        this->rd = Decode_c_rd(this->binary_code);
                        this->rs1 = Decode_c_rs1(this->binary_code);
                        this->rs2 = Decode_c_rs2(this->binary_code);
                        if (Decode_imm(this->binary_code, 12, 1, 0) == 0) {
                            if (this->rs2 == 0) {
                                this->instr_type = INSTR_CR;
                                this->op_type = OP_JR;
                            } else
                                this->op_type = OP_MV;
                        } else if (Decode_imm(this->binary_code, 12, 1, 0) == 1) {
                            if (this->rs2 == 0) {
                                this->instr_type = INSTR_CR;
                                this->op_type = OP_JALR;
                            } else
                                this->op_type = OP_ADD;
                        } else {
                            DEBUG("OP Code %x, Funct4 %x not implemented\n", Decode_imm(this->binary_code, 10, 4, 0));
                            return_value = false;
                        }
                        break;
                    case 0x6:
                        this->instr_type = INSTR_CSS;
                        this->rs2 = Decode_c_rs2(this->binary_code);
                        this->op_type = OP_SWSP;
                        this->imm = Decode_imm(this->binary_code, 9, 4, 2) + Decode_imm(this->binary_code, 7, 2, 6);
                        break;
                    case 0x7:
                        this->instr_type = INSTR_CSS;
                        this->rs2 = Decode_c_rs2(this->binary_code);
                        this->op_type = OP_SDSP;
                        this->imm = Decode_imm(this->binary_code, 10, 3, 3) + Decode_imm(this->binary_code, 7, 3, 6);
                        break;
                    default:
                        DEBUG("OP Code %x, Funct3 %x not implemented\n", this->opcode, this->funct3);
                        return_value = false;
                }
                break;
            default:
                DEBUG("OP Code %x not implemented\n", this->opcode);
                return_value = false;
        }
    }
    decoded = true;
    return return_value;
}

void Instruction::Print() {
    printf("PC=%16.16lx  ", instr_pc);
    if (this->decoded) {
        switch (this->instr_type) {
            case INSTR_R:
                printf("%8s %s, %s, %s\n", op_strings[op_type], reg_strings[rd], reg_strings[rs1], reg_strings[rs2]);
                break;
            case INSTR_I:
                switch (this->op_type) {
                    case OP_LB:
                    case OP_LH:
                    case OP_LW:
                    case OP_LD:
                        printf("%8s %s, %d(%s)\n", op_strings[op_type], reg_strings[rd], imm, reg_strings[rs1]);
                        break;
                    case OP_ECALL:
                        printf("ecall\n");
                        break;
                    default:
                        printf("%8s %s, %s, %d\n", op_strings[op_type], reg_strings[rd], reg_strings[rs1], imm);
                        break;
                }
                break;
            case INSTR_S:
                printf("%8s %s, %d(%s)\n", op_strings[op_type], reg_strings[rs2], imm, reg_strings[rs1]);
                break;
            case INSTR_SB:
                printf("%8s %s, %s, %d\n", op_strings[op_type], reg_strings[rs1], reg_strings[rs2], imm);
                break;
            case INSTR_U:
            case INSTR_UJ:
                printf("%8s %s, %d\n", op_strings[op_type], reg_strings[rd], imm);
                break;
            case INSTR_CR:
                printf("%8s %s\n", op_strings[op_type], reg_strings[rs1]);
                break;
            case INSTR_CI:
                switch (this->op_type) {
                    case OP_LI:
                    case OP_LUI:
                    case OP_LWSP:
                    case OP_LDSP:
                        printf("%8s %s, %d\n", op_strings[op_type], reg_strings[rd], imm);
                        break;
                    default:
                        printf("%8s %s, %s, %d\n", op_strings[op_type], reg_strings[rd], reg_strings[rd], imm);
                        break;
                }
                break;
            case INSTR_CSS:
                printf("%8s %s, %d\n", op_strings[op_type], reg_strings[rs2], imm);
                break;
            case INSTR_CL:
                printf("%8s %s, %d(%s)\n", op_strings[op_type], reg_strings[rd], imm, reg_strings[rs1]);
                break;
            case INSTR_CS:
                switch (this->op_type) {
                    case OP_SW:
                    case OP_SD:
                        printf("%8s %s, %d(%s)\n", op_strings[op_type], reg_strings[rs2], imm, reg_strings[rs1]);
                        break;
                    default:
                        printf("%8s %s, %s, %s\n", op_strings[op_type], reg_strings[rd], reg_strings[rd],
                               reg_strings[rs2]);
                }
                break;
            case INSTR_CB:
                switch (this->op_type) {
                    case OP_J:
                        printf("%8s %d\n", op_strings[op_type], imm);
                        break;
                    case OP_MV:
                        printf("%8s %s, %s\n", op_strings[op_type], reg_strings[rd], reg_strings[rs2]);
                        break;
                    case OP_ADD:
                        printf("%8s %s, %s, %s\n", op_strings[op_type], reg_strings[rd], reg_strings[rs1],
                               reg_strings[rs2]);
                        break;
                    case OP_SRLI:
                    case OP_SRAI:
                    case OP_ANDI:
                        printf("%8s %s, %s, %d\n", op_strings[op_type], reg_strings[rd], reg_strings[rd], imm);
                        break;
                    default:
                        printf("%8s %s, %d\n", op_strings[op_type], reg_strings[rs1], imm);
                }
                break;
            case INSTR_CIW:
                if (this->op_type == OP_ADDI) {
                    printf("%8s %s, %s, %d\n", op_strings[op_type], reg_strings[rd], reg_strings[rs1], imm);
                    break;
                }
            default: FATAL("Invalid op type %d\n", this->op_type);
        }
    } else {
        if (Decode_imm(binary_code, 0, 2, 0) == 0x3)
            printf("%8.8x\n", binary_code);
        else
            printf("%8.4x\n", (int16_t) binary_code);
    }
}
