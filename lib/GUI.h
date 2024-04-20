#ifndef _GUI_H
#define _GUI_H

#define TFT_CS        32 // Hallowing display control pins: chip select
#define TFT_RST       33 // Display reset
#define TFT_DC        25 // Display data/command select

void LCDInit();
uint8_t drawText(const char text[], uint8_t allignment, uint16_t color, uint8_t size, uint8_t y);
void updateGUI(float temprature, float humidity, bool heaterStatus, bool fanSatuts);
void warningScreen(const char *warning);

#endif

