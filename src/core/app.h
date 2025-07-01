#ifndef treeroot_core_app_h
#define treeroot_core_app_h

#include "../lib/common.h" /* Manditory before other imports */


void app_setup();


void app_exit(Int16 exitCode);


void app_exitError(Int16 exitCode, const char* errorMessage);


void app_exitFatal(Int16 exitCode, const char* errorMessage);


void app_getNodesForPath(const char* path, Int16 maxDepth);

#endif /* treeroot_core_app_h */