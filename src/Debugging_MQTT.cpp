#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "../lib/Debugging_MQTT.h"
#include "../lib/GUI.h"
#include "../lib/STD_TYPES.h"

void debuggingCallback(char* topic, byte* payload, unsigned int length);

static const char *HiveMQ_root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

WiFiClientSecure debuggingEspClient;  
PubSubClient debuggingClient(mqtt_server, 8883, debuggingCallback, debuggingEspClient);

/**
 * @brief Callback function for MQTT message reception.
 * 
 * This function is called when a new MQTT message is received.
 * It prints the received message to the Serial monitor.
 * 
 * @param topic The topic of the received message.
 * @param payload The payload (message content) of the received message.
 * @param length The length of the payload.
 */
  void debuggingCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }

/**
 * @brief Initializes the MQTT client and establishes a connection to the MQTT server.
 * 
 * This function sets the CA certificate, server address, server port, and callback function for the MQTT client.
 * It then attempts to connect to the MQTT server and waits for a maximum of 10 seconds for the connection to be established.
 * 
 * @return true if the MQTT client is successfully connected to the server, false otherwise.
 */
uint8_t debuggingMQTTInit() {
  debuggingEspClient.setCACert(HiveMQ_root_ca);

  debuggingClient.setServer(mqtt_server, mqtt_port);
  debuggingClient.setCallback(debuggingCallback);
 
  Serial.println("Connecting to MQTT...");
  unsigned long startTime = millis();
  while (!debuggingClient.connected() && millis() - startTime < 10000) {
    debuggingReconnectClient();
  }
  return debuggingClient.connected();  
}

/**
 * Attempts to reconnect the MQTT client to the broker.
 * 
 * @return true if the client is successfully reconnected, false otherwise.
 */
uint8_t debuggingReconnectClient() {
  /* Loop until we’re reconnected or timeout occurs */
  unsigned long startTime = millis();
  while (!debuggingClient.connected() && millis() - startTime < 10000) {
    Serial.print("Attempting MQTT connection… ");
    String clientId = "DEVICE0XA2";
    /* Attempt to connect */
    if (debuggingClient.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected!");
      /* Once connected, publish an announcement… */
      debuggingClient.publish("esp32", "Hello World!");
      /* … and resubscribe */
      debuggingClient.subscribe("listen");
    } else {
      Serial.print("failed, rc = ");
      Serial.print(debuggingClient.state());
      Serial.println(" try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(2000);
    }
  }
  return debuggingClient.connected();
}

/**
 * @brief Check the status of the MQTT client.
 * 
 * This function returns the status of the MQTT client, indicating whether it is currently connected or not.
 * 
 * @return true if the client is connected, false otherwise.
 */
bool debuggingGetClientStatus()
{
  return debuggingClient.connected();
}

/**
 * @brief Creates a JSON document with temperature and humidity values and publishes it to an MQTT topic.
 * 
 * @param temperature The temperature value to be included in the JSON document.
 * @param humidity The humidity value to be included in the JSON document.
 */
void debuggingCreateAndUploadJson(float temperature, float humidity, bool heaterState, bool fanState) 
{
  JsonDocument doc;

  /* Add values in the document */
  doc["messageType"] = "data";
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["fanState"] = fanState;
  doc["heaterState"] = heaterState;
  String jsonString;
  serializeJson(doc, jsonString);

  if (debuggingClient.publish("esp32", jsonString.c_str())) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }
}

void debuggingSerialPrint(String string) 
{
  JsonDocument doc;

  /* Add values in the document */
  doc["messageType"] = "serial";
  doc["serialMessage"] = string;
  String jsonString;
  serializeJson(doc, jsonString);

  if (debuggingClient.publish("esp32  ", jsonString.c_str())) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }
}