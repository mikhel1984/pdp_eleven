
#include "system.h"
#include "memory.h"
#include "logo.h"
#include "font.h"
#include "processor.h"

/// Defenition
void loadLogo(void);
void loadFonts(void);


/// Public function

BOOL systemInitialize(void)
{
    if(!memmoryInitialize())
        return FALSE;

    loadLogo();
    loadFonts();
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

void loadFonts(void)
{
    unsigned int i, j;
    uint8_t *mem =  getMemory(FONTS_ADR);

    for(i = 0; i < sizeof(font)/(8*sizeof(char)); ++i) {
        for(j = 0; j < 8; ++j) {
            *mem = font[i][j];
            mem++;
        }
    }
}
