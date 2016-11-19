
#include <stdio.h>
#include "asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <arraylist.h>

const char** getTestProgram1();
const char** getTestProgram2();
const char** getTestProgram3();

int main()
{
    const char** buf = getTestProgram3();
    assembly(buf, 41);

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

const char** getTestProgram3()
{
    static const char *buf[] = {
        "kbdaddr=60",
        "kbdb=177562",
        "fonts=177000",
        "lettersize=10",
        "videoram=100000",
        "strsizeaddr=5000",
        ";",
        ".origin 1000",
        "start:",
        "  mov #kdraw,@#kbdaddr",
        "  mov #videoram, @#strsizeaddr",
        "  jmp #application",
        ";",
        "; handle keyboard interrupt",
        "kdraw:",
        ";",
        "   mov r0,-(sp)",
        "   mov r1,-(sp)",
        "   mov r2,-(sp)",
        "   mov r3,-(sp)",
        ";",
        "   mov @#kbdb, r0",
        "   mov @#strsizeaddr, r1",
        "   mov #lettersize, r2",
        ";",
        "   mul #10, r0",
        "   add #fonts, r0",
        ";",
        "   l1: movb (r0)+, (r1)",
        "      add #40, r1",
        "      dec r2",
        "      bne l1",
        ";",
        "   inc @#strsizeaddr",
        ";",
        "   mov (sp)+, r0",
        "   mov (sp)+, r1",
        "   mov (sp)+, r2",
        "   mov (sp)+, r3",
        "   rti",
        ";",
        "application: nop",
        "      br application",
        ";",
        ".end start"
    };

    return buf;
}
