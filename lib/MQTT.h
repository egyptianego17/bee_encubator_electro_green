#ifndef _MQTT_H
#define _MQTT_H

/* HiveMQ Cloud Broker settings */
const char* mqtt_server = "98f5567ae14b46818d9c1f8eba4135a3.s1.eu.hivemq.cloud"; // replace with your HiveMQ Cluster URL
const char* mqtt_username = "xaviA"; // replace with your Username
const char* mqtt_password = "xavi1234A"; // replace with your Password
const int mqtt_port = 8883;


void createAndUploadJson(float temperature, float humidity);

#endif