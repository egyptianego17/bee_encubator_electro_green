#ifndef _WIFI_H
#define _WIFI_H

/* WiFi settings */
#define ssid "WE_DFD258"
#define password "23735162274"

#define WIFI_STATUS_CONNECTED 3
#define WIFI_STATUS_DISCONNECTED 6

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
uint8_t WIFIInit();

/**
 * @brief Attempts to reconnect to WiFi.
 * 
 * This function attempts to reconnect to WiFi by calling WiFi.begin() with the provided SSID and password.
 * It waits for the connection to be established and returns the status of the WiFi connection.
 * 
 * @return The status of the WiFi connection (WL_CONNECTED if successfully connected, otherwise not connected).
 */
uint8_t reconnectWiFi();

#endif