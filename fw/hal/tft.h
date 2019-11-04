#ifndef TFT_H_
#define TFT_H_

//#define LCD_MODE_PORTRAIT

#ifndef LCD_MODE_PORTRAIT
#define LCD_WIDTH	320
#define LCD_HEIGHT	240 + 80
#else
#define LCD_WIDTH	240
#define LCD_HEIGHT	320
#endif

#define HFP   10
#define HSYNC 10
#define HBP   20

#define VFP   4
#define VSYNC 2
#define VBP   2

void TFT_Init();
void TFT_WaitForVSYNC();

#endif