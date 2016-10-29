
#include "asm.h"
#include "utils.h"
#include "arraylist.h"
#include "asmparsecommand.h"

#include "asmConstant.h"
#include "asmParseCommand.h"
#include "asmUtils.h"

/*
 *  Defenition
 */

void convertProgram(int startCode, const char* text[], int currIndex,  int size);
int parseCommand(const char* str, CmdStructPtr cmd);
void parseMacro(int startCode, const char* text[], int index, int size);

/*
 *  Implementation
 */

int assembly(const char* text[], int size)
{
    uint16_t startCode = 0;
    int i;

    const char* str = NULL;

    for(i = 0; i < size; i++)
    {
        str = text[i];

        if((str[0] == ';'))
        {
            continue;
        }
        else if(strStartWith(str, synaxKey[SKEY_ORIGIN]))
        {
            str = str + strlen(synaxKey[SKEY_ORIGIN]);
            sscanf(str, "%ol", &startCode);
            printf("\nStart code: %0o\n", startCode);
        }
        else if(strStartWith(str, synaxKey[SKEY_START]))
        {
            convertProgram(startCode, text, i+1, size);
        }
    }

    return TRUE;
}

void convertProgram(int startCode, const char* text[], int currIndex,  int size)
{
    int i;

    CmdStruct cmds[256];
    CmdStructPtr cmd = NULL;

    int sizeCmds = 0;
    uint16_t address = startCode;

    for(i = currIndex; i < size; i++)
    {
        cmd = &(cmds[sizeCmds]);

        if(strStartWith(text[i], synaxKey[SKEY_DONE]))
        {
            parseCommand(text[i] + strlen(synaxKey[SKEY_DONE]), cmd);
            funcConvertCmd[cmd->cmd](cmd, &address);

            parseMacro(address, text, i+1, size);
            break;
        }

        parseCommand(text[i], cmd);

        funcConvertCmd[cmd->cmd](cmd, &address);

        sizeCmds++;
    }

    for(i = 0; i < sizeCmds; i++)
    {
        printf("\n\t%o CMD: %s, PARAM1: %s, PARAM2: %s",
               cmds[i].address, cmdName[cmds[i].cmd], cmds[i].param1, cmds[i].param2);
    }

    array_print();
}

void convertAsci(const char* src, uint16_t **dst, uint16_t *dstSize)
{
    const uint16_t startAddr = 02000;

    const char *start = strchr(src, '\"') + 1;
    const char *end = strchr(start, '\"');

    uint16_t len = end - start;
    int flag = 0;

    if(len % 2 == 0)
    {
        len++;
        flag = 1;
    }

    *dst = (uint16_t*)malloc(len * 2 * sizeof(uint16_t));

    int i = 0, j = 0;

    while(start != end)
    {
        (*dst)[i] = startAddr + (i*2);

        (*dst)[i+1] = 0;
        (*dst)[i+1] = *start;

        if(j != len - 1)
            (*dst)[i+1] |= (*(start+1) << 8);
        else
            break;

        i += 2;
        j += 2;
        start += 2;
    }

    if(flag)
    {
        (*dst)[len - 1] = startAddr + ((len - 1)*2);
        (*dst)[len] = 0x00;
    }

    (*dstSize) = len;
}

int getOffsetCmd(const char* str)
{
    if(strCompare(str, "mov"))
    {
        return 4;
    }
    else if(strCompare(str, "clr"))
    {
        return 2;
    }
    else
        return 2;
}

int parseCommand(const char* str, CmdStructPtr cmd)
{
    char cmdName[32] = "";
    char param1[32] = "";
    char param2[32] = "";

    int err = sscanf(str, "%s %[^','], %s",
                     cmdName, param1, param2);

    cmd->cmd = convertCmdType(cmdName);
    strcpy(cmd->param1, param1);
    strcpy(cmd->param2, param2);

    return err;
}

void parseMacro(int startCode, const char* text[], int index, int size)
{
    int i;
    char buffer[128] = "";

    for(i = index; i < size; i++)
    {
        sscanf(text[i], "msga: .string \"%s", buffer);
        printf("%s\n", buffer);
    }
}

