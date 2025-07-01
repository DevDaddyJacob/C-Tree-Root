#include <stdlib.h>

#include "common.h"

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

/**
 * TBD
 */
/* static void example(); */


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


UInt16 troot_safeStrCpy(
    char* destination,
    const char* source,
    UInt16 destSize
) {
    UInt16 i = 0;

    /* If our destination size is 0, don't copy */
    if (destSize == 0) return 0;


    /* Copy up to destSize - 1 characters */
    while (i < destSize - 1 && source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }


    /* Ensure the last char is a null terminator */
    destination[i] = '\0';

    return i + 1;
}
