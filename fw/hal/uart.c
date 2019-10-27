#include "uart.h"
#include "gpio.h"
#include "rcc.h"

void UART_Init(UART_t *uart, int baud)
{
    if(uart == UART1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_UART1;

        GPIO_AFConf(GPIOA, 9, 7);
        GPIO_AFConf(GPIOA, 10, 7);

        uart->CR1 = UART_CR1_UE;

        int divisor = 90000000 / (16 * baud);
        uart->BRR = (divisor & 0xFFF) << 4;
        uart->CR1 |= UART_CR1_TE | UART_CR1_RE;
    }
}

void UART_Send(UART_t *uart, uint8_t data)
{
    while(!(uart->SR & UART_SR_TXE));
    uart->DR = data;
}

uint8_t UART_Receive(UART_t *uart)
{
    while(!(uart->SR & UART_SR_RXNE));
    return uart->DR;
}
