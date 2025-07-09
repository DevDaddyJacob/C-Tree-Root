#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h" /* Manditory in all files */
#include "../treeroot.h"

#include "cli.h"
#include "../common/strings.h"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

typedef struct treeRoot_CliParams {
    int pointer;
    int argc;
    const char** argv;
} CliParams;


typedef enum treeRoot_OptionType {
    OPTION_UNKNOWN,         /** Not an option */
    OPTION_INVALID,         /** Invalid option */
    OPTION_END_OF_OPTIONS,  /** -- or --end-of-options */
    OPTION_HELP,            /** -h or --help */
    OPTION_VERSION,         /** -v or --version */
    OPTION_MAX_DEPTH        /** -d or --max-depth */
} OptionType;



static void printHelpMenu();

static uint8_t hasNext(CliParams* params);

static const char* peekArgument(CliParams* params);

static const char* consumeArgument(CliParams* params);

static OptionType parseOptionType(const char* arg);

static uint8_t consumeNextOption(CliParams* params, CliConfig* config);


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

static void printHelpMenu() {
#define TOTAL_OPTIONS 4
    int i = 0;
    const char* options[TOTAL_OPTIONS][2] = {
        { 
            "-h, --help",
            "prints this menu",
        },
        { 
            "-d, --max-depth=[depth]",
            "prints this menu",
        },
        { 
            "-v, --version",
            "prints the version of the program",
        },
        { 
            "--, --end-of-options",
            "indicates the end of the options",
        }
    };

    fprintf(
        stdout, 
        "Usage: %s [OPTIONS]... [FILE]\n\n" \
        "Options:\n", 
        TREEROOT_PROGRAM_NAME
    );

    for (i = 0; i < TOTAL_OPTIONS; i++) {
        fprintf(
            stdout,
            "\t%s\n\t\t%s\n\n",
            options[i][0],
            options[i][1]
        );
    }

#undef TOTAL_OPTIONS
    treeroot_exit(TREEROOT_EXIT_GOOD);
}


static uint8_t hasNext(CliParams* params) {
    if (params->pointer >= params->argc) return 0;
    return 1;
}


static const char* peekArgument(CliParams* params) {
    if (!hasNext(params)) return NULL;
    return params->argv[params->pointer];
}


static const char* consumeArgument(CliParams* params) {
    if (!hasNext(params)) return NULL;
    return params->argv[params->pointer++];
}


static OptionType parseOptionType(const char* arg) {
    if (arg[0] != '-' || strlen(arg) < 2) return OPTION_UNKNOWN;

    switch (arg[1]) {
        case '-': {
            if (strlen(arg) == 2) return OPTION_END_OF_OPTIONS;

            switch (arg[2]) {
                default: {
                    if (str_partialEquals(arg, "end-of-options", 14, 2)) {
                        return OPTION_END_OF_OPTIONS;

                    } else if (str_partialEquals(arg, "help", 4, 2)) {
                        return OPTION_HELP;

                    } else if (str_partialEquals(arg, "version", 7, 2)) {
                        return OPTION_VERSION;

                    } else if (str_partialEquals(arg, "max-depth", 9, 2)) {
                        return OPTION_VERSION;

                    } else {
                        return OPTION_INVALID;
                    }
                }
            }
        }

        case 'd': return OPTION_MAX_DEPTH;
        case 'h': return OPTION_HELP;
        case 'v': return OPTION_VERSION;
        default: return OPTION_INVALID;
    }
}


static uint8_t consumeNextOption(CliParams* params, CliConfig* config) {
    OptionType optionType;

    if (!hasNext(params)) return 0;

    switch ((optionType = parseOptionType(peekArgument(params)))) {
        case OPTION_INVALID:
        case OPTION_HELP:
        case OPTION_VERSION: {
            const char* arg = peekArgument(params);
            free(params);
            free(config);
            
            switch (optionType) {
                case OPTION_INVALID: {
                    exitErr_badUsageF("'%s' is not a recognized option.", arg);
                    UNREACHABLE(break);
                }

                case OPTION_HELP: {
                    printHelpMenu();
                    break;
                }

                case OPTION_VERSION: {
                    printf(TREEROOT_RELEASE "\n");
                    treeroot_exit(TREEROOT_EXIT_GOOD);
                    UNREACHABLE(break);
                }

                UNREACHABLE_DEFAULT(break);
            }
            
            UNREACHABLE_RETRUN(0);
        }

        case OPTION_UNKNOWN: {
            return 0;
        }

        case OPTION_END_OF_OPTIONS: {
            consumeArgument(params);
            return 0;
        }

        case OPTION_MAX_DEPTH: {
            const char* arg = consumeArgument(params);
            const char* separator = strrchr(arg, '=');

            if (!separator) {
                exitErr_badUsage("Missing depth value.");
                UNREACHABLE_RETRUN(0);
            }

            config->maxDepth = strtol(separator + 1, NULL, 10);
            if (config->maxDepth <= 0) {
                exitErr_badUsage("max depth must be greater than zero.");
                UNREACHABLE_RETRUN(0);
            }

            return 1;
        }
    }

    UNREACHABLE_RETRUN(1);
}


CliConfig* cli_parseArgs(int argc, const char** argv) {
    CliConfig* config;
    CliParams* params;
    
    /* Allocate memory for params & config */
    params = (CliParams*)malloc(sizeof(CliParams));
    if (params == NULL) {
        exitErr_noMem();
        UNREACHABLE_RETRUN(NULL);
    }

    config = (CliConfig*)malloc(sizeof(CliConfig));
    if (config == NULL) {
        free(params);

        exitErr_noMem();
        UNREACHABLE_RETRUN(NULL);
    }

    
    /* Initialize the params */
    params->pointer = 1;
    params->argc = argc;
    params->argv = argv;


    /* Default the config */
    config->rootPath = NULL;
    config->maxDepth = 1;


    /* Consume the options */
    while (consumeNextOption(params, config));


    /* If we still have next args, store it as the file */
    if (hasNext(params)) {
        const char* arg = consumeArgument(params);
        uint8_t isQuotted = (arg[0] == '"' || arg[0] == '\'')
            && (
                str_endsWithChar(arg, '"', strlen(arg)) 
                || str_endsWithChar(arg, '\'', strlen(arg))
            );
        int32_t strLen = strlen(arg) + (isQuotted ? -2 : 1);


        /* Allocate memory for the file */
        config->rootPath = (char*)malloc(sizeof(char) * strLen);
        if (config->rootPath == NULL) {
            free(params);
            exitErr_noMem();
            UNREACHABLE_RETRUN(NULL);
        }

        
        /* Copy the memory of the argument to the config */
        str_copy(config->rootPath, (isQuotted ? arg + 1 : arg), strLen);
    } else {
        config->rootPath = NULL;
    }


    free(params);
    return config;
}


void cli_destroy(CliConfig* config) {
    if (config == NULL) return;

    if (config->rootPath != NULL) {
        free(config->rootPath);
        config->rootPath = NULL;
    }

    free(config);
}