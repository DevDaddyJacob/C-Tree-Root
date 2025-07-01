#ifndef treeroot_config_h
#define treeroot_config_h

#include <limits.h>


/*
 * ============================================================================
 * Development debugging config
 * ============================================================================
 */

/**
 * Used manually if you want to have some extra debugging output, mainly for
 * development work
 */
#define USE_DEBUGGING

/* ========================================================================= */



/*
 * ============================================================================
 * Configure the C standard and OS
 * ============================================================================
 */

#if defined(OS_WINDOWS)
    #undef OS_POSIX_COMPLIANT
    #define _OS_DEF
#endif


#if defined(OS_MAC)
    #define OS_POSIX_COMPLIANT
    #define _OS_DEF
#endif


#if defined(OS_LINUX)
    #define OS_POSIX_COMPLIANT
    #define _OS_DEF
#endif


#if defined(OS_UNIX)
    #define OS_POSIX_COMPLIANT
    #define _OS_DEF
#endif


#if !defined(_OS_DEF)
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
#endif

/* ========================================================================= */



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


#endif /* treeroot_config_h */