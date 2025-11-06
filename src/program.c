#include <stdlib.h>
#include <sys/errno.h>

#include "program.h"
#include "winter.h"

/// Generates a random opcode except HALT.
opcode_t opcode_random(const uint32_t prefix_sums[5]) {
    const uint32_t v = (uint32_t)rand() % prefix_sums[4];

    for (uint32_t i = 0; i < 4; i++) {
        if (v < prefix_sums[i]) {
            return (opcode_t)(i + 1);
        }
    }

    return OP_BACK7;
}

/// Adapted version of theBytecode generator for Homework 2 (Lecture VT, Weidendorfer).
int program_generate(program_t* out, const uint32_t size, const uint8_t probabilities[5], const uint32_t seed) {
    opcode_t* buf = calloc(size, sizeof(opcode_t));
    if (buf == nullptr) {
        return ENOMEM;
    }

    // prefix sums for getRandomOpcode()
    uint32_t prefix_sums[5];

    uint32_t sum = 0;
    for (uint32_t i = 0; i < 5; i++) {
        sum += probabilities[i];
        prefix_sums[i] = sum;
    }

    srand(seed);
    out->initial_reg_a = rand() & 7;
    out->initial_reg_l = rand() & 7;

    for (uint32_t i = 0; i < size; i++) {
        const opcode_t opc = opcode_random(prefix_sums);
        if (opc == OP_BACK7) {
            if (i < 7) {
                // BACK7 not allowed in the first 7 instructions
                continue;
            }

            // remove SETL in the 6 instructions before
            for (uint32_t j = i - 6; j < i; j++) {
                if (buf[j] == OP_SETL) {
                    while (buf[j] == OP_SETL || buf[j] == OP_BACK7) {
                        buf[j] = opcode_random(prefix_sums);
                    }
                }
            }
        }

        buf[i] = opc;
    }

    buf[size - 1] = OP_HALT;
    out->code = buf;

    return 0;
}

describe(program) {
    static const uint8_t probabilities[5] = { 1, 1, 1, 1, 1 };

    it("generate HALT") {
        program_t program;
        program_generate(&program, 100, probabilities, 0);

        asserteq_int(program.code[99], OP_HALT);
    }

    it("generate only CLRA") {
        program_t program;
        program_generate(&program, 100, (uint8_t[]){ 1, 0, 0, 0, 0 }, 0);

        for (uint32_t i = 0; i < 99; i++) {
            asserteq_int(program.code[i], OP_CLRA);
        }
    }
}
