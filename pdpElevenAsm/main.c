#include <stdio.h>
#include "asm.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    /*
    FILE* file = fopen("E:\\Archive\\Archive\\Study\\Innopolis\\Pre-master\\Fall\\Computer achitecture\\Project\\pdp_eleven\\pdpElevenAsm\\strcpylen.asm", "r");
    if (!file)
        return 2;

    fseek(file, 0, SEEK_END);
    int size = ftell(file);

    fseek(file, 0, SEEK_SET);

    char* buffer = (char *)malloc(size);
    int error = fread(buffer, 1, size, file);

    if (error!=size)
        return 1;

    printf("asm size: %d\n", size);

    printf("%s", buffer);


    uint32_t* result = NULL;
    int result_size = 0;*/

    const char *buf[] = {
        "; Program to copy and determine length of string",
        ".origin 1000",
        "start: mov #msga, r1",
        "   mov #msgb, r2",
        "   clr r0",
        "l1: movb (r1)+, (r2)+",
        "   beq done",
        "   inc r0",
        "   br l1",
        "done: halt",
        "msga: .string \"A string whose length is to be determined\"",
        "msgb: .string \"Different string that should get overwritten\"",
        ".end start"
    };

    uint16_t *result = NULL;
    uint16_t resultSize = 0;

    assembly(buf, 13, &result, &resultSize);

    for(int i=0; i<resultSize; i+=2)
    {
        printf("%o %o\n", result[i], result[i+1]);
    }

    return 0;
}
