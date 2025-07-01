#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "treeroot.h"
#include "lib/common.h"
#include "lib/platform.h"

#include "core/app.h"
#include "core/state.h"
#include "core/fnode.h"

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

int main(int argc, const char* argv[]) {
    GlobalState* gState = NULL;
    FNode *fnode = NULL;

    /* Run setup operations */
    app_setup(argc, argv);
    gState = state_getGlobal();
    

    /* Get the node from the cli options */
    fnode = fnode_getNodeFromPath(gState->cli->file, gState->cli->maxDepth);
    fnode_print(fnode, 0);

    app_exit(EXIT_GODD);
    return -1; /* Unreachable */
}