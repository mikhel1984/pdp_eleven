
#include <stdio.h>
#include "asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <arraylist.h>

const char** getTestProgram1();
const char** getTestProgram2();

int main()
{
    const char** buf = getTestProgram2();
    assembly(buf, 13);

    return 0;
}

const char** getTestProgram1()
{
    static const char *buf[] = {
        "; Program to copy and determine length of string",
        ".origin 1000",
        "start: mov #msga, r1",
        "  mov #msgb, r2",
        "  clr r0",
        "l1: movb (r1)+, (r2)+",
        "  beq done",
        "  inc r0",
        "  br l1",
        "done: halt",
        "msga: .string \"A string whose length is to be determined\"",
        "msgb: .string \"Different string that should get overwritten\"",
        ".end start"
    };

    return buf;
}

const char** getTestProgram2()
{
    static const char *buf[] = {
        "; Program to copy and determine length of string",
        ".origin 1000",
        "imagerom=10",
        "videoram=5000",
        "videosize=1000",
        "start: mov #imagerom, r1",
        "   mov #videoram, r2",
        "   mov #videosize, r0",
        "l1: movb (r1)+, (r2)+",
        "   dec r0",
        "   bne l1",
        "done: halt",
        ".end start"
    };

    return buf;
}
