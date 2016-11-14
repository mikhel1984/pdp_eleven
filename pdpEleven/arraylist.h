
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "common.h"

uint16_t arrayInit(void);
uint16_t* arrayInstance(void);
void arrayPush(uint16_t value);
uint16_t arraySize(void);

uint16_t arrayCurrIndex();
void arraySetValue(uint16_t index, uint16_t value);
uint16_t arrayGetValue(uint16_t index);

void arrayPrint(void);

#endif // ARRAYLIST_H
