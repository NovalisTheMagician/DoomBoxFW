#ifndef LCDCOMMANDS_H_
#define LCDCOMMANDS_H_

#define LCD_CMD_RESET				0x01 // Software reset
#define LCD_CMD_ID					0x04 // Display ID
#define LCD_CMD_SLEEP_OUT			0x11 // Sleep out register
#define LCD_CMD_DISP_INV_OFF		0x20 // Display inversion off
#define LCD_CMD_GAMMA				0x26 // Gamma register
#define LCD_CMD_DISPLAY_OFF			0x28 // Display off register
#define LCD_CMD_DISPLAY_ON			0x29 // Display on register
#define LCD_CMD_COLUMN_ADDR			0x2A // Column address register
#define LCD_CMD_PAGE_ADDR			0x2B // Page address register
#define LCD_CMD_GRAM				0x2C // GRAM register
#define LCD_CMD_MAC					0x36 // Memory Access Control register
#define LCD_CMD_PIXEL_FORMAT		0x3A // Pixel Format register
#define LCD_CMD_WDB					0x51 // Write Brightness Display register
#define LCD_CMD_WCD					0x53 // Write Control Display register
#define LCD_CMD_RGB_INTERFACE		0xB0 // RGB Interface Signal Control
#define LCD_CMD_FRC					0xB1 // Frame Rate Control register
#define LCD_CMD_BPC					0xB5 // Blanking Porch Control register
#define LCD_CMD_DFC					0xB6 // Display Function Control register
#define LCD_CMD_POWER1				0xC0 // Power Control 1 register
#define LCD_CMD_POWER2				0xC1 // Power Control 2 register
#define LCD_CMD_VCOM1				0xC5 // VCOM Control 1 register
#define LCD_CMD_VCOM2				0xC7 // VCOM Control 2 register
#define LCD_CMD_POWERA				0xCB // Power control A register
#define LCD_CMD_POWERB				0xCF // Power control B register
#define LCD_CMD_PGAMMA				0xE0 // Positive Gamma Correction register
#define LCD_CMD_NGAMMA				0xE1 // Negative Gamma Correction register
#define LCD_CMD_DTCA				0xE8 // Driver timing control A
#define LCD_CMD_DTCB				0xEA // Driver timing control B
#define LCD_CMD_POWER_SEQ			0xED // Power on sequence register
#define LCD_CMD_3GAMMA_EN			0xF2 // 3 Gamma enable register
#define LCD_CMD_INTERFACE			0xF6 // Interface control register
#define LCD_CMD_PRC					0xF7 // Pump ratio control register

#endif
