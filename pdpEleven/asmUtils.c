
#include "asmUtils.h"
#include "asmConstant.h"
#include "utils.h"

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

BOOL isCmdName(const char* name)
{
    for(int i = 0; i < CMD_TOTAL; i++)
    {
        if(strCompare(name, cmdName[i]))
            return TRUE;
    }

    return FALSE;
}

BOOL isSynaxKey(const char* name)
{
    for(int i = 0; i < CMD_TOTAL; i++)
    {
        if(strCompare(name, synaxKey[i]))
            return TRUE;
    }

    return FALSE;
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

int convertCmdType(const char* str)
{
    if(strCompare(str, "mov"))
    {
        return CMD_MOV;
    }
    else if(strCompare(str, "clr"))
    {
        return CMD_CLR;
    }
    else if(strCompare(str, "movb"))
    {
        return CMD_MOVB;
    }
    else if(strCompare(str, "inc"))
    {
        return CMD_INC;
    }
    else if(strCompare(str, "br"))
    {
        return CMD_BR;
    }
    else if(strCompare(str, "halt"))
    {
        return  CMD_HALT;
    }
    else
        return 2;
}

int getRegAddr(const char* name)
{
    for(int i = 0; i < REG_TOTAL; i++)
    {
        if(strCompare(name, regName[i]))
            return i;
    }

    return REG_UNKNOWN;
}
