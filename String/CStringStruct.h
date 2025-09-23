#ifndef C_STRING_STRUCT_H
#define C_STRING_STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef char bool;
#define true 1;
#define false 0;


typedef struct AndyCStringStruct {
    char* str;
    size_t length;
    size_t capacity;
} String;


String String_constructor(char* string);
String* String_default(String* _this);
String* String_initializer(String* _this, char* string);
String* String_clear(String* _this);
String* String_assign(String* _this, const char* string);
String* String_append(String* _this, const char* to_be_append);
String* String_insert(String* _this, const char* to_be_inserted, size_t index);
String* String_reserve(String* _this, size_t capacity);
String String_iToA(int num);
String* String_free(String* _this);


String String_constructor(char* string) {
    String obj;
    if (String_initializer(&obj, string) == NULL) {
        obj = "String_initializer failed\nwhere: String_constructor\n";
    }
    return obj;
}
String* String_default(String* _this) {
    _this->str = (char*)(malloc(sizeof(char)));
    if (_this->str == NULL) {
        printf("Memory allocation failed\nwhere: String_default\n");
        return NULL;
    }
    *(_this->str) = '\0';
    _this->length = 0;
    _this->capacity = 1;
    return _this;
}
String* String_initializer(String* _this, char* string) {
    if (_this == NULL) {
        printf("InvalidArgument: _this is NULL\nwhere: String_initializer\n");
        return NULL;
    }
    if (string == NULL) {
        printf("InvalidArgument: string is NULL\nwhere: String_initializer\n");
        return NULL;
    }
    size_t len = strlen(string);
    size_t capacity = len+1;
    _this->str = (char*)(malloc(capacity * sizeof(char)));
    if (_this->str == NULL) {
        printf("Memory allocation failed\nwhere: String_initializer\n");
        return NULL;
    }
    strcpy(_this->str, string);
    _this->length = len;
    _this->capacity = capacity;
    return _this;
}
String* String_assign(String* _this, const char* string) { 
    if (_this == NULL) {
        printf("InvalidArgument: _this is NULL\nwhere: String_assign\n");
        return NULL;
    }
    if (string == NULL) {
        printf("InvalidArgument: string is NULL\nwhere: String_assign\n");
        return NULL;
    }
    size_t len = strlen(string);
    size_t capacity = len+1;
    if (_this->str == NULL) {
        _this->str = (char*)(malloc(capacity * sizeof(char)));
        if (_this->str == NULL) {
            printf("Memory allocation failed.\nwhere: String_assign\n");
            return NULL;
        }
        _this->capacity = capacity;
    }
    if (capacity > _this->capacity) {
        _this->str = (char*)(realloc(_this->str, capacity * sizeof(char)));
        if (_this->str == NULL) {
            printf("Memory reallocation failed.\nwhere: String_assign\n");
            return NULL;
        }
        _this->capacity = capacity;
    }
    strcpy(_this->str, string);
    _this->length = len;
    return _this;
}
String String_constructNullState() {
    String ret = {NULL, 0, 0};
    return ret;
}
char String_isNullState(const String *s) {
    return (
        (
            s->str == NULL 
            && s->length == 0
            && s->capacity == 0
        ) || 
        (
            s == NULL
        )
    );
}
String* String_clear(String* _this) {
    *(_this->str) = '\0';
    _this->length = 0;
    return _this;
}
String* String_append(String* _this, const char* to_be_append) {
    if (_this == NULL) {
        printf("InvalidArgument: _this is NULL\nwhere: String_append\n");
        return NULL;
    }
    if (to_be_append == NULL) {
        printf("InvalidArgument: string is NULL\nwhere: String_append\n");
        return NULL;
    }
    size_t len = strlen(to_be_append);
    if (len == 0) {
        return _this;
    }
    if (_this->capacity < _this->length + len + 1) {
        if (String_reserve(_this, (_this->capacity * 2 + len)) == NULL) {
            printf("String_reserve failed\nwhere: String_append\n");
            return NULL;
        }
    }
    strcpy(_this->str + _this->length, to_be_append);
    _this->length += len;
    return _this;
}
String* String_insert(String* _this, const char* to_be_inserted, size_t index) {
    if (_this == NULL) {
        printf("InvalidArgument: _this is NULL\nwhere: String_insert\n");
        return NULL;
    }
    size_t len = strlen(to_be_inserted);
    if (_this->capacity < _this->length + len + 1) {
        if (String_reserve(_this, _this->capacity * 2 + len) == NULL) {
            puts("String_reserve failed\nwhere: String_append");
            return NULL;
        }
    }
    for (size_t i = _this->length-1; i >= index && i != __INT_MAX__; --i) {
        _this->str[i+len] = _this->str[i];
    }
    strcpy(_this->str + index, to_be_inserted);
    _this->length += len;
    return _this;
}
String* String_reverse(String* _this) {
    for (size_t i = 0; i < _this->length / 2; ++i) {
        char tmp = _this->str[0];
        _this->str[0] = _this->str[_this->length-i-1];
        _this->str[_this->length-i-1] = tmp;
    }
    return _this;
}
String* String_reserve(String* _this, size_t capacity) {
    if (_this == NULL) {
        printf("InvalidArgument: _this is NULL\nwhere: String_reserve\n");
        return NULL;
    }
    if (capacity <= _this->capacity) {
        return _this;
    }
    capacity = MAX(capacity, 16);
    _this->str = (char*)(realloc(_this->str, capacity * sizeof(char)));
    if (_this->str == NULL) {
        printf("Memory reallocation failed.\nwhere: String_reserve\n");
        return NULL;
    }
    _this->capacity = capacity;
    _this->str[capacity-1] = '\0';
    return _this;
}
String String_constructIToA(int num) {
    String ret;
    String_reserve(&ret, 3);
    while (num) {
        String_append(&ret, '0' + num % 10);
        num /= 10;
    }
    return *String_reverse(&ret);
}
String String_constructFormat(const char *fmt, ...) {
    va_list args;
    
    va_start(args, fmt);

    int size = vsnprintf(NULL, 0, fmt, args);
    if (size < 0) {
        va_end(args);
        return String_constructNullState();
    }
    char *buffer = (char *)(malloc(size + 1));
    if (!buffer) {
        va_end(args);
        return String_constructNullState();
    }
    vsnprintf(buffer, size + 1, fmt, args);
    va_end(args);
    String ret = String_constructor(buffer);
    free(buffer);
    return ret;
}
String* String_free(String* _this) {
    if (_this == NULL) {
        printf("InvalidArgument: _this is NULL\nwhere: String_free\n");
        return NULL;
    }
    free(_this->str);
    _this->str = NULL;
    _this->length = 0;
    _this->capacity = 0;
    return _this;
}

#undef MAX

#endif
