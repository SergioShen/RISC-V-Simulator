//
// Name: machine
// Project: RISC_V_Simulator
// Author: Shen Sijie
// Date: 4/6/18
//

#ifndef RISC_V_SIMULATOR_MACHINE_H
#define RISC_V_SIMULATOR_MACHINE_H

#include "utility.h"
#include "mem.h"

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

// Instr type macro definitions
#define INSTR_R 0
#define INSTR_I 1
#define INSTR_S 2
#define INSTR_SB 3
#define INSTR_U 4
#define INSTR_UJ 5

extern char op_strings[39][8];

// Macros for decode
#define Decode_opcode(instr_code) ((int8_t) (instr_code & 0b1111111))
#define Decode_rd(instr_code) ((int8_t) ((instr_code >> 7) & 0b11111))
#define Decode_rs1(instr_code) ((int8_t) ((instr_code >> 15) & 0b11111))
#define Decode_rs2(instr_code) ((int8_t) ((instr_code >> 20) & 0b11111))
#define Decode_funct3(instr_code) ((int8_t) ((instr_code >> 12) & 0b111))
#define Decode_funct7(instr_code) ((int8_t) ((instr_code >> 25) & 0b1111111))
#define Decode_imm(instr_code, index_in_instr, num_of_bits, index_in_imm) \
        (((instr_code >> index_in_instr) & ((1 << num_of_bits) - 1)) << index_in_imm)

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
    int8_t funct3, funct7;
    int8_t opcode;
    int8_t rs1, rs2, rd;
    int8_t op_type; /* This is different from opcode,
                     * opcode is the value of 7 bits in instr code,
                     * but this is one of the OP_*** values defined above
                     */
    int8_t instr_type;

    // Decode the instruction
    void Decode();

    // Print the semantic meaning of the instruction
    void Print();
};

class Machine {
private:
    int64_t registers[32];
    int64_t reg_pc;
    int64_t reg_prev_pc;
    int64_t reg_addr;
    Memory *main_memory;

    Instruction *FetchInstruction();

    void Execute(Instruction *instruction);

    void ReadMemory(Instruction *instruction);

    void WriteBack(Instruction *instruction);

    void HandleSystemCall();


public:
    Machine();

    ~Machine();

    bool LoadExecutableFile(const char *file_name);

    void PrintRegisters();

    void Run();
};

#endif //RISC_V_SIMULATOR_MACHINE_H
