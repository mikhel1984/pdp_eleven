
#include "asm.h"
#include "utils.h"
#include "arraylist.h"

#include "asmConstant.h"
#include "asmCommandHandler.h"
#include "asmUtils.h"

#include "dictionary.h"

dict_t macros = NULL;

uint16_t address = 0;

/*
 *  Defenition
 */

void convertProgram(const char* text[], int size);
int parseCommand(const char* str, CmdStructPtr cmd);
void convertAsci(uint16_t* addr, const char* str);

char* dumpMacroName(const char* srcStr);

void pushMacroToDictionary(dict_t dictionary, const char* srcStr);
int getValueFromDictionary(dict_t dictionary, const char* srcStr);

void parseMacro(const char* text[], int size);

/*
 *  Implementation
 */

int assembly(const char* text[], int size)
{
    uint16_t startCode = 0;
    int index;
    const char* str = NULL;

    macros = dictNew();

    for(index = 0; index < size; index++)
    {
        str = strTrim(text[index]);

        if((str[0] == ';'))
            continue;
        else if(strStartWith(str, synaxKey[SKEY_ORIGIN]))
        {
            str = str + strlen(synaxKey[SKEY_ORIGIN]);
            sscanf(str, "%ho", &startCode);
            address = startCode;
            printf("\nStart code: %0o\n", startCode);
        }
        else if(strStartWith(str, synaxKey[SKEY_START]))
        {
            text[index] = text[index] + strlen(synaxKey[SKEY_START]);
            break;
        }
    }

    convertProgram(text + index, size-index);

    free(macros);

    return TRUE;
}

void convertProgram(const char* text[], int size)
{
    const char* str = NULL;
    int i;
    CmdStruct cmd;
    cmd.address = address;

    for(i = 0; i < size; i++)
    {
        str = strTrim(text[i]);
        parseCommand(str, &cmd);

        funcConvertCmd[cmd.cmd](&cmd);
        address = cmd.address;

        if(cmd.cmd == CMD_HALT)
            break;
    }

    parseMacro(text+i+1, size-i);
    arrayPrint();
}

int parseCommand(const char* str, CmdStructPtr cmd)
{
    int err = 0;

    char cmdName[32] = "";
    char param1[32]  = "";
    char param2[32]  = "";

    const char *pos = strchr(str, ':');
    if(pos)
    {
        pushMacroToDictionary(macros, str);
        str = pos + 1;
    }

    str = strTrim(str);

    err = sscanf(str, "%s %[^','], %s",
                     cmdName, param1, param2);

    cmd->cmd = convertCmdType(cmdName);
    strcpy(cmd->param1, param1);
    strcpy(cmd->param2, param2);

    return err;
}

void convertAsci(uint16_t* addr, const char* str)
{
    const char *start = strchr(str, '\"') + 1;
    const char *end = strchr(start, '\"');
    const char *curr = start;

    uint16_t len = end - start;
    uint16_t world = 0;

    while(curr != end)
    {
        arrayPush(*addr);
        *addr += 2;

        world = *curr;

        if(curr == end - 1)
        {
            arrayPush(world);
            break;
        }

        world |= (*(curr+1) << 8);
        arrayPush(world);

        curr += 2;
    }

    if(len % 2 == 0)
    {
        arrayPush(*addr);
        *addr += 2;

        arrayPush(0x00);
    }
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

void pushMacroToDictionary(dict_t dictionary, const char* srcStr)
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

void parseMacro(const char* text[], int size)
{
    int i;
    const char *str = NULL;
    int value = 0;

    for(i = 0; i < size; i++)
    {
        str = strTrim(text[i]);
        if(strStartWith(str, synaxKey[SKEY_END]) == TRUE)
            break;

        value = getValueFromDictionary(macros, str);
        if(value == -1)
            continue;

        arraySetValue(value, address);

        convertAsci(&address, text[i]);
    }
}

