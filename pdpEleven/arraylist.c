
#include "arraylist.h"

#define ARRAY_MAX_SIZE 256

static uint16_t array[ARRAY_MAX_SIZE] = {};
static uint16_t arrayCurrSize = 0;

uint16_t* arrayInstance(void)
{
    return array;
}

void arrayPush(uint16_t value)
{
    if(arrayCurrSize >= ARRAY_MAX_SIZE)
        return;

    array[arrayCurrSize++] = value;
}

uint16_t arraySize(void)
{
    return arrayCurrSize;
}

uint16_t arrayCurrIndex()
{
    return (arrayCurrSize != 0) ?
                arrayCurrSize - 1 : 0;
}

void arraySetValue(uint16_t index, uint16_t value)
{
    array[index] = value;
}

uint16_t arrayGetValue(uint16_t index){
    return array[index];
}

void arrayClear(void)
{
    arrayCurrSize = 0;
    memset(array, 0x00, sizeof(char)*ARRAY_MAX_SIZE);
}

void arrayPrint(void)
{
    int i;

    printf("\n\n");
    for(i = 0; i < arrayCurrSize; i += 2)
    {
        printf("%o %o\n", array[i], array[i+1]);
    }
}
