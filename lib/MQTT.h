#ifndef _MQTT_H
#define _MQTT_H

/* HiveMQ Cloud Broker settings */
#define mqtt_server "98f5567ae14b46818d9c1f8eba4135a3.s1.eu.hivemq.cloud"
#define mqtt_username "xaviA"
#define mqtt_password "xavi1234A"
#define mqtt_port 8883

void createAndUploadJson(float temperature, float humidity);
void MQTTConnect();
void reconnectClient();
bool getClientStatus();

#endif