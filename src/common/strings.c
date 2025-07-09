#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../config.h" /* Manditory in all files */

#include "strings.h"


/*
 * ==================================================
 * Macros
 * ==================================================
 */

#define FNV_HASH_OFFSET 14695981039346656037UL
#define FNV_HASH_PRIME 1099511628211UL

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

int32_t str_copy(char* destination, const char* source, int32_t destSize) {
    /* Null check our arguments*/
    if (destination == NULL || source == NULL) return 0;


    /* Validate the destination size */
    if (destSize <= 0) return 0;


    /* Copy the characters up to destSize - 1 */
    int32_t i = 0;
    for (i = 0; (i < destSize - 1 && source[i] != '\0'); i++) {
        destination[i] = source[i];
    }


    /* Ensure the last character written is the null terminator */
    destination[i] = '\0';

    return i + 1;
}


char* str_clone(const char* str, int32_t size) {
    /* Null check our arguments*/
    if (str == NULL || size - 1 <= 0) return NULL;


    /* Allocate the new string */
    char* copy = (char*)malloc(sizeof(char) * size);
    if (copy == NULL) return NULL;


    /* Use safe string copy */
    str_copy(copy, str, size);

    return copy;
}


uint8_t str_equals(const char* str, const char* target, int32_t targetLen) {
    /* Handle null comparison */
    if (str == NULL || target == NULL) return str == NULL && target == NULL;


    /* Length validation */
    if (targetLen <= 0) return 0;


    /* Compare the memory of the strings */
    return memcmp(str, target, targetLen) == 0;
}


uint8_t str_partialEquals(
    const char* str, 
    const char* target, 
    int32_t targetLen,
    int32_t strOffset
) {
    /* Handle null comparison */
    if (str == NULL || target == NULL) return str == NULL && target == NULL;


    /* Length validation */
    if (strOffset <= 0 || targetLen <= 0) return 0;


    /* Compare the memory of the strings */
    return memcmp(str + strOffset, target, targetLen) == 0;
}


uint8_t str_startsWith(
    const char* str, 
    const char* target, 
    int32_t strLen,
    int32_t targetLen
) {
    /* Handle null comparison */
    if (str == NULL || target == NULL) return 0;


    /* Length validation */
    if (strLen <= 0 || targetLen <= 0) return 0;


    /* Safety check the lengths */
    if (targetLen > strLen) return 0;


    /* Compare the memory of the strings */
    return memcmp(str, target, targetLen) == 0;
}


uint8_t str_endsWith(
    const char* str, 
    const char* target, 
    int32_t strLen,
    int32_t targetLen
) {
    /* Handle null comparison */
    if (str == NULL || target == NULL) return 0;


    /* Length validation */
    if (strLen <= 0 || targetLen <= 0) return 0;


    /* Safety check the lengths */
    if (targetLen > strLen) return 0;


    /* Compare the memory of the strings */
    return memcmp(str + (strLen - targetLen), target, targetLen) == 0;
}


uint8_t str_startsWithChar(const char* str, char target) {
    /* Handle null comparison */
    if (str == NULL) return 0;


    /* Compare the memory of the strings */
    return str[0] == target;
}


uint8_t str_endsWithChar(const char* str, char target, int32_t strLen) {
    /* Handle null comparison */
    if (str == NULL) return 0;


    /* Length validation */
    if (strLen <= 0) return 0;


    /* Compare the memory of the strings */
    return str[strLen - 1] == target;
}


uint64_t str_hash64(const char* str) {
    uint64_t hash = FNV_HASH_OFFSET;

    for (const char* c = str; *c; c++) {
        hash ^= (uint64_t)(unsigned char)(*c);
        hash *= FNV_HASH_PRIME;
    }

    return hash;
}