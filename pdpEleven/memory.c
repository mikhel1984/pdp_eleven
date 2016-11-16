
#include "memory.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

memory_ptr buffer = NULL;

bool memmoryInitialize(void)
{
    buffer = (uint8_t*)malloc(MEMORY_TOTAL_SIZE);
    memset(buffer, 0, MEMORY_TOTAL_SIZE);

    return (buffer) ? true : false;
}

void memmoryDestroy(void)
{
    free(buffer);
}

memory_ptr memoryGetVideoRom(void)
{
    return (buffer + OFFSET_VIDEO_RAM);
}

memory_ptr memoryGetRom(void)
{
    return buffer + OFFSET_ROM;
}

uint8_t* memoryGetPointer(void)
{
    return buffer;
}

