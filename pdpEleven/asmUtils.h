
#ifndef ASMUTILS_H
#define ASMUTILS_H

#include "common.h"

BOOL isComment(const char* str);
BOOL isRegister(const char* name);
BOOL isMacro(const char* macro);
BOOL isReserveName(const char *name);
BOOL isLabel(const char* name);
BOOL isExpression(const char* str);

uint8_t getRegAddr(const char* name);


#endif // ASMUTILS_H
