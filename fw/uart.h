#ifndef UART_H_
#define UART_H_

#include "stm32f4.h"

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} UART_t;

#define UARTA_BASE (APB2PERIPH_BASE)
#define UARTB_BASE (APB1PERIPH_BASE)

#define UART1 ((UART_t *)(UARTA_BASE + 0x1000))
#define UART2 ((UART_t *)(UARTB_BASE + 0x4400))
#define UART3 ((UART_t *)(UARTB_BASE + 0x4800))
#define UART4 ((UART_t *)(UARTB_BASE + 0x4C00))
#define UART5 ((UART_t *)(UARTB_BASE + 0x5000))
#define UART6 ((UART_t *)(UARTA_BASE + 0x1400))
#define UART7 ((UART_t *)(UARTB_BASE + 0x7800))
#define UART8 ((UART_t *)(UARTB_BASE + 0x7C00))

#define UART_SR_CTS     ((uint32_t)(1 << 9))
#define UART_SR_LBD     ((uint32_t)(1 << 8))
#define UART_SR_TXE     ((uint32_t)(1 << 7))
#define UART_SR_TC      ((uint32_t)(1 << 6))
#define UART_SR_RXNE    ((uint32_t)(1 << 5))
#define UART_SR_IDLE    ((uint32_t)(1 << 4))
#define UART_SR_ORE     ((uint32_t)(1 << 3))
#define UART_SR_NF      ((uint32_t)(1 << 2))
#define UART_SR_FE      ((uint32_t)(1 << 1))
#define UART_SR_PE      ((uint32_t)(1 << 0))

#define UART_CR1_OVER8  ((uint32_t)(1 << 15))
#define UART_CR1_UE     ((uint32_t)(1 << 13))
#define UART_CR1_M      ((uint32_t)(1 << 12))
#define UART_CR1_WAKE   ((uint32_t)(1 << 11))
#define UART_CR1_PCE    ((uint32_t)(1 << 10))
#define UART_CR1_PS     ((uint32_t)(1 << 9))
#define UART_CR1_PEIE   ((uint32_t)(1 << 8))
#define UART_CR1_TXEIE  ((uint32_t)(1 << 7))
#define UART_CR1_TCIE   ((uint32_t)(1 << 6))
#define UART_CR1_TXNEIE ((uint32_t)(1 << 5))
#define UART_CR1_IDLEIE ((uint32_t)(1 << 4))
#define UART_CR1_TE     ((uint32_t)(1 << 3))
#define UART_CR1_RE     ((uint32_t)(1 << 2))
#define UART_CR1_RWU    ((uint32_t)(1 << 1))
#define UART_CR1_SBK    ((uint32_t)(1 << 0))

#define UART_CR2_LINEN  ((uint32_t)(1 << 14))
#define UART_CR2_STOP_1 ((uint32_t)(0 << 12))
#define UART_CR2_STOP_05 ((uint32_t)(1 << 12))
#define UART_CR2_STOP_2 ((uint32_t)(2 << 12))
#define UART_CR2_STOP_15 ((uint32_t)(3 << 12))
#define UART_CR2_CLKEN  ((uint32_t)(1 << 11))
#define UART_CR2_CPOL   ((uint32_t)(1 << 10))
#define UART_CR2_CPHA   ((uint32_t)(1 << 9))
#define UART_CR2_LBCL   ((uint32_t)(1 << 8))
#define UART_CR2_LBDIE  ((uint32_t)(1 << 6))
#define UART_CR2_LBDL   ((uint32_t)(1 << 5))

#define UART_CR3_ONEBIT ((uint32_t)(1 << 11))
#define UART_CR3_CTSIE  ((uint32_t)(1 << 10))
#define UART_CR3_CTSE   ((uint32_t)(1 << 9))
#define UART_CR3_RTSE   ((uint32_t)(1 << 8))
#define UART_CR3_DMAT   ((uint32_t)(1 << 7))
#define UART_CR3_DMAR   ((uint32_t)(1 << 6))
#define UART_CR3_SCEN   ((uint32_t)(1 << 5))
#define UART_CR3_NACK   ((uint32_t)(1 << 4))
#define UART_CR3_HDSEL  ((uint32_t)(1 << 3))
#define UART_CR3_IRLP   ((uint32_t)(1 << 2))
#define UART_CR3_IREN   ((uint32_t)(1 << 1))
#define UART_CR3_EIE    ((uint32_t)(1 << 0))

void UART_Init(UART_t *uart, int baud);
void UART_Send(UART_t *uart, uint8_t data);
uint8_t UART_Receive(UART_t *uart);

#endif
