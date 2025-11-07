#pragma once

#include <stdint.h>

#include "program.h"

typedef struct {
    /// The instruction pointer should always point to a valid position inside
    /// the code array.
    uint32_t ip;

    /// Register A: Accumulator
    int32_t reg_a;

    /// Register L: Loop counter
    int32_t reg_l;
} vm_state_t;

/// Default implementation of the VM.
vm_state_t vm_exec_default(program_t program);

/// Threaded implementation of the VM.
vm_state_t vm_exec_threaded(program_t program);
