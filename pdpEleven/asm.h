#ifndef ASM_H
#define ASM_H

#include "stdint.h"

extern uint32_t* assembly(char* text, int size, char** result, int* result_size);

#endif // ASM_H
