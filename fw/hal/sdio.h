#ifndef SDIO_H_
#define SDIO_H_

#include "stm32f4.h"

typedef struct 
{
    volatile uint32_t POWER;
    volatile uint32_t CLKCR;
    volatile uint32_t ARG;
    volatile uint32_t CMD;
    volatile uint32_t RESPCMD;
    volatile uint32_t RESP1;
    volatile uint32_t RESP2;
    volatile uint32_t RESP3;
    volatile uint32_t RESP4;
    volatile uint32_t DTIMER;
    volatile uint32_t DLEN;
    volatile uint32_t DCTRL;
    volatile uint32_t DCOUNT;
    volatile uint32_t STA;
    volatile uint32_t ICR;
    volatile uint32_t MASK;
    uint32_t reserved1[2];
    volatile uint32_t FIFOCNT;
    uint32_t reserved2[12];
    volatile uint32_t FIFO;
} SDIO_t;


#endif