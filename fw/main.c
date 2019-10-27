#include "hal/stm32f4.h"
#include "hal/gpio.h"
#include "hal/systick.h"
#include "hal/delay.h"
#include "hal/rng.h"
#include "hal/layer.h"
#include "hal/dma2d.h"

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

extern unsigned long _ssdram;

const uint16_t GREEN = 0x07E0;
const uint16_t RED = 0xF800;

int main()
{
    GPIO_Conf(GPIOG, LED1, GPIO_MODE_OUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_VERYHIGHSPEED, GPIO_PUD_NONE);
    GPIO_Conf(GPIOG, LED2, GPIO_MODE_OUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_VERYHIGHSPEED, GPIO_PUD_NONE);

    //uint16_t framebuffer[320*240] = { 0 };

    uint16_t *buffer1 = malloc(320 * 240 * sizeof(uint16_t));
    uint16_t *buffer2 = malloc(320 * 240 * sizeof(uint16_t));

    uint16_t *frontbuffer = buffer1;
    uint16_t *backBuffer = buffer2;

    LAYER_SetWindow(LAYER1, 0, 0, 240, 320, PF_RGB565);
    LAYER_SetFramebuffer(LAYER1, (void *)(frontbuffer));
    LAYER_Reload(true);
    LAYER_Enable(LAYER1);

    DMA2D_Init();

    DMA2D_image_t output = { 0 };
    output.memoryAddr = (void *)backBuffer;
    output.pixelFormat = PF_RGB565;

    printf("Hello World!\r\n");

    uint16_t color = GREEN;
    while(1)
    {
        //printf("%f\r\n", color);

        output.color = color;
        DMA2D_SetOutput(&output, 240, 320);
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
