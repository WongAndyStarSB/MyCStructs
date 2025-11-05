#ifndef MY_STRING_STRUCT_H
#define MY_STRING_STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "StringUtils.h"


#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct CStringStruct {
    char* str;
    size_t length;
    size_t capacity;
} String;

char String_err_msg[100];

bool String_initEmpty(String* _this);
bool String_initFromStr(String* _this, const char* string);
bool String_initFromChar(String *_this, size_t n, char c);
bool String_initCopy(String *_this, const String *other);
bool String_initFromAllocatedStr(String* _this, char *allocated_str);
bool String_clear(String* _this);
bool String_assign(String* _this, const char* string);
bool String_append(String* _this, const char* to_be_append);
bool String_insert(String* _this, const char* to_be_inserted, size_t index);
bool String_reserve(String* _this, size_t capacity);
bool String_iToA(String *result, int num);
bool String_free(String* _this);


bool String_initEmpty(String* _this) {
    _this->str = (char*)(malloc(sizeof(char)));
    if (_this->str == NULL) {
        strcpy(String_err_msg, "Memory allocation failed\nwhere: String_initEmpty\n");
        return false;
    }
    *(_this->str) = '\0';
    _this->length = 0;
    _this->capacity = 1;
    return true;
}
bool String_initFromStr(String* _this, const char* string) {
    if (_this == NULL) {
        strcpy(String_err_msg, "InvalidArgument: _this is NULL\nwhere: String_initFromStr\n");
        return false;
    }
    if (string == NULL) {
        strcpy(String_err_msg, "InvalidArgument: string is NULL\nwhere: String_initFromStr\n");
        return false;
    }
    size_t len = strlen(string);
    size_t capacity = len+1;
    _this->str = (char*)(malloc(capacity * sizeof(char)));
    if (_this->str == NULL) {
        strcpy(String_err_msg, "Memory allocation failed\nwhere: String_initFromStr\n");
        return false;
    }
    strcpy(_this->str, string);
    _this->length = len;
    _this->capacity = capacity;
    return true;
}
bool String_initFromChar(String *_this, size_t n, char c) {
    _this->str = (char*)(malloc((n+1) * sizeof(char)));
    if (_this->str == NULL) {
        strcpy(String_err_msg, "Memory allocation failed\nwhere: String_initFromStr\n");
        return false;
    }
    _this->capacity = n+1;
    for (int i = 0; i < n; ++i) {
        _this->str[i] = c;
    }
    _this->str[n] = '\0';
    _this->length = n;
    return true;
}

bool String_initCopy(String *_this, const String *other) {
    if (_this == NULL) {
        strcpy(String_err_msg, "InvalidArgument: _this is NULL\nwhere: String_initCopy\n");
        return false;
    }
    if (other == NULL) {
        strcpy(String_err_msg, "InvalidArgument: other is NULL\nwhere: String_initCopy\n");
        return false;
    }
    _this->str = (char *)malloc(other->capacity * sizeof(char));
    if (_this->str == NULL) {
        strcpy(String_err_msg, "Memory Allocation Failed\nwhere: String_initCopy\n");
        return false;
    }
    memcpy(_this->str, other->str, other->length+1);
    _this->length = other->length;
    _this->capacity = other->capacity;
    return true;
}
bool String_initFromAllocatedStr(String* _this, char *allocated_str) {
    if (allocated_str == NULL) {
        strcpy(String_err_msg, "InvalidArgument: allocated_str is NULL\nwhere: String_initFromAllocatedStr\n");
        return false;
    }
    
    _this->str = allocated_str;

}
bool String_assign(String* _this, const char* string) { 
    if (_this == NULL) {
        strcpy(String_err_msg, "InvalidArgument: _this is NULL\nwhere: String_assign\n");
        return false;
    }
    if (string == NULL) {
        strcpy(String_err_msg, "InvalidArgument: string is NULL\nwhere: String_assign\n");
        return false;
    }
    size_t len = strlen(string);
    size_t capacity = len+1;
    if (_this->str == NULL) {
        _this->str = (char*)(malloc(capacity * sizeof(char)));
        if (_this->str == NULL) {
            strcpy(String_err_msg, "Memory allocation failed.\nwhere: String_assign\n");
            return false;
        }
        _this->capacity = capacity;
    }
    if (capacity > _this->capacity) {
        _this->str = (char*)(realloc(_this->str, capacity * sizeof(char)));
        if (_this->str == NULL) {
            strcpy(String_err_msg, "Memory reallocation failed.\nwhere: String_assign\n");
            return false;
        }
        _this->capacity = capacity;
    }
    strcpy(_this->str, string);
    _this->length = len;
    return true;
}
// String String_constructNullState() {
//     String ret = {NULL, 0, 0};
//     return ret;
// }
// char String_isNullState(const String *s) {
//     return (
//         (
//             s->str == NULL 
//             && s->length == 0
//             && s->capacity == 0
//         ) || 
//         (
//             s == NULL
//         )
//     );
// }
bool String_clear(String* _this) {
    *(_this->str) = '\0';
    _this->length = 0;
    return true;
}
bool String_appendChar(String *_this, char c) {
    if (_this == NULL) {
        strcpy(String_err_msg, "InvalidArgument: _this is NULL\nwhere: String_appendChar\n");
        return false;
    }
    if (_this->capacity - _this->length == 1) {
        _this->str = realloc(_this->str, _this->capacity * 2 * sizeof(char));
        if (_this->str == NULL) {
            strcpy(String_err_msg, "MemoryAllocationFailed: \nwhere: String_appendChar\n");
            return false;
        }
        _this->capacity <<= 1;
    }
    _this->str[_this->length++] = c;
    _this->str[_this->length] = '\0';
    return true;
}
bool String_append(String* _this, const char* to_be_append) {
    if (_this == NULL) {
        strcpy(String_err_msg, "InvalidArgument: _this is NULL\nwhere: String_append\n");
        return false;
    }
    if (to_be_append == NULL) {
        strcpy(String_err_msg, "InvalidArgument: string is NULL\nwhere: String_append\n");
        return false;
    }
    size_t len = strlen(to_be_append);
    if (len == 0) {
        return _this;
    }
    if (_this->capacity < _this->length + len + 1) {
        if (!String_reserve(_this, (_this->capacity * 2 + len))) {
            strcpy(String_err_msg, "String_reserve failed\nwhere: String_append\n");
            return false;
        }
    }
    strcpy(_this->str + _this->length, to_be_append);
    _this->length += len;
    return true;
}
bool String_insert(String* _this, const char* to_be_inserted, size_t index) {
    if (_this == NULL) {
        strcpy(String_err_msg, "InvalidArgument: _this is NULL\nwhere: String_insert\n");
        return false;
    }
    size_t len = strlen(to_be_inserted);
    if (_this->capacity < _this->length + len + 1) {
        if (String_reserve(_this, _this->capacity * 2 + len) == false) {
            puts("String_reserve failed\nwhere: String_append");
            return false;
        }
    }
    for (size_t i = _this->length-1; i >= index && i != __INT_MAX__; --i) {
        _this->str[i+len] = _this->str[i];
    }
    strcpy(_this->str + index, to_be_inserted);
    _this->length += len;
    return true;
}
bool String_reverse(String* _this) {
    for (size_t i = 0; i < _this->length / 2; ++i) {
        char tmp = _this->str[0];
        _this->str[0] = _this->str[_this->length-i-1];
        _this->str[_this->length-i-1] = tmp;
    }
    return true;
}
bool String_reserve(String* _this, size_t capacity) {
    if (_this == NULL) {
        strcpy(String_err_msg, "InvalidArgument: _this is NULL\nwhere: String_reserve\n");
        return false;
    }
    if (capacity <= _this->capacity) {
        return false;
    }
    capacity = MAX(capacity, 16);
    _this->str = (char*)(realloc(_this->str, capacity * sizeof(char)));
    if (_this->str == NULL) {
        strcpy(String_err_msg, "Memory reallocation failed.\nwhere: String_reserve\n");
        return false;
    }
    _this->capacity = capacity;
    _this->str[capacity-1] = '\0';
    return true;
}
bool String_iToA(String *result, int num) {
    String_reserve(result, 3);
    while (num) {
        String_appendChar(result, (char)('0' + (num % 10)));
        num /= 10;
    }
    String_reverse(result);
    return true;
}
bool String_initFmt(String *result, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Initial buffer size
    size_t size = 256;
    char* buffer = (char*)(malloc(size));
    if (buffer == NULL) {
        strcpy(String_err_msg, "Memory allocation failed\nwhere: String_initFmt\n");
        // exit procedure when fail (buffer has no mem-allocated)
        va_end(args);
        return false;
    }
    // Try to format the string
    size_t needed = (size_t)(vsnprintf(buffer, size, fmt, args));
    if (needed < 0) {
        strcpy(String_err_msg, "Formatting failed (1)\nwhere: String_initFmt\n");
        // exit procedure when fail
        free(buffer);
        va_end(args);
        return false;
    }
    // Resize buffer if necessary
    if (needed >= size) {
        size = needed + 1;
        buffer = (char*)(realloc(buffer, size));
        if (buffer == NULL) {
            strcpy(String_err_msg, "Memory allocation failed\nwhere: String_initFmt\n");
            // exit procedure when fail
            free(buffer);
            va_end(args);
            return false;
        }
        needed = vsnprintf(buffer, size, fmt, args);
        if (needed < 0) {
            strcpy(String_err_msg, "Formatting failed (2)\nwhere: String_initFmt\n");
            // exit procedure when fail
            free(buffer);
            va_end(args);
            return false;
        }
    }

    va_end(args);

    // finish the String object
    if (!String_initFromStr(result, buffer)) {
        free(buffer);
        strcpy(String_err_msg, "String_initFromStr failed\nwhere: String_initFmt\n");
        return false;
    }

    free(buffer);
    return true;
}
bool String_initFmtInt(String *result, const char *fmt, ...) {
    exit(1);
    const size_t MAX_NUM_OF_INTS = 256;
    size_t cnt = 0;
    size_t indices[MAX_NUM_OF_INTS];
    char previous = '\0';
    for (size_t i = 0; i < strlen(fmt); i++) {
        if (fmt[i] == '}' && previous == '{') {
            if (cnt == MAX_NUM_OF_INTS) {
                return false;
            }
            indices[cnt++] = i-1;
        }
        previous = fmt[i];
    }
    if (!cnt) {
        String_initFromStr(result, fmt);
        return true;
    }
    int *begin = (int *)((char *)fmt + 1);
    int *end = (int *)(begin + cnt);
    for (int *it = begin; it != end; ++it) {
        
    }
    sizeof(int);
    return false;
}
bool String_free(String* _this) {
    if (_this == NULL) {
        strcpy(String_err_msg, "InvalidArgument: _this is NULL\nwhere: String_free\n");
        return false;
    }
    free(_this->str);
    _this->str = NULL;
    _this->length = 0;
    _this->capacity = 0;
    return true;
}

#undef MAX

#endif