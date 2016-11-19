
#ifndef ASMUTILS_H
#define ASMUTILS_H

#include "common.h"

BOOL isEmpty(const char* str);
BOOL isComment(const char* str);
BOOL isRegister(const char* name);
BOOL isMacro(const char* macro);
BOOL isReserveName(const char *name);
BOOL isLabel(const char* name);
BOOL isExpression(const char* str);

uint8_t getRegAddr(const char* name);

enum ADDRESSING_MODE{
    AM_REGISTER  = 0x00,
    AM_AUTO_INCR = 0x02,
    AM_AUTO_DECR = 0x04,
    AM_INDEX     = 0x06,
    AM_UNDEFINED
};

int getAddrMode(const char* str);

enum DEFERED_ADDR_MODE{
    DAM_REGISTER  = 0x01,
    DAM_AUTO_INCR = 0x03,
    DAM_AUTO_DECR = 0x05,
    DAM_INDEX     = 0x07,
    DAM_UNDEFINED
};

int getDeferedAddrMode(const char* str);

enum PC_ADDRESSING_MODE{
    PAM_IMMIDIATE = 0x02,
    PAM_ABSOLUTE  = 0x03,
    PAM_RELATIVE  = 0x06,
    PAM_REL_DEF   = 0x07,
    PAM_UNDEFINED
};

int getPcAddrMode(const char* str);

#endif // ASMUTILS_H
