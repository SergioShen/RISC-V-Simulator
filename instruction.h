//
// Name: instruction
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/26/18
//

#ifndef RISC_V_SIMULATOR_INSTRUCTION_H
#define RISC_V_SIMULATOR_INSTRUCTION_H

#include "utility.h"

// Register macro definitions
#define REG_zero 0
#define REG_ra 1
#define REG_sp 2
#define REG_gp 3
#define REG_tp 4
#define REG_t0 5
#define REG_t1 6
#define REG_t2 7
#define REG_s0 8
#define REG_s1 9
#define REG_a0 10
#define REG_a1 11
#define REG_a2 12
#define REG_a3 13
#define REG_a4 14
#define REG_a5 15
#define REG_a6 16
#define REG_a7 17
#define REG_s2 18
#define REG_s3 19
#define REG_s4 20
#define REG_s5 21
#define REG_s6 22
#define REG_s7 23
#define REG_s8 24
#define REG_s9 25
#define REG_s10 26
#define REG_s11 27
#define REG_t3 28
#define REG_t4 29
#define REG_t5 30
#define REG_t6 31

// OP type macro definitions
#define OP_ADD 0
#define OP_MUL 1
#define OP_SUB 2
#define OP_SLL 3
#define OP_MULH 4
#define OP_SLT 5
#define OP_XOR 6
#define OP_DIV 7
#define OP_SRL 8
#define OP_SRA 9
#define OP_OR 10
#define OP_REM 11
#define OP_AND 12
#define OP_LB 13
#define OP_LH 14
#define OP_LW 15
#define OP_LD 16
#define OP_ADDI 17
#define OP_SLLI 18
#define OP_SLTI 19
#define OP_XORI 20
#define OP_SRLI 21
#define OP_SRAI 22
#define OP_ORI 23
#define OP_ANDI 24
#define OP_ADDIW 25
#define OP_JALR 26
#define OP_ECALL 27
#define OP_SB 28
#define OP_SH 29
#define OP_SW 30
#define OP_SD 31
#define OP_BEQ 32
#define OP_BNE 33
#define OP_BLT 34
#define OP_BGE 35
#define OP_AUIPC 36
#define OP_LUI 37
#define OP_JAL 38
#define OP_LI 39
#define OP_SUBW 40
#define OP_ADDW 41
#define OP_J 42
#define OP_BEQZ 43
#define OP_BNEZ 44
#define OP_LWSP 45
#define OP_LDSP 46
#define OP_SWSP 47
#define OP_SDSP 48
#define OP_MV 49
#define OP_BLTU 50
#define OP_BGEU 51
#define OP_JR 52
#define OP_SLLIW 53
#define OP_SRLIW 54
#define OP_SRAIW 55
#define OP_SLLW 56
#define OP_SRLW 57
#define OP_SRAW 58
#define OP_LBU 59
#define OP_LHU 60
#define OP_MULW 61

// Instr type macro definitions
#define INSTR_R 0
#define INSTR_I 1
#define INSTR_S 2
#define INSTR_SB 3
#define INSTR_U 4
#define INSTR_UJ 5
#define INSTR_CR 6
#define INSTR_CI 7
#define INSTR_CSS 8
#define INSTR_CIW 9
#define INSTR_CL 10
#define INSTR_CS 11
#define INSTR_CB 12
#define INSTR_CJ 13

// Macros for decode
#define Decode_opcode(instr_code) ((int8_t) (instr_code & 0b1111111))
#define Decode_rd(instr_code) ((int8_t) ((instr_code >> 7) & 0b11111))
#define Decode_rs1(instr_code) ((int8_t) ((instr_code >> 15) & 0b11111))
#define Decode_rs2(instr_code) ((int8_t) ((instr_code >> 20) & 0b11111))
#define Decode_funct3(instr_code) ((int8_t) ((instr_code >> 12) & 0b111))
#define Decode_funct7(instr_code) ((int8_t) ((instr_code >> 25) & 0b1111111))
#define Decode_imm(instr_code, index_in_instr, num_of_bits, index_in_imm) \
        (((instr_code >> index_in_instr) & ((1 << num_of_bits) - 1)) << index_in_imm)

#define Decode_c_opcode(instr_code) ((int8_t) (instr_code & 0b11))
#define Decode_c_funct3(instr_code) ((int8_t) ((instr_code >> 13) & 0b111))
#define Decode_c_funct4(instr_code) ((int8_t) ((instr_code >> 12) & 0b1111))
#define Decode_c_rd(instr_code) ((int8_t) ((instr_code >> 7) & 0b11111))
#define Decode_c_rs1(instr_code) ((int8_t) ((instr_code >> 7) & 0b11111))
#define Decode_c_rs2(instr_code) ((int8_t) ((instr_code >> 2) & 0b11111))
#define Decode_cc_rd(instr_code) ((int8_t) (((instr_code >> 2) & 0b111) + 8))
#define Decode_cc_rs1(instr_code) ((int8_t) (((instr_code >> 7) & 0b111) + 8))

class Instruction {
private:
    // Private decode functions for different instr formats
    void DecodeRInstruction();

    void DecodeIInstruction();

    void DecodeSInstruction();

    void DecodeSBInstruction();

    void DecodeUInstruction();

    void DecodeUJInstruction();

    // Extend sign of imm
    void ImmSignExtend(int num_of_bits);

public:
    int32_t binary_code;
    int32_t imm;
    int8_t funct3, funct7; // funct3 may also used by funct4
    int8_t opcode;
    int8_t rs1, rs2, rd;
    int8_t op_type; /* This is different from opcode,
                     * opcode is the value of 7 bits in instr code,
                     * but this is one of the OP_*** values defined above
                     */
    int8_t instr_type;
    int8_t decoded;
    bool write_reg;
    int64_t instr_pc;
    int64_t write_back_value;

    // Decode the instruction
    bool Decode();

    // Print the semantic meaning of the instruction
    void Print();
};

#endif //RISC_V_SIMULATOR_INSTRUCTION_H
