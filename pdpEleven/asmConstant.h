
#ifndef ASMCONSTANT_H
#define ASMCONSTANT_H

#include "dictionary.h"

enum SyntaxKeyType
{
    SKEY_ORIGIN = 0x00,
    SKEY_START,
    SKEY_DONE,
    SKEY_END,
    SKEY_TOTAL
};

static const char* synaxKey[SKEY_TOTAL] = {
    ".origin",
    "start:",
    "done: ",
    "end: "
};

enum RegType
{
    R0 = 0x00, R1, R2, R3, R4, R5, R6, REG_TOTAL, REG_UNKNOWN
};

static const char* regName[REG_TOTAL] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6"
};

enum CmdType
{
    CMD_MOV = 0x00, CMD_CLR, CMD_BR, CMD_MOVB, CMD_INC,  CMD_HALT, CMD_TOTAL, CMD_UNKNOWN = 0x00
};

static const char* cmdName[CMD_TOTAL] = {
    "mov", "clr", "br", "movb", "inc", "halt"
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
