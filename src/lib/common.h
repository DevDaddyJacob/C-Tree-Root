#ifndef treeroot_lib_common_h
#define treeroot_lib_common_h

#include <stdio.h>
#include <string.h>

#include "../config.h"
#include "../treeroot.h"


/*
 * ============================================================================
 * Define some shorthands to make interacting with numbers a bit better
 * ============================================================================
 */

typedef signed char     Int8;
typedef unsigned char   UInt8;
typedef UInt8           Byte;

typedef signed int      Int16;
typedef unsigned int    UInt16;

typedef signed long     Int32;
typedef unsigned long   UInt32;

typedef signed long long    Int64;
typedef unsigned long long  UInt64;

/* ========================================================================= */


/*
 * ============================================================================
 * Just here to add some consistiency and readability for boolean-treated ints
 * ============================================================================
 */

typedef UInt8 Bool;
#define TRUE 1
#define FALSE 0

/* ========================================================================= */


/*
 * ============================================================================
 * Program possible exit codes
 * ============================================================================
 */

#define EXIT_GODD 0
#define EXIT_BAD_USAGE 1
#define EXIT_NO_MEM 2
#define EXIT_BUFF_OVERFLOW 3
#define EXIT_ACCESS_STATE_BEFOR_INIT 4


/* ========================================================================= */


/*
 * ============================================================================
 * Console writting tools (Lua inspired)
 * ============================================================================
 */

#if !defined(errPrint)
    #define errPrint(msg) \
        ( \
            fprintf( \
                stderr, \
                TREEROOT_FILE_NAME ": Error: %s\n", \
                (msg) \
            ), \
            fflush(stderr) \
        )
#endif

#if !defined(errPrintFatal)
    #define errPrintFatal(msg) \
        ( \
            fprintf( \
                stderr, \
                TREEROOT_FILE_NAME ": Fatal Error: %s\n", \
                (msg) \
            ), \
            fflush(stderr) \
        )
#endif


/* ========================================================================= */


/*
 * ============================================================================
 * Some simple misc utilities
 * ============================================================================
 */

#if !defined(STR_EQ)
    #define STR_EQ(strA, strB) \
        (strcmp(strA, strB) == 0)
#endif

#if !defined(CHAR_IS_ALPHA)
    #define CHAR_IS_ALPHA(c) \
        ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#endif

#if !defined(CHAR_IS_DIGIT)
    #define CHAR_IS_DIGIT(c) \
        (c >= '0' && c <= '9')
#endif

#if !defined(STR_ENDS_WITH_CHAR)
    #define STR_ENDS_WITH_CHAR(str, end) \
        ((str[strlen(str) - 1] == end))
#endif

#if !defined(STR_STARTS_WITH)
    #define STR_STARTS_WITH(strA, strB) \
        (memcmp(strA, strB, strlen(strB)) == 0)
#endif

/* ========================================================================= */


/*
 * ============================================================================
 * Array interactions
 * ============================================================================
 */

#define ARRAY_GROW_CAPACITY(capacity) \
    ((capacity) < ARRAY_MIN_THRESHOLD \
        ? ARRAY_MIN_THRESHOLD \
        : (capacity) * ARRAY_GROW_FACTOR)


#define ARRAY_RESIZE(type, pointer, oldCount, newCount) \
    (type*)reallocateArray( \
        pointer, \
        sizeof(type) * (oldCount), \
        sizeof(type) * (newCount) \
    )


#define ARRAY_FREE(type, pointer, oldCount) \
    troot_reallocateArray(pointer, sizeof(type) * (oldCount), 0)

/* ========================================================================= */


/*
 * ============================================================================
 * Safe functions implementations
 * ============================================================================
 */

/**
 * \brief           Safely copies a string into a destination buffer, at most
 *                  copying size - 1 characters and always ensuring a null
 *                  termination
 * \param[in]       destination: The destination buffer to write into
 * \param[in]       source: The source to copy into the destination
 * \param[in]       destSize: The size of the destination buffer
 * \return          Returns the total number of characters copied including
 *                  the null terminator
 */
UInt16 troot_safeStrCpy(
    char* destination,
    const char* source,
    UInt16 destSize
);

/* ========================================================================= */

#endif /* treeroot_lib_common_h */