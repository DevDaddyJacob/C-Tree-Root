#ifndef treeroot_core_state_h
#define treeroot_core_state_h

#include "../lib/common.h" /* Manditory before other imports */

#include "../lib/cli.h"

typedef struct state_GlobalState {
    Int16 argc;
    const char** argv;
    CliConfig* cli;
} GlobalState;


void state_setup(Int16 argc, const char** argv);


void state_teardown();


GlobalState* state_getGlobal();



#endif /* treeroot_core_state_h */