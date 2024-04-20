#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "../lib/WIFI.h"
#include "../lib/STD_TYPES.h"

/*
@params: void
@return: void
@description: This function initializes the WiFi module
*/
void WIFIInit()
{
  delay(10);
  /* We start by connecting to a WiFi network */

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

/*
@params: void
@return: void
@description: This function is called when the client is disconnected from the MQTT server to reconnect again
*/
void reconnectWiFi() {
    /* Attempt to reconnect to WiFi */
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
}