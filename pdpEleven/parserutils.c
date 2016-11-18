
#include "parserUtils.h"

const char* prepareString(dict_t macros, const char* str, uint32_t address)
{
    if(isEmpty(str))
        return str;

    return pushIfMacro(macros, str, address);
}

const char* pushIfMacro(dict_t macros, const char* str, uint16_t address)
{
    const char *pos = strchr(str, ':');
    if(!pos || (strStartWith(str, "done:") == TRUE))
        return str;

    pushMacroToDictionary(macros, str, address);
    str = strTrim(str);

    return pos + 1;
}

int parseCommand(const char* str, CmdStructPtr cmd)
{
    int err = 0;

    char cmdName[32] = "";
    char param1[32]  = "";
    char param2[32]  = "";

    str = strTrim(str);

    err = sscanf(str, "%s %[^','], %[^';']",
                     cmdName, param1, param2);

    cmd->cmd = convertCmdType(cmdName);
    strcpy(cmd->param1, param1);
    strcpy(cmd->param2, param2);

    return err;
}

void pushExprToDict(const char* str)
{
    int exprVal = 0;
    const char* pos = strchr(str, '=');

    int lenNameVar = pos - str;
    char* nameVar = (char*)malloc(sizeof(char)*lenNameVar+1);

    strncpy(nameVar, str, lenNameVar);
    nameVar[lenNameVar] = '\0';

    exprVal = strtol (pos+1,NULL,8);

    dictAdd(macros, nameVar, exprVal);

    free(nameVar);
}

char* dumpMacroName(const char* srcStr)
{
    const char* pos = strchr(srcStr, ':');
    int len = pos-srcStr+1;
    char* macroName = (char*)malloc(sizeof(char)*(len));

    strncpy(macroName, srcStr, len);
    macroName[len-1] = '\0';

    return macroName;
}

void pushMacroToDictionary(dict_t dictionary, const char* srcStr, uint16_t address)
{
    char *str = dumpMacroName(srcStr);
    dictAdd(dictionary, str, address);
    free(str);
}

int getValueFromDictionary(dict_t dictionary, const char* srcStr)
{
    char *str = dumpMacroName(srcStr);
    int value = dictFind(dictionary, str, -1);

    free(str);

    return value;
}
