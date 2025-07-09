#include <stdlib.h>

#include "../config.h" /* Manditory in all files */
#include "../treeroot.h"

#include "gstate.h"
#include "cli.h"

/*
 * ==================================================
 * Macros
 * ==================================================
 */

/* #define XYZ "ABC" */

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

static GlobalState* GLOBAL_STATE = NULL;


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

void setupGlobalState(int32_t argc, const char** argv) {
    if (GLOBAL_STATE != NULL) return;

    /* Initialize the state in heap */
    GLOBAL_STATE = (GlobalState*)malloc(sizeof(GlobalState));
    if (GLOBAL_STATE == NULL) {
        exitErr_noMem();
        UNREACHABLE_RETRUN();
    }


    /* Initialize all fields */
    GLOBAL_STATE->argc = argc;
    GLOBAL_STATE->argv = argv;
    GLOBAL_STATE->cli = NULL;
    GLOBAL_STATE->nodeSizes = NULL;

    
    /* Setup the cli config */
    GLOBAL_STATE->cli = cli_parseArgs(
        GLOBAL_STATE->argc, 
        GLOBAL_STATE->argv
    );


    /* Initialize the node sizes table */
    GLOBAL_STATE->nodeSizes = hashTable_new(TBL_ITEM_UINT64);
}


void teardownGlobalState() {
    if (GLOBAL_STATE == NULL) return;


    /* Destroy the cli config */
    if (GLOBAL_STATE->cli != NULL) {
        cli_destroy(GLOBAL_STATE->cli);
        GLOBAL_STATE->cli = NULL;
    }


    /* Destroy the hash table for node sizes */
    if (GLOBAL_STATE->nodeSizes != NULL) {
        hashTable_destroy(GLOBAL_STATE->nodeSizes);
        GLOBAL_STATE->nodeSizes = NULL;
    }


    free(GLOBAL_STATE);
    GLOBAL_STATE = NULL;
}


GlobalState* getGlobalState() {
    if (GLOBAL_STATE == NULL) {
        exitErr_earlyStateAccess();
        UNREACHABLE_RETRUN(NULL);
    }

    return GLOBAL_STATE;
}