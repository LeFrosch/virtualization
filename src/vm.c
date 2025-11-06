#include "vm.h"
#include "winter.h"

vm_state_t vm_exec_default(const program_t program) {
    vm_state_t state = (vm_state_t){
        .ip = 0,
        .reg_a = program.initial_reg_a,
        .reg_l = program.initial_reg_l,
    };

    while (true) {
        const opcode_t op = program.code[state.ip++];

        switch (op) {
            case OP_HALT:
                return state;

            case OP_CLRA:
                state.reg_a = 0;
                break;

            case OP_INC3A:
                state.reg_a += 3;
                break;

            case OP_DECA:
                state.reg_a -= 1;
                break;

            case OP_SETL:
                state.reg_l = state.reg_a;
                break;

            case OP_BACK7:
                if (--state.reg_l > 0) {
                    state.ip -= 8;
                }
                break;
        }
    }
}

vm_state_t vm_exec_threaded(program_t program) {}

describe(vm) {
    it("opcode HALT") {
        const vm_state_t state = vm_exec_default((program_t){
          .code = (opcode_t[]){ OP_HALT },
        });

        asserteq_int(state.ip, 1);
    }

    it("opcode INC3A") {
        const vm_state_t state = vm_exec_default((program_t){
          .code = (opcode_t[]){ OP_INC3A, OP_HALT },
          .initial_reg_a = 0,
          .initial_reg_l = 0,
        });

        asserteq_int(state.ip, 2);
        asserteq_int(state.reg_a, 3);
    }

    it("opcode DECA") {
        const vm_state_t state = vm_exec_default((program_t){
          .code = (opcode_t[]){ OP_INC3A, OP_DECA, OP_HALT },
        });

        asserteq_int(state.ip, 3);
        asserteq_int(state.reg_a, 2);
    }

    it("opcode CLRA") {
        const vm_state_t state = vm_exec_default((program_t){
          .code = (opcode_t[]){ OP_INC3A, OP_CLRA, OP_HALT },
        });

        asserteq_int(state.ip, 3);
        asserteq_int(state.reg_a, 0);
    }

    it("opcode SETL") {
        const vm_state_t state = vm_exec_default((program_t){
          .code = (opcode_t[]){ OP_INC3A, OP_SETL, OP_HALT },
        });

        asserteq_int(state.ip, 3);
        asserteq_int(state.reg_l, 3);
    }

    it("opcode BACK7") {
        const vm_state_t state = vm_exec_default((program_t){
          .code = (opcode_t[]){ OP_INC3A, OP_INC3A, OP_DECA, OP_DECA, OP_DECA, OP_DECA, OP_DECA, OP_BACK7, OP_HALT },
          .initial_reg_l = 3,
        });

        asserteq_int(state.reg_a, 3);
        asserteq_int(state.reg_l, 0);
    }
}
