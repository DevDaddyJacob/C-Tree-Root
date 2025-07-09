#ifndef TREEROOT_SRC_CONFIG_H
#define TREEROOT_SRC_CONFIG_H

/*
 * ============================================================================
 * Globally used imports and definitions
 * ============================================================================
 */

#include <stdint.h>

typedef uint8_t byte_t;

/* ========================================================================= */


/*
 * ============================================================================
 * Development debugging config
 * ============================================================================
 */

/**
 * Used manually if you want to have some extra debugging output, mainly for
 * development work
 */
#define DEBUGGING_ENABLED

/* ========================================================================= */


/*
 * ============================================================================
 * Configure the operating system macros as well as platform dependent macros
 * ============================================================================
 */

#ifndef _OS_DEF
    #if defined(_WIN32) || defined(_WIN64)
        #define OS_WINDOWS
        #define _OS_DEF

    #elif defined(__APPLE__) && defined(__MACH__)
        #define OS_MAC
        #define OS_POSIX_COMPLIANT
        #define _OS_DEF

    #elif defined(__linux__)
        #define OS_LINUX
        #define OS_POSIX_COMPLIANT
        #define _OS_DEF

    #elif defined(__unix__)
        #define OS_UNIX
        #define OS_POSIX_COMPLIANT
        #define _OS_DEF

    #else
        #error "Unknown or unsupported platform"
    #endif
#endif /* _OS_DEF */


#ifndef PATH_SEPARATOR
    #if defined(OS_WINDOWS)
        #define PATH_SEPARATOR '\\'
    #elif defined(OS_POSIX_COMPLIANT)
        #define PATH_SEPARATOR '/'
    #else
        #error Unsupported platform, \
            no compliant implementation of macro 'PATH_SEPARATOR'.
    #endif
#endif /* PATH_SEPARATOR */


#ifndef MAX_PATH_LENGTH
    #if defined(OS_WINDOWS)
        #define MAX_PATH_LENGTH 260
    #elif defined(OS_MAC)
        #define MAX_PATH_LENGTH 1024
    #elif defined(OS_LINUX)
        #define MAX_PATH_LENGTH 4096
    #elif defined(OS_POSIX_COMPLIANT)
        #define MAX_PATH_LENGTH 4096
    #else
        #error Unsupported platform, \
            no compliant implementation of macro 'MAX_PATH_LENGTH'.
    #endif
#endif /* MAX_PATH_LENGTH */



/* ========================================================================= */


/*
 * ============================================================================
 * Configure the parameters for arrays
 * ============================================================================
 */


/**
 * The number by which array sizes is multiplied by when increasing their size
 * For example, if an array has 16 elements, and we are wanting to increase the 
 * size, we will multiply 16 by the value of ARRAY_GROW_FACTOR
 * 
 * Usage
 */
#define ARRAY_GROW_FACTOR 2


/**
 * The minimum array size. If we have an minimum of 8 and an array of size 3 
 * and we attempt to resize it, a check will occur to see if the size is lower 
 * than the minimum and if so, rather than multiplying by the grow factor,
 * instead we set the size to the minimum.
 */
#define ARRAY_MIN_THRESHOLD 8

/* ========================================================================= */


/*
 * ============================================================================
 * Configure the parameters for hash tables
 * ============================================================================
 */


/**
 * TBD
 */
#define HASH_TABLE_GROW_FACTOR 2


/**
 * TBD
 */
#define HASH_TABLE_MIN_THRESHOLD 16


/**
 * TBD
 */
#define HASH_TABLE_MAX_LOAD 0.75

/* ========================================================================= */


/*
 * ============================================================================
 * Configure the parameters for buffers
 * ============================================================================
 */


/**
 * The maximum size a buffer can be
 */
#define MAX_BUFFER_SIZE 32


/* ========================================================================= */


#endif /* TREEROOT_SRC_CONFIG_H */