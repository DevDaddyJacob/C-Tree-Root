#ifndef TREEROOT_SRC_TREEROOT_H
#define TREEROOT_SRC_TREEROOT_H

#include <stdint.h>

#include "./common/io.h"


/*
 * ============================================================================
 * Program definitions
 * ============================================================================
 */

#define TREEROOT_PROGRAM_NAME "treeroot"

#define TREEROOT_VERSION_MAJOR	    "1"
#define TREEROOT_VERSION_MINOR	    "0"
#define TREEROOT_VERSION_RELEASE	"0"

#define TREEROOT_VERSION \
    "TreeRoot v" TREEROOT_VERSION_MAJOR "." TREEROOT_VERSION_MINOR

#define TREEROOT_RELEASE \
    TREEROOT_VERSION "." TREEROOT_VERSION_RELEASE

/* ========================================================================= */


/*
 * ============================================================================
 * Program possible exit codes
 * ============================================================================
 */

#define TREEROOT_EXIT_GOOD                      0
#define TREEROOT_EXIT_UNDEF_INTERNAL_ERR        1
#define TREEROOT_EXIT_BAD_USAGE                 2
#define TREEROOT_EXIT_NO_MEM                    3
#define TREEROOT_EXIT_ACCESS_STATE_BEFOR_INIT   4
   
    
#define exitErr_internal(msg) \
    (treeroot_exitError( \
        TREEROOT_EXIT_UNDEF_INTERNAL_ERR, \
        "Undefined Internal Error: " msg \
    ))
    
#define exitErr_internalF(format, ...) \
    (treeroot_exitError( \
        TREEROOT_EXIT_UNDEF_INTERNAL_ERR, \
        "Undefined Internal Error: " format, \
        __VA_ARGS__ \
    ))
   
    
#define exitErr_badUsage(msg) \
    (treeroot_exitError(TREEROOT_EXIT_BAD_USAGE, "Bad Usage: " msg))
    
#define exitErr_badUsageF(format, ...) \
    (treeroot_exitError( \
        TREEROOT_EXIT_BAD_USAGE, \
        "Bad Usage: " format, \
        __VA_ARGS__ \
    ))
    

#define exitErr_noMem() \
    (treeroot_exitError( \
        TREEROOT_EXIT_NO_MEM, \
        "Memory Allocation Error: Failed to allocate memory" \
    ))
    

#define exitErr_earlyStateAccess() \
    (treeroot_exitError( \
        TREEROOT_EXIT_ACCESS_STATE_BEFOR_INIT, \
        "Early State Access: Attempted to access a state instance before " \
        "it's initialization" \
    ))

/* ========================================================================= */


/*
 * ============================================================================
 * Common / Utility Macros
 * ============================================================================
 */

#define UNREACHABLE(value) value
#define UNREACHABLE_RETRUN(value) UNREACHABLE(return value)
#define UNREACHABLE_DEFAULT(value) UNREACHABLE(default: value)

/* ========================================================================= */


void treeroot_exit(int32_t exitCode);


void treeroot_exitError(int32_t exitCode, const char* format, ...);


#endif /* TREEROOT_SRC_TREEROOT_H */