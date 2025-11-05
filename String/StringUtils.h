#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * @brief Allocates and initializes a string with a repeated character.
 *
 * This function allocates memory for a string of length `len` and fills it with the character `c`.
 * The resulting string is null-terminated.
 *
 * @param[out]  uninit_s_ptr    Pointer to an uninitialized char * that will be allocated and filled.
 * @param[in]   len             Length of the string to allocate (excluding the null terminator).
 * @param[in]   c               Character to fill the string with.
 * 
 * @return true if successful, false on failure
 */
bool charToStr(char **restrict uninit_s, size_t len, char c);

/**
 * Adds indentation to a source string and stores the result in a newly allocated buffer.
 *
 * @param[out] result       Pointer to an uninitialized char* that will receive the indented string.
 *                           The function allocates memory for *result; caller must free it.
 * @param[in] src           Source string to indent. Must not overlap with *result.
 * @param[in] num_of_indent Number of indent characters to add per line.
 * @param[in] indent_char   Character used for indentation (e.g., ' ' or '\\t').
 *
 * @return true if successful, false on failure (invalid input or memory allocation error).
 *
 * @note
 * - *result must point to an uninitialized char* (or NULL).
 * - src and *result must not overlap (enforced by restrict qualifier).
 * - Caller is responsible for freeing *result after use.
 */
bool addIndent(char **restrict result, const char *restrict src, size_t num_of_indent, char indent_char);

bool charToStr(char **restrict uninit_s_ptr, size_t len, char c) {
    if (uninit_s_ptr == NULL || len == 0) {
        return false;
    }

    *uninit_s_ptr = (char *)(malloc((len+1) * sizeof(char)));
    if (*uninit_s_ptr == NULL) {
        return false;
    }
    for (size_t i = 0; i < len; ++i) {
        (*uninit_s_ptr)[i] = c;
    }
    (*uninit_s_ptr)[len] = '\0';
    return true;
}

bool addIndent(char **restrict result, const char *restrict src, size_t num_of_indent, char indent_char) {
    if (src == NULL) {
        return false;
    }
    if (result == NULL) {
        return false;
    }

    size_t len = strlen(src);

    if (num_of_indent == 0) {
        *result = (char *)malloc(len + 1);
        if (*result == NULL) {
            return false;
        }
        memcpy(*result, src, len+1);
        return true;
    }

    // init indent_need
    size_t indent_need = num_of_indent;
    for (size_t i = 0; i < len; ++i) {
        if (src[i] == '\n') { indent_need += num_of_indent; }
    }

    // mem-alloc for result
    *result = (char *)malloc(len + indent_need + 1);
    if (*result == NULL) {
        return false;
    }

    // add one indent beforehand
    size_t ri = 0; // ri: result_index
    for (; ri < num_of_indent; ++ri) {
        (*result)[ri] = indent_char;
    }

    // copy and add from src to (*result)
    for (size_t si = 0; si < len; ++si) { // si: src_index
        (*result)[ri++] = src[si];
        if (src[si] == '\n' && src[si+1] != '\0') {
            for (size_t i = 0; i < num_of_indent; ++i) {
                (*result)[ri++] = indent_char;
            }
        }
    }

    // put '\0' at the back of (*result)
    (*result)[ri] = '\0';
    return true;
}