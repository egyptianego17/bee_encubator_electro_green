#ifndef _GUI_H
#define _GUI_H

#define TFT_CS        GPIO_NUM_5 /* Hallowing display control pins: chip select */
#define TFT_RST       GPIO_NUM_17 /* Display reset */
#define TFT_DC        GPIO_NUM_16 /* Display data/command select */

#define WIFI_DISCONNECTED 0
#define WIFI_CONNECTED 1
#define WIFI_CONNECTING 2


/**
 * @brief Initializes the LCD display.
 * 
 * This function initializes the LCD display by configuring the ST7735S chip,
 * setting the display inversion, filling the screen with white color, and
 * drawing some text on the screen.
 */
void LCDInit();

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
uint8_t drawText(const char text[], uint8_t allignment, uint16_t color, uint8_t size, uint8_t y);

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
void updateGUI(float temprature, float humidity, bool heaterStatus);

/**
 * Displays a warning screen on the TFT display.
 * 
 * @param warning The warning message to be displayed.
 */
void warningScreen(const char *warning);

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
void updateWIFIStatus(uint8_t status);

#endif

