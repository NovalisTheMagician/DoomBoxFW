#ifndef TERM_H_
#define TERM_H_

#include <stdint.h>
#include <stdbool.h>

int TERM_Init();
int TERM_SetVisible(bool visible);
int TERM_Write(char c);

#endif
