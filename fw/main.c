#include "hal/stm32f4.h"
#include "hal/gpio.h"
#include "hal/systick.h"
#include "hal/delay.h"
#include "hal/rng.h"
#include "hal/layer.h"
#include "hal/dma2d.h"
#include "hal/nvic.h"

#include "hal/io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LED1 13
#define LED2 14

#define WIDTH 320
#define HEIGHT 240

void led_on(uint32_t);
void led_off(uint32_t);
void led_toggle(uint32_t);
void drawRect(uint16_t*, int, int, int, int, int, uint16_t);
uint16_t colAdd(uint16_t, int);

extern unsigned long _ssdram;

const uint16_t GREEN = 0x07E0;
const uint16_t RED = 0xF800;
const uint16_t BLUE = 0x001F;
const uint16_t WHITE = 0xFFFF;
const uint16_t BLACK = 0x0000;

void DMA2DHandler()
{
    led_on(LED2);
}

int main()
{
    GPIO_Conf(GPIOG, LED1, GPIO_MODE_OUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_VERYHIGHSPEED, GPIO_PUD_NONE);
    GPIO_Conf(GPIOG, LED2, GPIO_MODE_OUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_VERYHIGHSPEED, GPIO_PUD_NONE);

    //uint16_t framebuffer[320*240] = { 0 };

    uint16_t *buffer1 = malloc(320 * 240 * sizeof(uint16_t));
    uint16_t *buffer2 = malloc(320 * 240 * sizeof(uint16_t));

    uint16_t *frontbuffer = buffer1;
    uint16_t *backBuffer = buffer2;

    LAYER_SetWindow(LAYER1, 0, 0, WIDTH, HEIGHT, PF_RGB565);
    LAYER_SetFramebuffer(LAYER1, frontbuffer);
    LAYER_Reload(true);
    LAYER_Enable(LAYER1);

    DMA2D_Init();

    NVIC_EnableIRQ(DMA2D_IRQn);
    DMA2D->CR |= DMA2D_CR_TEIE;
    DMA2D->CR |= DMA2D_CR_CEIE;

    DMA2D_image_t output = { 0 };
    output.memoryAddr = backBuffer;
    output.pixelFormat = PF_RGB565;

    fprintf(iouartout, "Hello World!\r\n");
    fprintf(iouartout, "Buffer1 Address: %p\r\n", buffer1);
    fprintf(iouartout, "Buffer2 Address: %p\r\n", buffer2);

    uint16_t color = BLACK;
    while(1)
    {
        //printf("%f\r\n", color);
        //color = colAdd(color, 1);

        output.color = color;
        output.offset = 0;
        DMA2D_SetOutput(&output, WIDTH, HEIGHT);
        DMA2D_StartTransfer(TT_REG_TO_MEM);

        output.color = GREEN;
        output.offset = WIDTH - 16;
        DMA2D_SetOutput(&output, 16, 16);
        DMA2D_StartTransfer(TT_REG_TO_MEM);

        output.color = RED;
        output.offset = WIDTH - 16;
        output.memoryAddr = backBuffer + (WIDTH - 16);
        DMA2D_SetOutput(&output, 16, 16);
        DMA2D_StartTransfer(TT_REG_TO_MEM);
        
        
        if(backBuffer == buffer1)
        {
            backBuffer = buffer2;
            frontbuffer = buffer1;
        }
        else
        {
            backBuffer = buffer1;
            frontbuffer = buffer2;
        }
        output.memoryAddr = backBuffer;

        LAYER_SetFramebuffer(LAYER1, frontbuffer);
        LAYER_Reload(false);
        TFT_WaitForVSYNC();

        //delay(100);
    }
}

uint16_t colAdd(uint16_t col, int val)
{
    int r = (col & RED) >> 11;
    int g = (col & GREEN) >> 5;
    int b = (col & BLUE);

    return (((r + 1) << 11) & RED) | (((g + 1) << 5) & GREEN) | ((b + 1) & BLUE);
}

void drawRect(uint16_t *buffer, int bufWidth, int x, int y, int width, int height, uint16_t color)
{
    for(int i = y; i < height; ++i)
    {
        for(int j = x; j < width; ++j)
        {
            int index = (y + i) * bufWidth + (x + j);
            buffer[index] = color;
        }
    }
}

void led_toggle(uint32_t led)
{
    static int state = 0;
    state = !state;
    GPIO_WritePin(GPIOG, led, state);
}

void led_on(uint32_t led)
{
    GPIO_WritePin(GPIOG, led, 1);
}


void led_off(uint32_t led)
{
    GPIO_WritePin(GPIOG, led, 0);
}
