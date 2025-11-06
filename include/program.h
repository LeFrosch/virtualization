#pragma once

#include <stdint.h>

typedef enum : uint8_t {
    /// Stop execution.
    OP_HALT = 0,

    /// Set the content of register A to 0.
    OP_CLRA,

    /// Increment register A by 3.
    OP_INC3A,

    /// Decrement register A by 1.
    OP_DECA,

    /// Copy value of register A to L.
    OP_SETL,

    /// Decrement L; if the value of L is positive, jump back by 7 instructions
    /// i.e. loop body is 6 one-byte instructions and the BACK7 itself.
    /// Otherwise, fall through to the next instruction.
    OP_BACK7,
} opcode_t;

typedef struct {
    const opcode_t* code;
    int32_t initial_reg_a;
    int32_t initial_reg_l;
} program_t;

// Randomly generates an opcode sequence of length <size>, terminated by HALT,
// using relative probabilities. Deterministic for a given seed.
int program_generate(program_t* out, uint32_t size, const uint8_t probabilities[5], uint32_t seed);

program_t program_load(const char* filename);
