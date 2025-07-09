#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../config.h" /* Manditory in all files */

#include "paths.h"
#include "strings.h"

#ifdef OS_WINDOWS
    #include <windows.h>
#endif /* OS_WINDOWS */

#ifdef OS_POSIX_COMPLIANT
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif /* OS_POSIX_COMPLIANT */

#ifdef OS_MAC
#endif /* OS_MAC */

#ifdef OS_LINUX
#endif /* OS_LINUX */

#ifdef OS_UNIX
#endif /* OS_UNIX */


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

static const char* findLastPathSep(const char* path);


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

static const char* findLastPathSep(const char* path) {
#if defined(OS_WINDOWS)
    const char* lastSlash = strrchr(path, '/');
    const char* lastBackslash = strrchr(path, '\\');

    if (!lastSlash) return lastBackslash;
    if (!lastBackslash) return lastSlash;

    return (lastSlash > lastBackslash) ? lastSlash : lastBackslash;
#elif defined(OS_POSIX_COMPLIANT)
    return strrchr(path, '/');
#else
    #error Unsupported platform, \
        no compliant implementation of function 'platform_findLastPathSep'.
#endif
}


char* path_normalize(const char* path) {
    /* Define function level macros */
#if defined(OS_WINDOWS)
    #define _TARGET '/'
#elif defined(OS_POSIX_COMPLIANT)
    #define _TARGET '\\'
#else
    #error Unsupported platform, \
        no compliant implementation of function 'path_normalize'.
#endif
    
    /* Validate the path */
    if (path == NULL) return NULL;

    
    /* Allocate memory for the new normalized path */
    char* tempPath = NULL;
    tempPath = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (tempPath == NULL) {
        return NULL;
    }

    str_copy(tempPath, path, MAX_PATH_LENGTH);


    /* Make the separators consistient */
    for (char* c = tempPath; *c; c++) {
        if (*c == _TARGET) *c = PATH_SEPARATOR;
    }


    /* Replace any '.' and '..' if it's not the first character */
    uint8_t isFirstToken = 1;
    int32_t top = -1;
    const char _PATH_SEP_STR[2] = { PATH_SEPARATOR, '\0' };
    char* parts[MAX_PATH_LENGTH];
    char* token = strtok(tempPath, _PATH_SEP_STR);
    
    while (token != NULL) {
        /* Check if the token is "." and not the first token */
        if (str_equals(token, ".", 1)) {
            if (isFirstToken) parts[++top] = token;
        } else if (str_equals(token, "..", 2)) {
            if (top >= 0) top--;
        } else {
            parts[++top] = token;
        }

        token = strtok(NULL, _PATH_SEP_STR);
        isFirstToken = 0;
    }


    /* Rebuild the path parts */
    char rebuiltPath[MAX_PATH_LENGTH] = "";
    for (int32_t i = 0; i <= top; i++) {
        strcat(rebuiltPath, (i == 0) ? "" : _PATH_SEP_STR);
        strcat(rebuiltPath, parts[i]);
    }


    /* If the rebuilt string is empty, return the "." path */
    if (rebuiltPath[0] == '\0') str_copy(rebuiltPath, ".", MAX_PATH_LENGTH);

    
    /* Allocate memory for the new normalized path */
    int32_t normalizedPathLen = strlen(rebuiltPath) + 1;
    char* normalizedPath = NULL;
    normalizedPath = (char*)malloc(sizeof(char) * normalizedPathLen);
    if (normalizedPath == NULL) {
        return NULL;
    }


    /* Copy the rebuilt path into the normalized one and resize the memory */
    str_copy(normalizedPath, rebuiltPath, normalizedPathLen);

    return normalizedPath;

    /* Undefine the function level macros */
#ifdef _TARGET
    #undef _TARGET
#endif /* _TARGET */
}


char* path_baseName(const char* path)  {
    /* Validate the path */
    if (path == NULL) return NULL;


    /* Normalize the path */
    char* nPath = path_normalize(path);


    /* Find the last separator char */
    const char* lastSeparator = findLastPathSep(nPath);


    /* If no path separator exists, return standard */
    if (!lastSeparator) {
        char* baseName = (char*)malloc(sizeof(char) * 2);
        if (baseName == NULL) {
            free(nPath);
            return NULL;
        }

        str_copy(baseName, ".", 2);

        free(nPath);
        return baseName;
    }


    /* Determine the length of the base name */
    int32_t length = strlen(lastSeparator) + 1;
    if (length <= 0) {
        free(nPath);
        return NULL;
    }


    /* Create allocation for the base name and copy into it */
    char* baseName = (char*)malloc(sizeof(char) * (length - 1));
    if (baseName == NULL) {
        free(nPath);
        return NULL;
    }

    str_copy(baseName, lastSeparator + 1, length - 1);

    free(nPath);
    return baseName;
}


uint8_t path_exists(const char* path) {
    /* Define function level macros */
#if defined(OS_WINDOWS)
    #define _CHECK(p) GetFileAttributesA(p) == INVALID_FILE_ATTRIBUTES
#elif defined(OS_POSIX_COMPLIANT)
    #define _CHECK(p) stat(p, NULL) != 0
#else
    #error Unsupported platform, \
        no compliant implementation of function 'path_exists'.
#endif 


    /* Validate the path */
    if (path == NULL) return 0;


    /* Normalize the path */
    char* nPath = path_normalize(path);


    /* Perform platform dependant check */
    if (_CHECK(nPath)) {
        free(nPath);
        return 0;
    }

    free(nPath);
    return 1;

    /* Undefine function level macros */
#ifdef _CHECK
    #undef _CHECK
#endif /* _CHECK */
}


uint8_t path_isFile(const char* path) {
    /* Validate the path */
    if (path == NULL) return 0;


    /* Normalize the path */
    char* nPath = path_normalize(path);


    /* Perform platform dependant check */
#if defined(OS_WINDOWS)
    /* Get the file attributes */
    DWORD fileAttributes = GetFileAttributesA(nPath);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        free(nPath);
        return 0;
    }


    /* Ensure the path isn't a directory */
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        free(nPath);
        return 0;
    }

#elif defined(OS_POSIX_COMPLIANT)
    /* Get the file stat */
    struct stat fileStat;
    if (stat(nPath, &fileStat) != 0) {
        free(nPath);
        return 0;
    }


    /* Ensure the path is a file */
    if (!S_ISREG(fileStat.st_mode)) {
        free(nPath);
        return 0;
    }

#else
    #error Unsupported platform, \
        no compliant implementation of function 'path_exists'.
#endif 

    free(nPath);
    return 1;
}


uint8_t path_isFolder(const char* path) {
    /* Validate the path */
    if (path == NULL) return 0;


    /* Normalize the path */
    char* nPath = path_normalize(path);


    /* Perform platform dependant check */
#if defined(OS_WINDOWS)
    /* Get the file attributes */
    DWORD fileAttributes = GetFileAttributesA(nPath);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        free(nPath);
        return 0;
    }


    /* Ensure the path is a link */
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        free(nPath);
        return 1;
    }

    free(nPath);
    return 0;
#elif defined(OS_POSIX_COMPLIANT)
    /* Get the file stat */
    struct stat fileStat;
    if (stat(nPath, &fileStat) != 0) {
        free(nPath);
        return 0;
    }


    /* Check if the file is a link */
    free(nPath);
    return S_ISDIR(fileStat.st_mode);
#else
    #error Unsupported platform, \
        no compliant implementation of function 'path_isFolder'.
#endif 
}


uint8_t path_isLink(const char* path) {
    /* Validate the path */
    if (path == NULL) return 0;


    /* Normalize the path */
    char* nPath = path_normalize(path);


    /* Perform platform dependant check */
#if defined(OS_WINDOWS)
    /* Get the file attributes */
    DWORD fileAttributes = GetFileAttributesA(nPath);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        free(nPath);
        return 0;
    }


    /* Ensure the path is a link */
    if (fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        free(nPath);
        return 1;
    }

    free(nPath);
    return 0;
#elif defined(OS_POSIX_COMPLIANT)
    /* Get the file stat */
    struct stat fileStat;
    if (lstat(nPath, &fileStat) != 0) {
        free(nPath);
        return 0;
    }


    /* Check if the file is a link */
    free(nPath);
    return S_ISLNK(fileStat.st_mode);
#else
    #error Unsupported platform, \
        no compliant implementation of function 'path_isLink'.
#endif 
}


uint8_t path_isHidden(const char* path) {
    /* Validate the path */
    if (path == NULL) return 0;


    /* Normalize the path */
    char* nPath = path_normalize(path);


    /* Perform platform dependant check */
#if defined(OS_WINDOWS)
    /* Get the file attributes */
    DWORD fileAttributes = GetFileAttributesA(nPath);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        free(nPath);
        return 0;
    }


    /* Ensure the path is hidden */
    if (fileAttributes & FILE_ATTRIBUTE_HIDDEN) {
        free(nPath);
        return 1;
    }

    free(nPath);
    return 0;
#elif defined(OS_POSIX_COMPLIANT)
    char* fileName = path_baseName(nPath);
    return fileName[0] == '.';
#else
    #error Unsupported platform, \
        no compliant implementation of function 'path_isHidden'.
#endif 
}
