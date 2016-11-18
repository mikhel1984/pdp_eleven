
#ifndef PARSERUTILS_H
#define PARSERUTILS_H

#include "dictionary.h"
#include "asmConstant.h"

int parseCommand(const char* str, CmdStructPtr cmd);
void pushExprToDict(const char* str);
char* dumpMacroName(const char* srcStr);
void pushMacroToDictionary(dict_t dictionary, const char* srcStr, uint16_t address);
int getValueFromDictionary(dict_t dictionary, const char* srcStr);

#endif // PARSERUTILS_H
