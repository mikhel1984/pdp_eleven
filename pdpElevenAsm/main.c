
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
    const char** buf = getTestProgram1();
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

const char** getTestProgram3()
{
    static const char *buf[] = {
        "kbdaddr=60 ; interrupt entry point for kbd - set to address of handler routine",
        "kbdb=177562 ; data register for console input",
        "fonts=177000",
        "lettersize=10 ; lettersize = 8bytes",
        "videoram=100000",
        "start: mov #kdraw,@#kbdaddr ; write our handle to IVT",
        "kdraw: MOV r0,-(sp) ; push to stack",
        "  MOV r1,-(sp) ; push to stack",
        "  MOV r2,-(sp) ; push to stack",
        "  MOV r3,-(sp) ; push to stack",
        ";",
        "  movb @#kbdb, r0",
        "  mov #videoram, r1",
        "  mov #lettersize, r2",
        ";",
        "  mul 10, r0",
        "  add fonts, r0",
        ";",
        "   l1: movb (r0)+, r1",
        "      add 40, r1",
        "     dec r2",
        "     bne l1",
        ";",
        "   MOV (sp)+, r0 ; pop from stack",
        "   MOV (sp)+, r1 ; pop from stack",
        "   MOV (sp)+, r2 ; pop from stack",
        "   MOV (sp)+, r3 ; pop from stack",
        "   rti"
    };

    return buf;
}
