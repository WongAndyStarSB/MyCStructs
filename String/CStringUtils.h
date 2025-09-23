#ifndef C_STRING_UTILS_H
#define C_STRING_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CStringStruct.h"




String convertToBits(int num) {
    String s = String_constructor("");
    unsigned int mask = 1 << (sizeof(num) * 8 - 1); // 10000...000
    for (int i = 0; i < sizeof(num) * 8; ++i) {
        if (mask & num) {
            String_append(&s, "1");
        } else {
            String_append(&s, "0");
        }
        mask >>= 1;
    }
    return s;
}

void getline(FILE *f, String* s) {
    int buf_size = s->capacity;
    if (s->capacity < 2) {
        buf_size = 100;
        String_reserve(s, buf_size);
    }
    fgets(s->str, buf_size, f);
    s->str[strcspn(s->str, "\n")] = '\0';
}


#endif // C_STRING_UTILS_H
