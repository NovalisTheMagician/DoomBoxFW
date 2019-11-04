#ifndef IO_H_
#define IO_H_

#include <stdio.h>

#define IO_UART_OUT 3
#define IO_UART_IN 4

#define IO_DBG_OUT 5

extern FILE *iouartin;
extern FILE *iouartout;
extern FILE *iodbgout;

void IO_Init();

#endif