#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "../lib/MQTT.h"
#include "../lib/GUI.h"
#include "../lib/STD_TYPES.h"

void callback(char* topic, byte* payload, unsigned int length);

const char* OUTTOPIC = "device1/pub";
const char* INTOPIC = "device1/sub";
const char* THINGNAME = "device1";
const char* awsEndpoint = "ag4n5fourdi53-ats.iot.eu-north-1.amazonaws.com";

static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

static const char *private_pem_key PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA0ALmHkj4hS/pvDd5ityOhP8VStPjMvAlUjViMVi4+7QQAWGR
egEAu2jqvxCva7nlXNPZWXVnX2Y3OgaVVZVuLOc+QlhVfEwlpFRjbiRJHHvBaoEI
EWmUxmZiW9km4VyLDpdTar8lGHobu2oLUsx9Qerb0THQWRAR3EaU4M+7P85cmY+l
EliSiLLG3ZeEhKjn4ZysyQbQ7w8Ale+xY8AKEnBhOWt3hxfFm/msGbBrFg8cyeG3
rORHc1a/c8X5sGzzoDzwY8Hh47DChR4oy0p+jq1srqn95VJQEY58cQxw/Hg3ju+y
a2vOwwCBsFYscFyqYiT3nFHM8Qj7r77dwNFpOwIDAQABAoIBAGRM0YtO4k7OgKUa
FshHe41Ttn+jVg1s0IIobZWf02rDM2iHFG/A5gp4F6FTuVJ3i8pL+KTzQIo2rnCT
VhSzfjG2u0WgdHAAA7KOYndslbhfUC870pka0PstyUXCmsXWKtM5UqbJTmjUEGU5
SAZ/xfIv/hiMLYuUfRg7DHSulfq4dp++XdK8vycpW8Unp0DoPVPV2vOEtxngZXV5
3biBfg7cspRbIWZBA3x5sYft4zMlaHGWSj2xm3TlqcvBf13mvHrH4JiTBRnfyaXL
o9AK9P4Czjg7V4ictSWbUGwYeTFUE4WvLGUYSuynwdgR9ca8S7VaLrbbRhghKDVa
j863uEECgYEA+bRHQrW5NJhMIYC0vbrbxZ6kOcxFWc5O6XOFdz/rvUBIbnJQ2DtJ
GHy/5Pg5UOh8nocwgCzr5jg2lzPwh6umbxtGlLhTpcl+syQtvk5m7JB5931ls5jR
M/cMAeInfkcoWFnECUwzZrH1vEm3qzGYaVv38yDqs3J+c+m3FVdEmqECgYEA1UGD
SDX+BzPKA3Ovz0xDmeFol9ocQ7jkMaAyWlSSQ6GqVq/ji6PUXYJnp2C4/qzAz1wZ
Y5oW0Ge2bOLEWexLGpH4PAYAxLoJl1+qOVE583kBfzqzK699FeVIRdNM0zJp6OYA
eTfwZX+8ogA3RE/VuWqEDeD/Yfyp6TNEKp5bMlsCgYEA8v2jJuCuROc7lpKnz8Xp
neqeB3yntBfWpMj9BSmX3weSNKBb2b//hqvHFSsUo45Jzv7hjKmqOdNUFFm/5FZ+
VLqnOYwSIk/tUqiPNxUWhXimp9dIQ7gfRMFQ1vhb1D+snCVmZmHX10SwTDU0XYya
s1SC+f57RnrRYTz7I/JTWWECgYBwAvwo1qs6Lzqhe4dRs/YiAmKEuDx2WcwKQOGR
zrAQs/6bj8FOHFw0TYWSRdyFcEnJ4M85ILtw9bi9EpMdxF5ic19Wb7TBoBHfn5Dy
kDaHKvzdTgDEJmXmNUsz2duVZvp4KS8AfKBZP7HdyY095U0WGduIyrB5UFQSuCnK
1OiD1wKBgQDndH7M4YTeTD1s1tcb2WSBJvtJaIDAxqa1L/vxK+NbiSAeBEcX49xp
0/ll1DeYtdo255e+/oHDvHbjsbn5HmMsFGVom8q8vJRS8ZXyQuEnU7NrcgNkn0aC
gH2j6aOi+fd8g5YBNktDwR6cKjpaL9U48nwSrY+CdvU26gn7UmnZRg==
-----END RSA PRIVATE KEY-----
)EOF";

static const char *certificate_pem_crt PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVANraKtaLFb2JiZQ+XuidimQdaL0XMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA0MjUxODM2
MzJaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDQAuYeSPiFL+m8N3mK
3I6E/xVK0+My8CVSNWIxWLj7tBABYZF6AQC7aOq/EK9rueVc09lZdWdfZjc6BpVV
lW4s5z5CWFV8TCWkVGNuJEkce8FqgQgRaZTGZmJb2SbhXIsOl1NqvyUYehu7agtS
zH1B6tvRMdBZEBHcRpTgz7s/zlyZj6USWJKIssbdl4SEqOfhnKzJBtDvDwCV77Fj
wAoScGE5a3eHF8Wb+awZsGsWDxzJ4bes5EdzVr9zxfmwbPOgPPBjweHjsMKFHijL
Sn6OrWyuqf3lUlARjnxxDHD8eDeO77Jra87DAIGwVixwXKpiJPecUczxCPuvvt3A
0Wk7AgMBAAGjYDBeMB8GA1UdIwQYMBaAFNzpa5G3nktRA4NnFExZdwdDT/onMB0G
A1UdDgQWBBQ+fcF95B1bXREC5EZOp+oTgaH1nzAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAfhlFKHPHMYzqMWphLzYlyW2h
vdz5yTeDoJdKwEG5M61u50E8SWL7HkGqiiRYXVlrMcsNWdDvNW66ZaMHoA+BmZ38
aF9MaNQkIhnVeBtGnwd5tfFr++I8WrZ5ryZGlHE9OEuLejEyyZlAtFWHKL7MPQ2f
I/G52zLN36qAkpq1nvLnJvnIhUgUDaetDp1tuRRD7x08RIV439619aEJGfm8alky
0vBimU7rUj8FrZ61Rs62UbOdVlo8DApGAiy4M54fVyEdNnAWKRxvc7CdgtMIxQTo
MmYFVyQsEOSE5zTyJPkCFhY2RNn+0zRGJ/jImAr4LwVC/dG1rG96JJEFIoPkpA==
-----END CERTIFICATE-----
)EOF";

WiFiClientSecure espClient;  
PubSubClient client(awsEndpoint, 8883, callback, espClient);

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
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) 
  {
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
uint8_t MQTTInit() 
{
  espClient.setCACert(root_ca);
  espClient.setCertificate(certificate_pem_crt);
  espClient.setPrivateKey(private_pem_key);
 
  Serial.println("Connecting to MQTT...");
  unsigned long startTime = millis();
  while (!client.connected() && millis() - startTime < 10000) 
  {
    reconnectClient();
  }
  return client.connected();  
}

/**
 * Attempts to reconnect the MQTT client to the broker.
 * 
 * @return true if the client is successfully reconnected, false otherwise.
 */
uint8_t reconnectClient() 
{
  /* Loop until we’re reconnected or timeout occurs */
  unsigned long startTime = millis();
  while (!client.connected() && millis() - startTime < 10000) 
  {
    Serial.print("Attempting MQTT connection… ");
    /* Attempt to connect */
    if (client.connect(THINGNAME)) {
      Serial.println("connected!");
      /* Once connected, publish an announcement… */
      client.publish(OUTTOPIC, "Hello World!");
      /* … and resubscribe */
      client.subscribe(INTOPIC);
    } 
    else 
    {
      Serial.print("failed, rc = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(2000);
    }
  }
  return client.connected();
}

/**
 * @brief Check the status of the MQTT client.
 * 
 * This function returns the status of the MQTT client, indicating whether it is currently connected or not.
 * 
 * @return true if the client is connected, false otherwise.
 */
bool getClientStatus()
{
  return client.connected();
}

/**
 * @brief Creates a JSON document with temperature and humidity values and publishes it to an MQTT topic.
 * 
 * @param temperature The temperature value to be included in the JSON document.
 * @param humidity The humidity value to be included in the JSON document.
 */
void createAndUploadJson(float temperature, float humidity) 
{
  JsonDocument doc;

  /* Add values in the document */
  doc["deviceID"] = "ESP32_7KAX";
  doc["tempreature"] = temperature;
  doc["humidity"] = humidity;

  String jsonString;
  serializeJson(doc, jsonString);

  if (client.publish(OUTTOPIC, jsonString.c_str())) 
  {
    Serial.println("Success sending message");
  } 
  else 
  {
    Serial.println("Error sending message");
  }
}