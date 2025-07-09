#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h" /* Manditory in all files */
#include "treeroot.h"

#include "./app/nodes.h"
#include "./tool/gstate.h"

/*
 * ==================================================
 * Macros
 * ==================================================
 */


/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

/**
 * TBD
 */
static void setup(int32_t argc, const char* argv[]);


/**
 * TBD
 */
static void teardown();


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static void setup(int32_t argc, const char* argv[]) {
    setupGlobalState(argc, argv);
}


static void teardown() {
    teardownGlobalState();
}

 
void treeroot_exit(int32_t exitCode) {
    teardown();
    exit(exitCode);
}


void treeroot_exitError(int32_t exitCode, const char* format, ...) {
    if (format != NULL) {
        /* Get the vargs, and print the message */
        va_list vargs;
        va_start(vargs, format);
        vErrPrint(format, vargs);
        va_end(vargs);
    }

    teardown();
    exit(exitCode);
}


int main(int argc, const char* argv[]) {
    /* Run setup operations */
    setup(argc, argv);
    GlobalState* gState = getGlobalState();

    /* Get the node from the cli options */
    FNode* fnode = fnode_getNodeFromPath(
        gState->cli->rootPath,
        gState->cli->maxDepth
    );
    fnode_print(fnode, 0);


    /* Exit the program gracefully */
    treeroot_exit(TREEROOT_EXIT_GOOD);
    UNREACHABLE_RETRUN(-1);
}