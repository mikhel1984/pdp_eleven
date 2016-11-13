
#ifndef ASMCONSTANT_H
#define ASMCONSTANT_H

#include "dictionary.h"
#include "common.h"

enum RegType
{
    R0 = 0x00, R1, R2, R3, R4, R5, R6, REG_TOTAL, REG_UNKNOWN
};

static const char* regName[REG_TOTAL] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6"
};

typedef struct Cmd
{
    uint16_t address;
    int cmd;
    char param1[32];
    char param2[32];
} CmdStruct, *CmdStructPtr;

extern dict_t macros;

#endif // ASMCONSTANT_H
