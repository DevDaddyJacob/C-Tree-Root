#include "../lib/common.h" /* Manditory before other imports */

#include <stdlib.h>
#include "state.h"
#include "app.h"
#include "../lib/cli.h"

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

void state_setup(Int16 argc, const char** argv) {
    if (GLOBAL_STATE != NULL) return;

    /* Initialize the state in heap */
    GLOBAL_STATE = (GlobalState*)malloc(sizeof(GlobalState));
    if (GLOBAL_STATE == NULL) {
        app_exitFatal(EXIT_NO_MEM, "Failed to allocate memory");
        return; /* Unreachable */
    }


    /* Initialize all fields */
    GLOBAL_STATE->argc = argc;
    GLOBAL_STATE->argv = argv;
    GLOBAL_STATE->cli = NULL;

    
    /* Setup the cli config */
    GLOBAL_STATE->cli = cli_parseArgs(
        GLOBAL_STATE->argc, 
        GLOBAL_STATE->argv
    );
}


void state_teardown() {
    if (GLOBAL_STATE == NULL) return;


    /* Destroy the cli config */
    if (GLOBAL_STATE->cli != NULL) {
        cli_destroy(GLOBAL_STATE->cli);
        GLOBAL_STATE->cli = NULL;
    }


    free(GLOBAL_STATE);
    GLOBAL_STATE = NULL;
}


GlobalState* state_getGlobal() {
    if (GLOBAL_STATE == NULL) {
        app_exitFatal(
            EXIT_ACCESS_STATE_BEFOR_INIT,
            "Attempted to access global state before initialization"
        );
        return NULL; /* Unreachable */
    }

    return GLOBAL_STATE;
}