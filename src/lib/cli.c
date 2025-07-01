#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h" /* Manditory before other imports */

#include "cli.h"
#include "../core/app.h"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

typedef struct {
    int pointer;
    int argc;
    const char** argv;
} CliParams;


typedef enum {
    OPTION_UNKNOWN,         /** Not an option */
    OPTION_INVALID,         /** Invalid option */
    OPTION_END_OF_OPTIONS,  /** -- or --end-of-options */
    OPTION_HELP,            /** -h or --help */
    OPTION_VERSION,         /** -v or --version */
    OPTION_MAX_DEPTH        /** -d or --max-depth */
} OptionType;

static void exitBadUsage(const char* message);

static void printHelpMenu();

static Bool hasNext(CliParams* params);

static const char* peekArgument(CliParams* params);

static const char* consumeArgument(CliParams* params);

static OptionType parseOptionType(const char* arg);

static Bool consumeNextOption(CliParams* params, CliConfig* config);


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

static void exitBadUsage(const char* message) {
    if (message == NULL) {
        app_exitError(
            EXIT_BAD_USAGE,
            "bad usage: use '" TREEROOT_FILE_NAME \
            " --help' for more information"
        );

        return;
    }


    char buffer[512];
    snprintf(
        buffer, 
        512, 
        "bad usage: %s " \
        "use '" TREEROOT_FILE_NAME \
        " --help' for more information",
        message
    );

    app_exitError(EXIT_BAD_USAGE, buffer);
}

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
        TREEROOT_FILE_NAME
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
    app_exit(EXIT_GODD);
}


static Bool hasNext(CliParams* params) {
    if (params->pointer >= params->argc) return FALSE;
    return TRUE;
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
                    if (STR_EQ(arg, "--end-of-options")) {
                        return OPTION_END_OF_OPTIONS;

                    } else if (STR_EQ(arg, "--help")) {
                        return OPTION_HELP;

                    } else if (STR_EQ(arg, "--version")) {
                        return OPTION_VERSION;

                    } else if (STR_STARTS_WITH(arg, "--max-depth")) {
                        return OPTION_MAX_DEPTH;

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


static Bool consumeNextOption(CliParams* params, CliConfig* config) {
    OptionType optionType;

    if (!hasNext(params)) return FALSE;

    switch ((optionType = parseOptionType(peekArgument(params)))) {
        case OPTION_INVALID:
        case OPTION_HELP:
        case OPTION_VERSION: {
            const char* arg = peekArgument(params);
            free(params);
            free(config);
            
            switch (optionType) {
                case OPTION_INVALID: {
                    char buffer[512];
                    snprintf(
                        buffer, 
                        512, 
                        "invalid option: %s is not a recognized option. " \
                        "use '" TREEROOT_FILE_NAME \
                        " --help' for more information",
                        arg
                    );

                    app_exitError(EXIT_BAD_USAGE, buffer);
                    break;
                }

                case OPTION_HELP: {
                    printHelpMenu();
                    break;
                }

                case OPTION_VERSION: {
                    printf(TREEROOT_RELEASE "\n");
                    app_exit(EXIT_GODD);
                    break;
                }

                default: break; /* Unreachable */
            }
            
            return FALSE; /* Unreachable */
        }

        case OPTION_UNKNOWN: {
            return FALSE;
        }

        case OPTION_END_OF_OPTIONS: {
            consumeArgument(params);
            return FALSE;
        }

        case OPTION_MAX_DEPTH: {
            const char* arg = consumeArgument(params);
            const char* separator = strrchr(arg, '=');

            if (!separator) {
                exitBadUsage(NULL);
                return FALSE; /* Unreachable */
            }

            config->maxDepth = strtol(separator + 1, NULL, 10);
            if (config->maxDepth <= 0) {
                exitBadUsage("max depth must be greater than zero.");
                return FALSE; /* Unreachable */
            }
            
            return TRUE;
        }
    }

    return TRUE; /* Unreachable? */
}


CliConfig* cli_parseArgs(int argc, const char** argv) {
    CliConfig* config;
    CliParams* params;
    
    /* Allocate memory for params & config */
    params = (CliParams*)malloc(sizeof(CliParams));
    if (params == NULL) {
        app_exitFatal(EXIT_NO_MEM, "Failed to allocate memory");
        return NULL; /* Unreachable */
    }

    config = (CliConfig*)malloc(sizeof(CliConfig));
    if (config == NULL) {
        free(params);

        app_exitFatal(EXIT_NO_MEM, "Failed to allocate memory");
        return NULL; /* Unreachable */
    }

    
    /* Initialize the params */
    params->pointer = 1;
    params->argc = argc;
    params->argv = argv;


    /* Default the config */
    config->file = NULL;
    config->maxDepth = 1;


    /* Consume the options */
    while (consumeNextOption(params, config));


    /* If we still have next args, store it as the file */
    if (hasNext(params)) {
        const char* arg = consumeArgument(params);
        Bool isQuotted = (arg[0] == '"' || arg[0] == '\'')
            && (STR_ENDS_WITH_CHAR(arg, '"') || STR_ENDS_WITH_CHAR(arg, '\''));
        UInt16 strLen = strlen(arg) + (isQuotted ? -2 : 1);

        /* Allocate memory for the file */
        config->file = (char*)malloc(sizeof(char) * strLen);
        if (config->file == NULL) {
            free(params);
            app_exitFatal(EXIT_NO_MEM, "Failed to allocate memory");
            return NULL; /* Unreachable */
        }

        
        /* Copy the memory of the argument to the config */
        troot_safeStrCpy(config->file, (isQuotted ? arg + 1 : arg), strLen);
    } else {
        config->file = NULL;
    }


    free(params);
    return config;
}


void cli_destroy(CliConfig* config) {
    if (config == NULL) return;

    if (config->file != NULL) {
        free(config->file);
        config->file = NULL;
    }

    free(config);
}