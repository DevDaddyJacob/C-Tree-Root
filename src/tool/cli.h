#ifndef TULA_SRC_TOOL_CLI_H
#define TULA_SRC_TOOL_CLI_H

#include "../config.h" /* Manditory in all files */


typedef struct treeRoot_CliConfig {
    /**
     * \brief           The root path to start treeing from
     */
    char* rootPath;

    /**
     * \brief           The maximum folder depth to read
     */
    int16_t maxDepth;
} CliConfig;


/**
 * \brief               Takes the command line arguments and parses them into
 *                      a CliConfig struct
 */
CliConfig* cli_parseArgs(int32_t argc, const char* argv[]);


void cli_destroy(CliConfig* config);


#endif /* TULA_SRC_TOOL_CLI_H */