#include "stm32f4.h"
#include "gpio.h"
#include "systick.h"
#include "delay.h"
#include "rng.h"
#include "layer.h"
#include "dma2d.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <lua.h>
#include <lauxlib.h>

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

typedef struct {
    float r;       // a fraction between 0 and 1
    float g;       // a fraction between 0 and 1
    float b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    float h;       // angle in degrees
    float s;       // a fraction between 0 and 1
    float v;       // a fraction between 0 and 1
} hsv;

static uint16_t RGB2True(rgb in)
{
    uint8_t r = (uint8_t)(in.r * 255);
    uint8_t g = (uint8_t)(in.g * 255);
    uint8_t b = (uint8_t)(in.b * 255);

    r = r & 0x1F;
    g = g & 0x3F;
    b = b & 0x1F;
    return (r << 11) | (g << 5) | b;
}

static rgb   hsv2rgb(hsv in);

rgb hsv2rgb(hsv in)
{
    float      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0f) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0f) hh = 0.0;
    hh /= 60.0f;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0f - in.s);
    q = in.v * (1.0f - (in.s * ff));
    t = in.v * (1.0f - (in.s * (1.0f - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}

int main()
{
    lua_State *L = luaL_newstate();
    //luaL_dostring(L, "print('Hello World!')");
    lua_close(L);

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

    hsv hsv_col = { 0, 1, 1 };
    rgb rgb = hsv2rgb(hsv_col);
    uint16_t color = RGB2True(rgb);
    while(1)
    {
        hsv_col.h = fmodf((hsv_col.h + 1), 360);
        rgb = hsv2rgb(hsv_col);
        color = RGB2True(rgb);

        printf("%f\r\n", hsv_col.h);

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
        delay(16);
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
