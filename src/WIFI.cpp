#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "../lib/WIFI.h"
#include "../lib/STD_TYPES.h"

/**
 * @brief Initializes the WiFi connection.
 * 
 * This function connects to a WiFi network using the provided SSID and password.
 * It waits for a maximum of 10 seconds for the connection to be established.
 * 
 * @return The status of the WiFi connection.
 *         Possible return values:
 *         - WL_CONNECTED: WiFi connection successful
 *         - WL_NO_SSID_AVAIL: WiFi network not found
 *         - WL_CONNECT_FAILED: WiFi connection failed
 *         - WL_IDLE_STATUS: WiFi module is not configured in station mode
 *         - WL_DISCONNECTED: WiFi module is not connected to any network
 */
uint8_t WIFIInit()
{
  delay(10);
  /* We start by connecting to a WiFi network */

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
      delay(500);
      Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return WiFi.status();
}

/**
 * @brief Attempts to reconnect to WiFi.
 * 
 * This function attempts to reconnect to WiFi by calling WiFi.begin() with the provided SSID and password.
 * It waits for the connection to be established and returns the status of the WiFi connection.
 * 
 * @return The status of the WiFi connection (WL_CONNECTED if successfully connected, otherwise not connected).
 */
uint8_t reconnectWiFi() {
    /* Attempt to reconnect to WiFi */
    Serial.println("Reconnecting to WiFi...");
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected, trying to reconnect...");

        /* Begin connection attempt */
        WiFi.begin(ssid, password);

        /* Wait for the connection to be established */
        int attempt = 0;
        while (WiFi.status() != WL_CONNECTED && attempt < 10) {
            delay(500);
            Serial.print(".");
            attempt++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            /* WiFi is successfully connected */
            Serial.println("\nConnected to WiFi");
        } else {
            /* Failed to connect to WiFi */
            Serial.println("\nFailed to connect to WiFi");
        }
    }
  return WiFi.status();
}