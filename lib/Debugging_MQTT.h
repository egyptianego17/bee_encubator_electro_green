#ifndef _DEBUGGING_MQTT_H
#define _DEBUGGING_MQTT_H

/* Debugging HiveMQ Cloud Broker settings */
#define mqtt_server "0dc88b2a3f78492bb869b40cd2223080.s1.eu.hivemq.cloud"
#define mqtt_username "xaviA"
#define mqtt_password "12345678Aa"
#define mqtt_port 8883
#define MQTT_CLIENT_CONNECTED 1
#define MQTT_CLIENT_DISCONNECTED 0
#define DEVICE_ID "DEVICE0XA2"
#define VERSION "1.0.3"

/**
 * @brief Creates a JSON document with temperature and humidity values and publishes it to an MQTT topic.
 * 
 * @param temperature The temperature value to be included in the JSON document.
 * @param humidity The humidity value to be included in the JSON document.
 */
void debuggingCreateAndUploadJson(float temperature, float humidity, bool heaterState);

/**
 * @brief Initializes the MQTT client and establishes a connection to the MQTT server.
 * 
 * This function sets the CA certificate, server address, server port, and callback function for the MQTT client.
 * It then attempts to connect to the MQTT server and waits for a maximum of 10 seconds for the connection to be established.
 * 
 * @return true if the MQTT client is successfully connected to the server, false otherwise.
 */
uint8_t debuggingMQTTInit();

/**
 * Attempts to reconnect the MQTT client to the broker.
 * 
 * @return true if the client is successfully reconnected, false otherwise.
 */
uint8_t debuggingReconnectClient();

/**
 * @brief Check the status of the MQTT client.
 * 
 * This function returns the status of the MQTT client, indicating whether it is currently connected or not.
 * 
 * @return true if the client is connected, false otherwise.
 */
bool debuggingGetClientStatus();

void debuggingSerialPrint(const String &string);

/**
 * @brief Sends an integer value to the MQTT broker.
 * 
 * @param value The integer value to be sent.
 */
void debuggingSerialPrint(int value);

/**
 * @brief Sends a float value to the MQTT broker.
 * 
 * @param value The float value to be sent.
*/
void debuggingSerialPrint(float value);

#endif