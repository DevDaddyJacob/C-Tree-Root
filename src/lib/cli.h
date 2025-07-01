#ifndef treeroot_lib_cli_h
#define treeroot_lib_cli_h

#include "common.h" /* Manditory before other imports */

typedef struct {
    /**
     * The file to run
     */
    char* file;

    /**
     * The maximum folder depth to read
     */
    Int16 maxDepth;
} CliConfig;


/**
 * \brief               Takes the command line arguments and parses them into
 *                      a CliConfig struct
 */
CliConfig* cli_parseArgs(int argc, const char* argv[]);


void cli_destroy(CliConfig* config);


#endif /* treeroot_lib_cli_h */