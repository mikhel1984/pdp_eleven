; Program to copy logo_bitmap from ROM to video ROM
.origin 1000
imagerom=10
videoram=5000
videosize=1000
start: mov #imagerom, r1
   mov #videoram, r2
   mov #videosize, r0
l1: movb (r1)+, (r2)+
   dec r0
   bne l1
done: halt
.end start