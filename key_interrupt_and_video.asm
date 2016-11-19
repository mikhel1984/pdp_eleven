kbdaddr=60
kbdb=177562
fonts=177000
lettersize=10
videoram=100000
strsizeaddr=5000
;
.origin 1000
start: 
  mov #kdraw,@#kbdaddr ; write our handle to IVT
  mov #videoram, @#strsizeaddr
  jmp #application
;
; handle keyboard interrupt
kdraw:
;   
   mov r0,-(sp)
   mov r1,-(sp)
   mov r2,-(sp)
   mov r3,-(sp)
;
   mov @#kbdb, r0
   mov @#strsizeaddr, r1
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
   inc @#strsizeaddr
;
   mov (sp)+, r0
   mov (sp)+, r1
   mov (sp)+, r2
   mov (sp)+, r3
   rti
;
application: nop
      br application
;
.end start