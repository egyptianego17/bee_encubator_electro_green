#ifndef _MQTT_H
#define _MQTT_H

/* HiveMQ Cloud Broker settings */
#define mqtt_server "ag4n5fourdi53-ats.iot.eu-north-1.amazonaws.com"
#define mqtt_username ""
#define mqtt_password ""
#define mqtt_port 8883
// const char* OUTTOPIC = "device1/sub";
// const char* INTOPIC = "device1/pub";
// const char* THINGNAME = "Device1";
// const char* awsEndpoint = "ag4n5fourdi53-ats.iot.eu-north-1.amazonaws.com";
#define MQTT_CLIENT_CONNECTED 1
#define MQTT_CLIENT_DISCONNECTED 0

/**
 * @brief Creates a JSON document with temperature and humidity values and publishes it to an MQTT topic.
 * 
 * @param temperature The temperature value to be included in the JSON document.
 * @param humidity The humidity value to be included in the JSON document.
 */
void createAndUploadJson(float temperature, float humidity);

/**
 * @brief Initializes the MQTT client and establishes a connection to the MQTT server.
 * 
 * This function sets the CA certificate, server address, server port, and callback function for the MQTT client.
 * It then attempts to connect to the MQTT server and waits for a maximum of 10 seconds for the connection to be established.
 * 
 * @return true if the MQTT client is successfully connected to the server, false otherwise.
 */
uint8_t MQTTInit();

/**
 * Attempts to reconnect the MQTT client to the broker.
 * 
 * @return true if the client is successfully reconnected, false otherwise.
 */
uint8_t reconnectClient();

/**
 * @brief Check the status of the MQTT client.
 * 
 * This function returns the status of the MQTT client, indicating whether it is currently connected or not.
 * 
 * @return true if the client is connected, false otherwise.
 */
bool getClientStatus();

#endif