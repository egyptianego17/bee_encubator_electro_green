#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <string>
#include "../lib/GUI.h"
#include "../lib/STD_TYPES.h"

const uint8_t FONT_DEFAULT_SIZE = 3;
const uint8_t LCD_MAX_X = 128;
const uint8_t LCD_MAX_Y = 160;
const uint8_t MAX_TEXT_SIZE = 5;

typedef enum
{
  LEFT_ALLIGNMENT,
  RIGHT_ALLIGNMENT,
  MIDDLE_ALLIGNMENT
} allign;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void LCDInit()
{
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.invertDisplay(true);
  tft.fillScreen(ST77XX_WHITE);
  drawText("Queen Bee", MIDDLE_ALLIGNMENT, ST77XX_RED, 2, 50);
  drawText("Incubator", MIDDLE_ALLIGNMENT, ST77XX_RED, 2, 70);
  drawText("Powered by", MIDDLE_ALLIGNMENT, ST77XX_BLACK, 1, 110);
  drawText("Electro Green", MIDDLE_ALLIGNMENT, ST77XX_GREEN, 1, 125);
}

uint8_t drawText(const char *text, uint8_t allignment, uint16_t color, uint8_t size, uint8_t y)
{
  if ((size > MAX_TEXT_SIZE) || (y > LCD_MAX_X) || (strlen(text) > (LCD_MAX_X/FONT_DEFAULT_SIZE)))
  {
    return STD_TYPES_NOK;
  }
  else
  {
    switch (allignment)
    {
    case MIDDLE_ALLIGNMENT:
        tft.setCursor((LCD_MAX_X/2)-strlen(text)*FONT_DEFAULT_SIZE*size, y);
        break;
    
    case RIGHT_ALLIGNMENT:
        tft.setCursor(LCD_MAX_X-strlen(text)*FONT_DEFAULT_SIZE*size, y);
        break;

    case LEFT_ALLIGNMENT:
        tft.setCursor(0, y);
        break;

    default:
        break;
    }
    tft.setTextSize(size);
    tft.setTextWrap(true);
    tft.setTextColor(color);
    tft.print(text);
  }
  return STD_TYPES_OK;
}

void updateGUI(float temprature, float humidity, bool heaterStatus, bool fanSatuts)
{
  tft.fillScreen(ST77XX_WHITE);
  drawText("System is stable", MIDDLE_ALLIGNMENT, ST77XX_BLACK, 1, 20);
  drawText("  Temperature: ", LEFT_ALLIGNMENT, ST77XX_BLUE, 1, 50);
  tft.setTextColor(ST77XX_ORANGE);
  tft.print(temprature);
  drawText("  Humidity: ", LEFT_ALLIGNMENT, ST77XX_BLUE, 1, 70);
  tft.setTextColor(ST77XX_ORANGE);
  tft.print(humidity);
  drawText("  Heater: ", LEFT_ALLIGNMENT, ST77XX_BLUE, 1, 90);
  tft.setTextColor(ST77XX_ORANGE);
  if (heaterStatus == true)
  {
    tft.print("On");
  }
  else
  {
    tft.print("Off");
  }

  drawText("  Fan: ", LEFT_ALLIGNMENT, ST77XX_BLUE, 1, 110);
  tft.setTextColor(ST77XX_ORANGE);
  if (fanSatuts == true)
  {
    tft.print("On");
  }
  else
  {
    tft.print("Off");
  }
}

void warningScreen(const char warning[])
{
  tft.fillScreen(ST77XX_WHITE);
  drawText("System", MIDDLE_ALLIGNMENT, ST77XX_RED, 1, 60);
  drawText("Fail", MIDDLE_ALLIGNMENT, ST77XX_RED, 1, 80);
  drawText(warning, MIDDLE_ALLIGNMENT, ST77XX_RED, 1, 100);
}