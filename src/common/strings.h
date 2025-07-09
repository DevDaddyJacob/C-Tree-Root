#ifndef TREEROOT_SRC_COMMON_STRINGS_H
#define TREEROOT_SRC_COMMON_STRINGS_H

#include "../config.h" /* Manditory in all files */


#define char_isAlpha(c) \
    ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))

    
#define char_isDigit(c) \
    (c >= '0' && c <= '9')


/**
 * \brief           Safely copies a string into a destination buffer, at most
 *                  copying size - 1 characters and always ensuring a null
 *                  termination
 * \param[out]      destination: The destination buffer to write into
 * \param[in]       source: The source to copy into the destination
 * \param[in]       destSize: The size of the destination buffer
 * \return          Returns the total number of characters copied including
 *                  the null terminator
 */
int32_t str_copy(char* destination, const char* source, int32_t destSize);


/**
 * \brief           Creates a close of the provided string up to size - 1
 *                  characters
 * \param[in]       str: The string to copy
 * \param[in]       size: The size of the destination buffer
 * \return          Returns the copied string
 */
char* str_clone(const char* str, int32_t size);


/**
 * \brief           Compares 2 strings for equality
 * \param[in]       str: The string to search in
 * \param[in]       target: The string to search for
 * \param[in]       targetLen: The number of characters in the target string
 * \return          Returns a 1 if the target matches the str, otherwise 0.
 * \note            Return value is a boolean treated integer
 */
uint8_t str_equals(const char* str, const char* target, int32_t targetLen);


uint8_t str_partialEquals(
    const char* str, 
    const char* target, 
    int32_t targetLen,
    int32_t strOffset
);


uint8_t str_startsWith(
    const char* str, 
    const char* target, 
    int32_t strLen,
    int32_t targetLen
);


uint8_t str_endsWith(
    const char* str, 
    const char* target, 
    int32_t strLen,
    int32_t targetLen
);


uint8_t str_startsWithChar(const char* str, char target);


uint8_t str_endsWithChar(const char* str, char target, int32_t strLen);


uint64_t str_hash64(const char* str);

#endif /* TREEROOT_SRC_COMMON_STRINGS_H */