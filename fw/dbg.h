#ifndef DBG_H_
#define DBG_H_

#include "stm32f4.h"

typedef struct 
{
    volatile uint32_t DBGMCU_IDCODE;
    volatile uint32_t DBGMCU_CR;
    volatile uint32_t DBGMCU_APB1_FZ;
    volatile uint32_t DBGMCU_APB2_FZ;
} DBG_t;

#define DBG_BASE (0xE0042000)
#define DBG ((DBG_t *) DBG_BASE)

#define DBG_TRACE_MODE_ASYNC        ((uint32_t)(0 << 6))
#define DBG_TRACE_MODE_SYNC_1BIT    ((uint32_t)(1 << 6))
#define DBG_TRACE_MODE_SYNC_2BIT    ((uint32_t)(2 << 6))
#define DBG_TRACE_MODE_SYNC_4BIT    ((uint32_t)(3 << 6))

#define DBG_TRACE_IOEN              ((uint32_t)(1 << 5))

typedef struct 
{
    volatile uint32_t STIM[32];
} ITM_STIM_t;
#define ITM_STIM ((ITM_STIM_t *)(0xE0000000))

#define ITM_LOCK    (*(volatile uint32_t *)(0xE0000FB0))
#define ITM_TC      (*(volatile uint32_t *)(0xE0000E80))
#define ITM_TP      (*(volatile uint32_t *)(0xE0000E40))
#define ITM_TE      (*(volatile uint32_t *)(0xE0000E00))

#define ITM_TC_BUSY             ((uint32_t)(1 << 23))
#define ITM_TC_PRESCALE_SHIFT   ((uint32_t)(8))
#define ITM_TC_SWOENA           ((uint32_t)(1 << 4))
#define ITM_TC_DWTENA           ((uint32_t)(1 << 3))
#define ITM_TC_SYNCENA          ((uint32_t)(1 << 2))
#define ITM_TC_TSENA            ((uint32_t)(1 << 1))
#define ITM_TC_ITMENA           ((uint32_t)(1 << 0))

#define TPIU_PORTSIZE (*(volatile uint32_t *)(0xE0040004))
#define TPIU_PROTOCOL (*(volatile uint32_t *)(0xE00400F0))

#define DEMCR (*(volatile uint32_t *)(0xE000EDFC))
#define DEMCR_TRACEEN ((uint32_t)(1 << 24))

void DBG_EnableSWO();

#endif
