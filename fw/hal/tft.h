#ifndef TFT_H_
#define TFT_H_

#define LCD_WIDTH	240
#define LCD_HEIGHT	320

#define HFP   10
#define HSYNC 10
#define HBP   20

#define VFP   4
#define VSYNC 2
#define VBP   2

void TFT_Init();
void TFT_WaitForVSYNC();

#endif