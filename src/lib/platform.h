#ifndef treeroot_lib_platform_h
#define treeroot_lib_platform_h

#include "common.h" /* Manditory before other imports */


#define MAX_PATH_LENGTH 4096


#if defined(OS_WINDOWS)
    #define PATH_SEPARATOR '\\'
#elif defined(OS_POSIX_COMPLIANT)
    #define PATH_SEPARATOR '/'
#else
    #error Unsupported platform, \
        no compliant value for macro 'PATH_SEPARATOR'.
#endif


Bool platform_pathExists(const char* path);


Bool platform_isFile(const char* path);


Bool platform_isFolder(const char* path);


Bool platform_isLink(const char* path);


Bool platform_isHidden(const char* path);


void platform_normalizePath(char* path);


const char* platform_findLastPathSep(const char* path);


void platform_extractDirName(const char* path, char* outDir);


void platform_extractFileName(const char* path, char* outDir);


void platform_joinPaths(
    const char* dir, 
    const char* file, 
    char* outPath
);

#endif /* treeroot_lib_platform_h */