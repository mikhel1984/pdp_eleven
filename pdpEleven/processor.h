#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>


extern int evalCode();
extern int evalOneCircle(int *tact);
extern uint16_t *getRegister(uint8_t ind);

int testProcessor2();





#endif // PROCESSOR_H
