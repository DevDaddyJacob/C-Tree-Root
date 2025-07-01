#include <stdlib.h>
#include <stdio.h>

#include "../lib/common.h" /* Manditory before other imports */

#include "app.h"
#include "state.h"
#include "fnode.h"



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

static void teardown(Int16 exitCode, const char* exitMessage, Bool isFatal);


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

/* int magicNumber = 420; */


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static void teardown(Int16 exitCode, const char* exitMessage, Bool isFatal) {
    state_teardown();

    if (exitMessage != NULL) {
        if (isFatal) errPrintFatal(exitMessage);
        else errPrint(exitMessage);
    }

    exit(exitCode);
}


void app_setup(Int16 argc, const char* argv[]) {
    state_setup(argc, argv);
}


void app_exit(Int16 exitCode) {
    teardown(exitCode, NULL, FALSE);
}


void app_exitError(Int16 exitCode, const char* errorMessage) {
    teardown(exitCode, errorMessage, FALSE);
}


void app_exitFatal(Int16 exitCode, const char* errorMessage) {
    teardown(exitCode, errorMessage, TRUE);
}
