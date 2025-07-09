#ifndef TREEROOT_SRC_COMMON_IO_H
#define TREEROOT_SRC_COMMON_IO_H

#include <stdarg.h>

#include "../config.h" /* Manditory in all files */


#define PRINT_FAILED -1
#define PRINT_PARTIAL_SUCCESS -2


int32_t errPrintF(const char* format, ...);

int32_t vErrPrint(const char* format, va_list vargs);


#endif /* TREEROOT_SRC_COMMON_IO_H */