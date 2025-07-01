#include "common.h" /* Manditory before other imports */

#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_WINDOWS
    #include <windows.h>
#endif /* OS_WINDOWS */

#ifdef OS_POSIX_COMPLIANT
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

Bool platform_pathExists(const char* path) {
#if defined(OS_WINDOWS)
    DWORD fileAttributes;

    if (path == NULL) return FALSE;


    /* Get the file attributes */
    fileAttributes = GetFileAttributesA(path);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) return FALSE;

    return TRUE;
#elif defined(OS_POSIX_COMPLIANT)
    struct stat fileStat;

    if (path == NULL) return FALSE;

    
    /* Try to stat the file */
    if (stat(path, &fileStat) != 0) return FALSE;

    return TRUE;
#else
    #error Unsupported platform, \
        no compliant implementation of function 'platform_fileExists'.
#endif
}


Bool platform_isFile(const char* path) {
#if defined(OS_WINDOWS)
    DWORD fileAttributes;

    if (path == NULL) return FALSE;


    /* Get the file attributes */
    fileAttributes = GetFileAttributesA(path);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) return FALSE;


    /* Check if the path is a directory */
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return FALSE;

    return TRUE;
#elif defined(OS_POSIX_COMPLIANT)
    struct stat fileStat;

    if (path == NULL) return FALSE;

    
    /* Try to stat the file */
    if (stat(path, &fileStat) != 0) return FALSE;


    /* Check if the path is a file */
    return S_ISREG(fileStat.st_mode);
    /* return (fileStat.st_mode & S_IFMT) == S_IFREG;*/
#else
    #error Unsupported platform, \
        no compliant implementation of function 'platform_isFile'.
#endif
}


Bool platform_isFolder(const char* path) {
#if defined(OS_WINDOWS)
    DWORD fileAttributes;

    if (path == NULL) return FALSE;


    /* Get the file attributes */
    fileAttributes = GetFileAttributesA(path);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) return FALSE;


    /* Check if the path is a directory */
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return TRUE;

    return FALSE;
#elif defined(OS_POSIX_COMPLIANT)
    struct stat fileStat;

    if (path == NULL) return FALSE;

    
    /* Try to stat the file */
    if (stat(path, &fileStat) != 0) return FALSE;


    /* Check if the path is a file */
    return S_ISDIR(fileStat.st_mode);
#else
    #error Unsupported platform, \
        no compliant implementation of function 'platform_isFolder'.
#endif
}


Bool platform_isLink(const char* path) {
#if defined(OS_WINDOWS)
    DWORD fileAttributes;

    if (path == NULL) return FALSE;


    /* Get the file attributes */
    fileAttributes = GetFileAttributesA(path);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) return FALSE;


    /* Check if the path is a directory */
    if (fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) return TRUE;

    return FALSE;
#elif defined(OS_POSIX_COMPLIANT)
    struct stat fileStat;

    if (path == NULL) return FALSE;

    
    /* Try to stat the file */
    if (lstat(path, &fileStat) != 0) return FALSE;


    /* Check if the path is a file */
    return S_ISLNK(fileStat.st_mode);
#else
    #error Unsupported platform, \
        no compliant implementation of function 'platform_isLink'.
#endif
}


Bool platform_isHidden(const char* path) {
#if defined(OS_WINDOWS)
    DWORD fileAttributes;

    if (path == NULL) return FALSE;


    /* Get the file attributes */
    fileAttributes = GetFileAttributesA(path);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) return FALSE;


    /* Check if the path is hidden */
    if (fileAttributes & FILE_ATTRIBUTE_HIDDEN) return TRUE;

    return FALSE;
#elif defined(OS_POSIX_COMPLIANT)
    char fileName[MAX_PATH_LENGTH];
    
    if (path == NULL) return FALSE;
    platform_extractFileName(path, fileName);
    
    return fileName[0] == '.';
#else
    #error Unsupported platform, \
        no compliant implementation of function 'platform_isHidden'.
#endif
}


void platform_normalizePath(char* path) {
    Bool isFirstToken = TRUE;
    int top = -1;
    int i = 0;
    const char pathSeparator[2] = { PATH_SEPARATOR, '\0' };
    char result[MAX_PATH_LENGTH] = "";
    char temp[MAX_PATH_LENGTH];
    char* parts[MAX_PATH_LENGTH];
    char* p;
    char* token;

    /* Copy the path */
    troot_safeStrCpy(temp, path, MAX_PATH_LENGTH);


    /* Make separators consistient */
    for (p = temp; *p; p++) {
#if defined(OS_WINDOWS)
        if (*p == '/') *p = '\\';
#elif defined(OS_POSIX_COMPLIANT)
        if (*p == '\\') *p = '/';
#else
    #error Unsupported platform, \
        no compliant implementation of function 'platform_findLastPathSep'.
#endif
    }


    /* Tokenize the path */
    token = strtok(temp, pathSeparator);
    while (token != NULL) {
        if (strcmp(token, ".") == 0) {
            if (isFirstToken) parts[++top] = token;
        
        } else if (strcmp(token, "..") == 0) {
            if (top >= 0) top--;
        } else {
            parts[++top] = token;
        }

        token = strtok(NULL, pathSeparator);
        isFirstToken = FALSE;
    }


    /* Rebuild the normalized path */
    for (i = 0; i <= top; i++) {
        strcat(result, (i == 0) ? "" : pathSeparator);
        strcat(result, parts[i]);
    }

    if (result[0] == '\0') strcpy(result, ".");

    troot_safeStrCpy(path, result, MAX_PATH_LENGTH);
}


const char* platform_findLastPathSep(const char* path) {
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


void platform_extractDirName(const char* path, char* outDir) {
    const char* lastSeparator = platform_findLastPathSep(path);
    UInt32 length = 0;

    if (!lastSeparator) {
        strcpy(outDir, ".");
        return;
    }

    length = lastSeparator - path;
    if (length == 0) {
        strncpy(outDir, path, 1);
        outDir[1] = '\0';
        return;
    }

    strncpy(outDir, path, length);
    outDir[length] = '\0';
    return;
}


void platform_extractFileName(const char* path, char* outDir) {
    const char* lastSeparator = platform_findLastPathSep(path);
    Int16 pathLen = strlen(path);
    UInt32 length = 0;

    if (!lastSeparator) {
        strcpy(outDir, path);
        return;
    }
    
    length = pathLen - (lastSeparator - path) - 1;
    if (length == 0) {
        strncpy(outDir, path, 1);
        outDir[1] = '\0';
        return;
    }

    strncpy(outDir, lastSeparator + 1, length);
    outDir[length] = '\0';

    return;
}


void platform_joinPaths(
    const char* dir, 
    const char* file, 
    char* outPath
) {
    UInt32 length = strlen(dir);

    if (length == 0) {
        troot_safeStrCpy(outPath, file, strlen(file));
        return;
    }

    if (dir[length - 1] == PATH_SEPARATOR) {
        snprintf(outPath, MAX_PATH_LENGTH, "%s%s", dir, file);
        return;
    }

    snprintf(outPath, MAX_PATH_LENGTH, "%s%c%s", dir, PATH_SEPARATOR, file);
    return;
}