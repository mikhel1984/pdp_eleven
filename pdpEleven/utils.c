
#include "utils.h"

BOOL strStartWith(const char* str1, const char* str2)
{
    int len = strlen(str2);
    return (strncmp(str1, str2, len) == 0) ? TRUE : FALSE;
}

BOOL strCompare(const char* s1, const char* s2)
{
    return (strcmp(s1, s2) == 0) ? TRUE : FALSE;
}

const char* strTrim(const char* str)
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
