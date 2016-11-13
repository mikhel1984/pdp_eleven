
#ifndef UTILS_H
#define UTILS_H

#include "common.h"

static BOOL strStartWith(const char* str1, const char* str2)
{
    int len = strlen(str2);
    return (strncmp(str1, str2, len) == 0) ? TRUE : FALSE;
}

static BOOL strCompare(const char* s1, const char* s2)
{
    return (strncmp(s1, s2, strlen(s2)) == 0) ? TRUE : FALSE;
}

static const char* strTrim(const char* str)
{
    int i;
    int len = strlen(str);

    for(i = 0; i < len; i++)
    {
        if(str[i] == ' ')
            continue;

        return str+i;
    }

    return NULL;
}

#endif // UTILS_H
