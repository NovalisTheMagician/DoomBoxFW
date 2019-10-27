#include "hal/stm32f4.h"
#include "hal/rcc.h"
#include "hal/gpio.h"
#include "hal/sdram.h"
#include "hal/tft.h"
#include "hal/systick.h"
#include "hal/delay.h"
#include "hal/scb.h"
#include "hal/dbg.h"
#include "hal/uart.h"
#include "hal/rng.h"
#include "hal/spi.h"

#include "hal/interrupts.h"

extern unsigned long _sflashdata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _estack;
extern unsigned long _ssdram;
extern unsigned long _esdram;

extern int main();

void Startup();

static void FaultHandler()
{
    GPIO_Conf(GPIOG, 14, GPIO_MODE_OUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_VERYHIGHSPEED, GPIO_PUD_NONE);
    GPIO_WritePin(GPIOG, 14, 1);
    while(1);
}

static void UnusedHandler()
{
    while(1);
}

static void DefaultHandler()
{
    while(1);
}

__attribute__ ((section(".vectors"), used))
void (* const _vectors[107])(void) = 
{
    (void *)&_estack,
    Startup,
    NIMHandler,
    HardFaultHandler,
    MemFaultHandler,
    BusFaultHandler,
    UsageFaultHandler,
    0,
    0,
    0,
    0,
    SVCallHandler,
    DebugHandler,
    0,
    PendSVHandler,
    SystickHandler,
    WindowWatchdogHandler,
    PVDHandler,
    TampStampHandler,
    RTCWakeupHandler,
    FlashHandler,
    RCCHandler,
    EXTI0Handler,
    EXTI1Handler,
    EXTI2Handler,
    EXTI3Handler,
    EXTI4Handler,
    DMA1S0Handler,
    DMA1S1Handler,
    DMA1S2Handler,
    DMA1S3Handler,
    DMA1S4Handler,
    DMA1S5Handler,
    DMA1S6Handler,
    ADCHandler,
    CAN1TXHandler,
    CAN1RX0Handler,
    CAN1RX1Handler,
    CAN1SCEHandler,
    EXTI9_5Handler,
    TIM1BRK_TIM9Handler,
    TIM1UP_TIM10Handler,
    TIM1TRG_COM_TIM11Handler,
    TIM1CCHandler,
    TIM2Handler,
    TIM3Handler,
    TIM4Handler,
    I2C1EVHandler,
    I2C1ERHandler,
    I2C2EVHandler,
    I2C2ERHandler,
    SPI1Handler,
    SPI2Handler,
    USART1Handler,
    USART2Handler,
    USART3Handler,
    EXTI15_10Handler,
    RTCAlarmHandler,
    OTGFSWakeuphandler,
    TIM8BRK_TIM12Handler,
    TIM8UP_TIM13Handler,
    TIM8TRG_COM_TIM14Handler,
    TIM8CCHandler,
    DMA1S7Handler,
    FSMCHandler,
    SDMMC1Handler,
    TIM5Handler,
    SPI3Handler,
    UART4Handler,
    UART5Handler,
    TIM6_DACHandler,
    TIM7Handler,
    DMA2S0Handler,
    DMA2S1Handler,
    DMA2S2Handler,
    DMA2S3Handler,
    DMA2S4Handler,
    ETHHandler,
    ETHWakeupHandler,
    CAN2TXHandler,
    CAN2RX0Handler,
    CAN2RX1Handler,
    CAN2SCEHandler,
    OTGFSHandler,
    DMA2S5Handler,
    DMA2S6Handler,
    DMA2S7Handler,
    USART6Handler,
    I2C3EVHandler,
    I2C3ERHandler,
    OTGHSEP1_OUTHandler,
    OTGHSEP1_INHandler,
    OTGHSWakeupHandler,
    OTGHSHandler,
    DCMIHandler,
    CRYPHandler,
    HASH_RNGHandler,
    FPUHandler,
    UART7Handler,
    UART8Handler,
    SPI4Handler,
    SPI5Handler,
    SPI6Handler,
    SAI1Handler,
    LCDHandler,
    LCDERRHandler,
    DMA2DHandler
};

void EnableFPU()
{
    volatile uint32_t *CPACR = (uint32_t*)(0xE000ED88);
    *CPACR = (3 << 22) | (3 << 20);
}

void Startup()
{
    uint32_t *src = &_sflashdata;
    uint32_t *dest = &_sdata;

    while (dest < &_edata) *dest++ = *src++;
    dest = &_sbss;
    while (dest < &_ebss) *dest++ = 0;

    RCC_InitClocks();

    SYSTICK_Init();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | 
                    RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | 
                    RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | 
                    RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN | 
                    RCC_AHB1ENR_GPIOIEN | RCC_AHB1ENR_GPIOJEN | 
                    RCC_AHB1ENR_GPIOKEN;

    SDRAM_Init();

    UART_Init(UART1, 9600);

    RNG_Init();

    SPI_Init(SPI5);

    TFT_Init();

    EnableFPU();
    
    DBG_EnableSWO();

    main();
}
