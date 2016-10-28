
#include "arraylist.h"

#define ARRAY_MAX_SIZE 256

static uint16_t array[ARRAY_MAX_SIZE] = {};
static uint16_t arraySize = 0;

uint16_t* array_instance(void)
{
    return array;
}

void array_push(uint16_t value)
{
    array[arraySize++] = value;
}

uint16_t array_size(void)
{
    return arraySize;
}

uint16_t array_print(void)
{
    int i;

    printf("\n\n");
    for(i = 0; i < arraySize; i += 2)
    {
        printf("%o %o\n", array[i], array[i+1]);
    }
}
