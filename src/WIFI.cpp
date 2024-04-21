#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "../lib/WIFI.h"
#include "../lib/STD_TYPES.h"

/*
@params: void
@return: uint8_t - returns the status of the WiFi connection
@description: This function initializes the WiFi module
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

/*
@params: void
@return: uint8_t - returns the status of the WiFi connection
@description: This function is called when the client is disconnected from the MQTT server to reconnect again
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