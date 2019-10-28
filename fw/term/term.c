#include "term.h"

typedef struct _font
{
    const char *name;
    const uint8_t *data;
    unsigned int w;
    unsigned int h;
} term_font_t;

#include "fonts/normal2.h"

static bool drawToScreen = true;

static void redrawBuffer()
{

}

int TERM_Init()
{

}

int TERM_SetVisible(bool visible)
{

}

int TERM_Write(char c)
{
    
}
