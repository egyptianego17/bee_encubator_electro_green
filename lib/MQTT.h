#ifndef _MQTT_H
#define _MQTT_H

/* HiveMQ Cloud Broker settings */
#define mqtt_server "98f5567ae14b46818d9c1f8eba4135a3.s1.eu.hivemq.cloud"
#define mqtt_username "xaviA"
#define mqtt_password "xavi1234A"
#define mqtt_port 8883
#define MQTT_CLIENT_CONNECTED 1
#define MQTT_CLIENT_DISCONNECTED 0
void createAndUploadJson(float temperature, float humidity);
uint8_t MQTTInit();
uint8_t reconnectClient();
bool getClientStatus();

#endif