#include <stdio.h>
#include "asm.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* file = fopen("/Users/vansickle/work/innopolis/ca/pdp_eleven/pdpElevenAsm/strcpylen.asm", "r");
    if (!file)
        return 2;

    int error;
    error = fseek(file, 0, SEEK_END);
//    if (error != 0)
//        return 1;
    int size = ftell(file);

    fseek(file, 0, SEEK_SET);

    char* buffer = (char *)malloc(size);
    error = fread(buffer, 1, size, file);

    if (error!=size)
        return 1;

    printf("asm size: %d\n", size);

    printf("%s", buffer);


    uint32_t* result = NULL;
    int result_size = 0;

    error = assembly(buffer, size, &result, &result_size);

    for(int i=0; i<result_size; i+=2){
        printf("%o %o\n", result[i], result[i+1]);
    }

//        printf("%o\n", result[0]);
    //    printf("%o\n", result[1]);

//    char* text = "";

//    uint32_t* result = assembly("ADD R0, R1");
//    printf("%o\n", result[0]);
//    printf("%o\n", result[1]);

    return 0;
}
