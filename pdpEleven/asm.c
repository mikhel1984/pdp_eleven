
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
int getValueFromDictionary(const char* srcStr);
void parseMacro(const char* text[], int size);
void convertAsci(uint16_t* addr, const char* str);

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
        {
            continue;
        }
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
    BOOL flag = FALSE;
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

const char* extractMacro(const char* str)
{
    int i;
    int len = strlen(str);

    for(i=0; i < len; i++)
    {
        if(str[i] == ':')
        {
            pushMacroToDictionary(str);
            return str+i+1;
        }
    }

    return str;
}

int parseCommand(const char* str, CmdStructPtr cmd)
{
    int err = 0;

    char cmdName[32] = "";
    char param1[32]  = "";
    char param2[32]  = "";

    str = extractMacro(str);
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

void pushMacroToDictionary(const char* srcStr)
{
    char *str = strdup(srcStr);

    char* pos = strchr(str, ':');
    *pos = '\0';

    dictAdd(macros, str, address);

    *pos = ':';
    free(str);
}

int getValueFromDictionary(const char* srcStr)
{
    char *str = strdup(srcStr);
    int value = 0;

    char* pos = strchr(str, ':');
    *pos = '\0';

    value = dictFind(macros, str, -1);

    *pos = ':';
    free(str);

    return value;
}

void parseMacro(const char* text[], int size)
{
    int i;
    char *str = NULL;
    int value = 0;

    for(i = 0; i < size; i++)
    {
        str = strTrim(text[i]);
        if(strStartWith(str, synaxKey[SKEY_END]) == TRUE)
            break;

        value = getValueFromDictionary(str);
        if(value == -1)
            continue;

        arraySetValue(value, address);

        convertAsci(&address, text[i]);
    }
}

