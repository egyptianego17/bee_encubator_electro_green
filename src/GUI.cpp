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


/**
 * @brief Initializes the LCD display.
 * 
 * This function initializes the LCD display by configuring the ST7735S chip,
 * setting the display inversion, filling the screen with white color, and
 * drawing some text on the screen.
 */
void LCDInit()
{
  tft.initR(INITR_BLACKTAB);      /* Init ST7735S chip, black tab */
  tft.invertDisplay(true);
  tft.fillScreen(ST77XX_WHITE);
  drawText("Queen Bee", MIDDLE_ALLIGNMENT, ST77XX_RED, 2, 50);
  drawText("Incubator", MIDDLE_ALLIGNMENT, ST77XX_RED, 2, 70);
  drawText("Powered by", MIDDLE_ALLIGNMENT, ST77XX_BLACK, 1, 110);
  drawText("Electro Green", MIDDLE_ALLIGNMENT, ST77XX_GREEN, 1, 125);
}

/**
 * @brief Draws text on the screen with the specified alignment, color, size, and y-coordinate.
 * 
 * @param text The text to be drawn.
 * @param allignment The alignment of the text (MIDDLE_ALLIGNMENT, RIGHT_ALLIGNMENT, or LEFT_ALLIGNMENT).
 * @param color The color of the text.
 * @param size The size of the text.
 * @param y The y-coordinate of the text.
 * @return uint8_t Returns STD_TYPES_OK if the text is drawn successfully, otherwise returns STD_TYPES_NOK.
 */
uint8_t drawText(const char *text, uint8_t allignment, uint16_t color, uint8_t size, uint8_t y)
{
  if ((size > MAX_TEXT_SIZE) || (y > LCD_MAX_Y) || (strlen(text) > (LCD_MAX_X/FONT_DEFAULT_SIZE)))
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


/**
 * @brief Updates the GUI with the given temperature, humidity, heater status, and fan status.
 * 
 * This function clears the screen and displays the system status, temperature, humidity, heater status, and fan status on the screen.
 * 
 * @param temperature The temperature value to be displayed.
 * @param humidity The humidity value to be displayed.
 * @param heaterStatus The status of the heater (true for on, false for off).
 * @param fanStatus The status of the fan (true for on, false for off).
 */
void updateGUI(float temprature, float humidity, bool heaterStatus)
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
}


/**
 * Displays a warning screen on the TFT display.
 * 
 * @param warning The warning message to be displayed.
 */
void warningScreen(const char warning[])
{
  tft.fillScreen(ST77XX_WHITE);
  drawText("System", MIDDLE_ALLIGNMENT, ST77XX_RED, 1, 60);
  drawText("Fail", MIDDLE_ALLIGNMENT, ST77XX_RED, 1, 80);
  drawText(warning, MIDDLE_ALLIGNMENT, ST77XX_RED, 1, 100);
}

/**
 * @brief Updates the WIFI status on the GUI.
 * 
 * This function updates the WIFI status on the graphical user interface (GUI) based on the provided status value.
 * It prints the WIFI status to the serial monitor and displays the corresponding message on the TFT display.
 * 
 * @param status The WIFI status to be updated. It can be one of the following values:
 *               - WIFI_CONNECTED: Indicates that the client is connected to the WIFI network.
 *               - WIFI_CONNECTING: Indicates that the client is currently connecting to the WIFI network.
 *               - Any other value: Indicates that the client is disconnected from the WIFI network.
 */
void updateWIFIStatus(uint8_t status)
{
  Serial.println("Updating WIFI status");
  Serial.println(status);
  tft.fillRoundRect(1, 125, 128, 20, 1, ST7735_WHITE);
  if (status == WIFI_CONNECTED)
  {
    drawText("Client Connected", MIDDLE_ALLIGNMENT, ST77XX_GREEN, 1, 130);
  }
  else if (status == WIFI_CONNECTING)
  {
    drawText("Client Connecting", MIDDLE_ALLIGNMENT, ST77XX_ORANGE, 1, 130);
  }
  else
  {
    drawText("Client Disconnected", MIDDLE_ALLIGNMENT, ST77XX_RED, 1, 130);
  }
}