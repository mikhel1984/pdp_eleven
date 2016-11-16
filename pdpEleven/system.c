
#include "system.h"
#include "memory.h"
#include "logo.h"

/// Defenition
void loadLogo(void);


/// Public function

BOOL systemInitialize(void)
{
    if(!memmoryInitialize())
        return FALSE;

    loadLogo();
}

void systemDestroy(void)
{
    memmoryDestroy();
}


/// Private function

void loadLogo(void)
{
    memory_ptr romPtr = memoryGetRom();
    int i;

    for(i = 0; i < (logoHeight*logoWidth)/8; i++)
        romPtr[i] = logoData[i];
}
