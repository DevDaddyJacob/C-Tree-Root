#ifndef TULA_SRC_GSTATE_H
#define TULA_SRC_GSTATE_H

#include "../config.h" /* Manditory in all files */

#include "cli.h"
#include "../common/table.h"


typedef struct treeRoot_GlobalState {
    int32_t argc;
    const char** argv;
    CliConfig* cli;
    HashTable* nodeSizes;
} GlobalState;


void setupGlobalState(int32_t argc, const char** argv);


void teardownGlobalState();


GlobalState* getGlobalState();



#endif /* TULA_SRC_GSTATE_H */