#include "asm.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

int assembly(char** text, int size, uint16_t** result, uint16_t* resultSize)
{
    // 10 11

    convertAsci(text[11], result, resultSize);

    return 1;
}

void convertAsci(const char* src, uint16_t **dst, uint16_t *dstSize)
{
    const uint16_t startAddr = 02000;

    const char *start = strchr(src, '\"') + 1;
    const char *end = strchr(start, '\"');

    uint16_t len = end - start;
    int flag = 0;

    if(len % 2 == 0)
    {
        len++;
        flag = 1;
    }

    *dst = (uint16_t*)malloc(len * 2 * sizeof(uint16_t));

    int i = 0, j = 0;

    while(start != end)
    {
        (*dst)[i] = startAddr + (i*2);

        (*dst)[i+1] = 0;
        (*dst)[i+1] = *start;

        if(j != len - 1)
            (*dst)[i+1] |= (*(start+1) << 8);
        else
            break;

        i += 2;
        j += 2;
        start += 2;
    }

    if(flag)
    {
        (*dst)[len - 1] = startAddr + ((len - 1)*2);
        (*dst)[len] = 0x00;
    }

    (*dstSize) = len;
}
