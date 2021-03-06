#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "emu.h"

State8080* state_alloc(size_t mem_size) {
    State8080 *state = malloc(sizeof(*state));
    state->memory = (uint8_t*) calloc(mem_size, sizeof(uint8_t));
    return state;
}

void state_free(State8080 *state) {
    if (state) {
        if (state->memory) {
            free(state->memory);
            state->memory = NULL;
        }
        free(state);
        state = NULL;
    }
}


#define MAX_STEPS 100000

/*
 * Returns the number of instructions
 * to advance
 */
size_t get_num_instrs(char *input) {
    if (strlen(input) == 1) {
        return 1;
    }
    // If the string starts with an 
    // alphanumeric character or only 
    // contains alphanumeric characters,
    // 0 is returned.
    size_t steps = (size_t) atoi(input); 
    return steps < MAX_STEPS ? steps : MAX_STEPS; 
}


int load_and_run(char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("Error: couldn't open %s\n", filename);
        exit(1);
    }

    // declare ConditionCodes struct
    ConditionCodes cc;
    cc.z = 0;
    cc.s = 0;
    cc.p = 0;
    cc.cy = 0;
    cc.ac = 0;

    // declare State8080 struct
    State8080 state;
    state.a = 0;
    state.b = 0;
    state.c = 0;
    state.d = 0;
    state.e = 0;
    state.h = 0;
    state.l = 0;

    state.sp = 0;
    state.pc = 0;
    state.int_enable = 0;
    // 16-bit address has a maximum of
    // 2^15 addressable 8-bit chunks
    size_t max_size = 1 << 15;
    state.memory = (uint8_t*) malloc(max_size * sizeof(*state.memory));

    state.cc = cc;

    // get the file size and read it into a memory buffer
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    fread(state.memory, fsize, 1, f);
    fclose(f);

    size_t instr_count = 0;
    char user_in [20];

    size_t instrs_to_advance = 0;
    while (state.pc < fsize) {
        printf("Emulator state:\n");
        print_state(&state);
        printf("Instructions executed: %zu\n", instr_count);

        if (instrs_to_advance == 0) {
            printf(
                "Press enter to advance one instruction, or " 
                "enter number of instructions to advance "
                "and then press enter: "); 
            fgets(user_in, 20, stdin);
            instrs_to_advance = get_num_instrs(user_in);
            if (instrs_to_advance == 0) {
                continue;
            }
        }
        printf("\n\n");
        emulate_op(&state);
        instr_count++;
        instrs_to_advance--;
    }
    printf("LOOP EXITED.\n");
    print_state(&state);
    printf("fsize: 0x%x\n", fsize);

    free(state.memory);

    return 0;
}
