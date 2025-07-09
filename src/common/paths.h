#ifndef TREEROOT_SRC_COMMON_PATHS_H
#define TREEROOT_SRC_COMMON_PATHS_H

#include "../config.h" /* Manditory in all files */


char* path_normalize(const char* path);


char* path_baseName(const char* path);


uint8_t path_exists(const char* path);


uint8_t path_isFile(const char* path);


uint8_t path_isFolder(const char* path);


uint8_t path_isLink(const char* path);


uint8_t path_isHidden(const char* path);

#endif  /* TREEROOT_SRC_COMMON_PATHS_H */