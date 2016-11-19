kbdaddr=60 ; interrupt entry point for kbd - set to address of handler routine
kbdb=177562 ; data register for console input
fonts=177000
lettersize=10 ; lettersize = 8bytes
videoram=100000
;
.origin 1000
start: 
  mov #kdraw,@#kbdaddr ; write our handle to IVT
  jmp application
;
; handle keyboard interrupt
kdraw:
;   
   mov r0,-(sp) ; push to stack
   mov r1,-(sp) ; push to stack
   mov r2,-(sp) ; push to stack
   mov r3,-(sp) ; push to stack
;
   movb @#kbdb, r0
   mov #videoram, r1
   mov #lettersize, r2
;
   mul #10, r0
   add #fonts, r0
;
   l1: movb (r0)+, (r1)
      add #40, r1
      dec r2
      bne l1
;
   mov (sp)+, r0 ; pop from stack
   mov (sp)+, r1 ; pop from stack
   mov (sp)+, r2 ; pop from stack
   mov (sp)+, r3 ; pop from stack
   rti
;
application: nop
      br application
;
.end start