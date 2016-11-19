
#include "asm.h"
#include "utils.h"
#include "arraylist.h"

#include "asmConstant.h"
#include "asmCommandHandler.h"
#include "asmUtils.h"

#include "dictionary.h"

dict_t macros = NULL;

/*
 *  Defenition
 */

void convertProgram(uint16_t address, const char* text[], int size);
void convertMacro(uint16_t* address , const char* text[], int size);

void convertAsci(uint16_t* addr     , const char* str);

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

        if(isComment(str))
            continue;
        else if(isExpression(str))
        {
            pushExprToDict(str);
        }
        else if(strStartWith(str, synaxKey[SKEY_ORIGIN]))
        {
            str = str + strlen(synaxKey[SKEY_ORIGIN]);
            sscanf(str, "%ho", &startCode);
            printf("\nStart code: %0o\n", startCode);
        }
        else if(strStartWith(str, synaxKey[SKEY_START]))
        {
            text[index] = text[index] + strlen(synaxKey[SKEY_START]);
            break;
        }
    }

    convertProgram(startCode, text + index, size-index);

    free(macros);

    return TRUE;
}

void convertProgram(uint16_t address, const char* text[], int size)
{
    const char* str = NULL;
    int i;
    CmdStruct cmd;
    cmd.address = address;

    for(i = 0; i < size; i++)
    {
        str = strTrim(text[i]);

        if(isComment(str))
            continue;

        parseCommand(str, &cmd);

        funcConvertCmd[cmd.cmd](&cmd);

        if(cmd.cmd == CMD_HALT)
            break;
    }

    convertMacro(&(cmd.address), text+i+1, size-i);
    arrayPrint();
}

void convertMacro(uint16_t* address, const char* text[], int size)
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

        arraySetValue(value, *address);

        convertAsci(address, text[i]);
    }
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
