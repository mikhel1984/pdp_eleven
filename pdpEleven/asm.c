#include "asm.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

uint32_t* assembly(char* text, int size, char** result, int* result_size){

    uint32_t *buffer;
    buffer = (uint32_t*)malloc(1024*sizeof(uint32_t));
    memset(buffer, 0, 1024*sizeof(uint32_t));

    buffer[0] = 012701;
    buffer[1] = 012702;
//    return 012701;
    return buffer;
}
