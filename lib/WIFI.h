#ifndef _WIFI_H
#define _WIFI_H

/* WiFi settings */
#define ssid "zucse2022"
#define password "zucse2024"

#define WIFI_STATUS_CONNECTED 3
#define WIFI_STATUS_DISCONNECTED 6

uint8_t WIFIInit();
uint8_t reconnectWiFi();

#endif