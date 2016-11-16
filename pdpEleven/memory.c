
#include "memory.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

uint8_t* buffer = NULL;

bool memmoryInitialize()
{
    buffer = (uint8_t*)malloc(MEMMORY_TOTAL_SIZE);
    memset(buffer, 0, MEMMORY_TOTAL_SIZE);

    return (buffer) ? true : false;
}

const uint8_t* getVideoRom()
{
    return (buffer + OFFSET_VIDEO_RAM);
}

const uint8_t* getRom()
{
    return buffer + OFFSET_ROM;
}

uint8_t* getMemoryBuf()
{
    return buffer;
}
