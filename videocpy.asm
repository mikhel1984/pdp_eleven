; Program to copy logo_bitmap from ROM to video ROM, 21450
.origin 00000
imagerom=140000
videoram=100000
videosize=20000
start: mov #imagerom, r1
   mov #videoram, r2
   mov #videosize, r0
l1: movb (r1)+, (r2)+
   dec r0
   bne l1
done: halt
.end start