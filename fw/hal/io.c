#include "io.h"

#include <stdio.h>

FILE *iouartin;
FILE *iouartout;
FILE *iodbgout;

void IO_Init()
{
    iouartin = fdopen(IO_UART_IN, "r+");
    iouartout = fdopen(IO_UART_OUT, "w+");
    iodbgout = fdopen(IO_DBG_OUT, "w+");
}
