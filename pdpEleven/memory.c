
#include "memory.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

///
/// Memory map
///

static const uint32_t MEMORY_TOTAL_SIZE = MEMORY_IVT_SIZE + MEMORY_RAM_SIZE +
                      MEMORY_VIDEO_RAM_SIZE + MEMORY_ROM_SIZE + MEMORY_IO_SIZE +
                      REGISTER_NUMBER;

// offset
static const uint32_t OFFSET_IVT       = 0;
static const uint32_t OFFSET_RAM       = MEMORY_IVT_SIZE;
static const uint32_t OFFSET_VIDEO_RAM = MEMORY_IVT_SIZE + MEMORY_RAM_SIZE;
static const uint32_t OFFSET_ROM       = MEMORY_IVT_SIZE + MEMORY_RAM_SIZE + MEMORY_ROM_SIZE;
static const uint32_t OFFSET_IO        = MEMORY_IVT_SIZE + MEMORY_RAM_SIZE + MEMORY_ROM_SIZE +
                                         MEMORY_ROM_SIZE;

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

