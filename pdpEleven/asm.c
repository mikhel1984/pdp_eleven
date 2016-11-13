
#include "asm.h"
#include "utils.h"
#include "arraylist.h"
#include "asmparsecommand.h"

#include "asmConstant.h"
#include "asmParseCommand.h"
#include "asmUtils.h"

#include "dictionary.h"

dict_t macros = NULL;

uint16_t address = 0;

/*
 *  Defenition
 */

void convertProgram(const char* text[], int currIndex,  int size);
int parseCommand(const char* str, CmdStructPtr cmd);
int getValueFromDictionary(const char* srcStr);
void parseMacro(uint16_t* addr, const char* text[], int index, int size);
void convertAsci(uint16_t* addr, const char* str);

/*
 *  Implementation
 */

int assembly(const char* text[], int size)
{
    uint16_t startCode = 0;
    int i;

    macros = dictNew();

    const char* str = NULL;

    for(i = 0; i < size; i++)
    {
        str = strTrim(text[i]);

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
            text[i] = text[i] + strlen(synaxKey[SKEY_START]);
            convertProgram(text, i, size);
        }
    }

    free(macros);

    return TRUE;
}

void convertProgram(const char* text[], int currIndex,  int size)
{
    int i;

    CmdStruct cmds[256];
    CmdStructPtr cmd = NULL;

    int sizeCmds = 0;
    const char* str = NULL;

    for(i = currIndex; i < size; i++)
    {
        cmd = &(cmds[sizeCmds]);

        if(strStartWith(text[i], synaxKey[SKEY_DONE]))
        {
            parseCommand(text[i] + strlen(synaxKey[SKEY_DONE]), cmd);
            funcConvertCmd[cmd->cmd](cmd, &address);

            address += 2;

            parseMacro(&address, text, i+1, size);
            break;
        }

        str = strTrim(text[i]);
        parseCommand(str, cmd);

        funcConvertCmd[cmd->cmd](cmd, &address);

        sizeCmds++;
    }

    for(i = 0; i < sizeCmds; i++)
    {
        printf("\n\t%o CMD: %s, PARAM1: %s, PARAM2: %s",
               cmds[i].address, cmdName[cmds[i].cmd], cmds[i].param1, cmds[i].param2);
    }

    arrayPrint();
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
    int value = 0;

    char* pos = strchr(str, ':');
    *pos = '\0';

    dictAdd(macros, str, address);

    *pos = ':';
    free(str);
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
    char param1[32] = "";
    char param2[32] = "";

    str = extractMacro(str);
    str = strTrim(str);

    err = sscanf(str, "%s %[^','], %s",
                     cmdName, param1, param2);

    cmd->cmd = convertCmdType(cmdName);
    strcpy(cmd->param1, param1);
    strcpy(cmd->param2, param2);

    return err;
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

void parseMacro(uint16_t* addr, const char* text[], int index, int size)
{
    int i;
    char *str = NULL;
    int value = 0;

    for(i = index; i < size; i++)
    {
        str = strTrim(text[i]);
        if(strStartWith(str, synaxKey[SKEY_END]) == TRUE)
            break;

        value = getValueFromDictionary(str);
        if(value == -1)
            continue;

        arraySetValue(value, *addr);

        convertAsci(addr, text[i]);
    }
}

