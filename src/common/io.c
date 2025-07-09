#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../config.h" /* Manditory in all files */
#include "../treeroot.h"

#include "io.h"


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

int32_t errPrint(const char* format, ...) {
    /* Null check the format */
    if (format == NULL) return PRINT_FAILED;


    /* Get the variable arguments */
    va_list vargs;
    va_start(vargs, format);


    /* Send to the v function */
    int32_t charsPrinted = vErrPrint(format, vargs);


    /* Release the vargs and return the status */
    va_end(vargs);

    if (charsPrinted > 2048) return PRINT_PARTIAL_SUCCESS;

    if (charsPrinted <= 0) return PRINT_FAILED;

    return charsPrinted;
}


int32_t vErrPrint(const char* format, va_list vargs) {
    /* Null check the format */
    if (format == NULL) return PRINT_FAILED;


    /* Print the message into the program's error format */
    char outFormatBuff[2048];
    snprintf(
        outFormatBuff,
        2048,
        TREEROOT_PROGRAM_NAME ": %s\n",
        format
    );


    /* Format the output */
    char outputStr[2048];
    int32_t charsPrinted = vsnprintf(
        outputStr,
        2048,
        outFormatBuff,
        vargs
    );


    /* Print the output to stderr */
    fprintf(stderr, outputStr);


    /* Return the status */
    if (charsPrinted > 2048) return PRINT_PARTIAL_SUCCESS;

    if (charsPrinted <= 0) return PRINT_FAILED;

    return charsPrinted;
}