
#include "asmUtils.h"
#include "asmConstant.h"
#include "utils.h"
#include "asmCommandHandler.h"

BOOL isEmpty(const char* str)
{
    if(str == NULL)
        return TRUE;

    return (strlen(str) == 0) ? TRUE : FALSE;
}

BOOL isComment(const char* str)
{
    return str[0] == ';' ? TRUE : FALSE;
}

BOOL isRegister(const char* name)
{
    int i;
    for(i = 0; i < REG_TOTAL; i++)
    {
        if(strCompare(name, regName[i]))
            return TRUE;
    }

    return FALSE;
}

BOOL isMacro(const char* macro)
{
    return (macro[0] == '#');
}

BOOL isReserveName(const char* name)
{
    return isCmdName(name) ||
           isCmdName(name) ||
           isRegister(name);
}

BOOL isLabel(const char* name)
{
    int poslastCh = strlen(name) - 1;
    return (name[poslastCh] == ':') ? TRUE : FALSE;
}

BOOL isExpression(const char* str)
{
    const char pos = strchr(str, '=');

    return (pos == NULL) ? FALSE : TRUE;
}

uint8_t getRegAddr(const char* name)
{
    for(uint8_t i = 0; i < REG_TOTAL; i++)
    {
        if(strCompare(name, regName[i]))
            return i;
    }

    return REG_UNKNOWN;
}
