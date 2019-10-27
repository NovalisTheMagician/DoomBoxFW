#include "tft.h"
#include "ltdc.h"
#include "gpio.h"
#include "rcc.h"
#include "sdram.h"

#include "nvic.h"

#include "delay.h"

#include "spi.h"

#include "LCDCommands.h"

#define CS_PORT GPIOC
#define CS_PIN 2

#define WRX_PORT GPIOD
#define WRX_PIN 13

#define SPI SPI5

static void SetupGPIO();
static void SendCmd(uint8_t cmd);
static void SendData(uint8_t data);
static void InitController();

void TFT_Init()
{
    SetupGPIO();
	InitController();

    RCC->PLLSAICFGR = (189 << 6) | (7 << 28); // 175 -> 6.25Mhz 182 -> 6.5Mhz 189 -> 6.75 Mhz

    RCC->DCKCFGR1 |= (1 << 16);

    RCC->CR |= RCC_CR_PLLSAION;
    while((RCC->CR & RCC_CR_PLLSAIRDY) == 0);
    RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;

    LTDC->SSCR = ((HSYNC - 1) << 16) | (VSYNC - 1);
    LTDC->BPCR = ((HSYNC + HBP - 1) << 16) | (VSYNC + VBP - 1);
    LTDC->AWCR = ((HSYNC + HBP + LCD_WIDTH - 1) << 16) | (VSYNC + VBP + LCD_HEIGHT - 1);
    LTDC->TWCR = ((HSYNC + HBP + LCD_WIDTH + HFP - 1) << 16) | (VSYNC + VBP + LCD_HEIGHT + VFP - 1);
	LTDC->BCCR = 0x00000000;

	LTDC->IER = (1 << 3);

    LTDC->GCR |= LTDC_GCR_LTDCEN;

	NVIC_EnableIRQ(LCD_IRQn);
}

static volatile int vblank = 0;

void TFT_WaitForVSYNC()
{
	while(!vblank);
    vblank = 0;
}

void LCDHandler()
{
    vblank = 1;
    LTDC->ICR = (1 << 3);
}

static void SetupGPIO()
{
	GPIO_AFConf(GPIOA, 3, 14);
	GPIO_AFConf(GPIOA, 4, 14);
	GPIO_AFConf(GPIOA, 6, 14);
	GPIO_AFConf(GPIOA, 11, 14);
	GPIO_AFConf(GPIOA, 12, 14);

	GPIO_AFConf(GPIOB, 0, 9);
	GPIO_AFConf(GPIOB, 1, 9);
	GPIO_AFConf(GPIOB, 8, 14);
	GPIO_AFConf(GPIOB, 9, 14);
	GPIO_AFConf(GPIOB, 10, 14);
	GPIO_AFConf(GPIOB, 11, 14);

	GPIO_AFConf(GPIOC, 6, 14);
	GPIO_AFConf(GPIOC, 7, 14);
	GPIO_AFConf(GPIOC, 10, 14);

	GPIO_AFConf(GPIOD, 3, 14);
	GPIO_AFConf(GPIOD, 6, 14);

	GPIO_AFConf(GPIOF, 10, 14);

	GPIO_AFConf(GPIOG, 6, 14);
	GPIO_AFConf(GPIOG, 7, 14);
	GPIO_AFConf(GPIOG, 10, 9);
	GPIO_AFConf(GPIOG, 11, 14);
	GPIO_AFConf(GPIOG, 12, 9);

	GPIO_Conf(WRX_PORT, WRX_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_VERYHIGHSPEED, GPIO_PUD_NONE);
	GPIO_Conf(CS_PORT, CS_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_VERYHIGHSPEED, GPIO_PUD_NONE);
}

static void SendCmd(uint8_t cmd)
{
	GPIO_WritePin(WRX_PORT, WRX_PIN, 0);
	GPIO_WritePin(CS_PORT, CS_PIN, 0);

	SPI_Transmit(SPI, cmd);
	delay(1);

	GPIO_WritePin(CS_PORT, CS_PIN, 1);
}

static void SendData(uint8_t data)
{
	GPIO_WritePin(WRX_PORT, WRX_PIN, 1);
	GPIO_WritePin(CS_PORT, CS_PIN, 0);

	SPI_Transmit(SPI, data);
	delay(1);

	GPIO_WritePin(CS_PORT, CS_PIN, 1);
}

static void InitController()
{
	SendCmd(LCD_CMD_RESET);

	SendCmd(LCD_CMD_FRC);
	SendData(0x00);
	SendData(0x1B);

	SendCmd(LCD_CMD_MAC);
#ifdef LCD_UPSIDE_DOWN
	SendData(0x08); // upside down
#else
	SendData(0xC8);
#endif

	SendCmd(LCD_CMD_RGB_INTERFACE);
	SendData(0xC2);

	SendCmd(LCD_CMD_DFC);
	SendData(0x0A);
	SendData(0xA7);
	SendData(0x27);
	SendData(0x04);

	SendCmd(LCD_CMD_COLUMN_ADDR);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0xEF);

	SendCmd(LCD_CMD_PAGE_ADDR);
	SendData(0x00);
	SendData(0x00);
	SendData(0x01);
	SendData(0x3F);

	SendCmd(LCD_CMD_INTERFACE);
	SendData(0x01);
	SendData(0x00);
	SendData(0x06);

	SendCmd(LCD_CMD_SLEEP_OUT);
	delay(1);

	SendCmd(LCD_CMD_DISPLAY_ON);
}
