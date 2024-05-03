#include <Arduino.h>
#include <esp_task_wdt.h>
#include "../lib/GUI.h"
#include "../lib/TempHumidityControl.h"
#include "../lib/WIFI.h"
#include "../lib/MQTT.h"
#include "../lib/Debugging_MQTT.h"
#include "../lib/STD_TYPES.h"

const uint8_t targetTempreature = 25;
const uint8_t tempPrecision = 0.5;
bool heaterRelayState = false;
bool fanRelayState = false;

void controlTask(void* parameter);
void iotTask(void* parameter);

void setup(void) {
  Serial.begin(115200);

  LCDInit();
  if (sensorsActuatorsInit() == STD_TYPES_NOK)
  {
    esp_task_wdt_init(5, true); 
    esp_task_wdt_add(NULL); /* add current thread to WDT watch */
    while(sensorsActuatorsInit() != STD_TYPES_OK)
    {
      Serial.println("ERROR");
      warningScreen("Sensors Error");
      delay(500);
    }
    esp_task_wdt_reset();
  }
  WIFIInit();
  debuggingMQTTInit();
  delay(2500);
  xTaskCreate(controlTask, "Tezk", 8000, NULL, 1, NULL);
  xTaskCreate(iotTask, "Tezk 2", 8000, NULL, 2, NULL);

}

void loop() {
}

void controlTask(void* parameter)
{
  while(1)
  {
    /* Update and check sensors readings */
    if (sensorsUpdateCheck() == STD_TYPES_NOK)
    {
      esp_task_wdt_init(5, true); 
      esp_task_wdt_add(NULL); /* add current thread to WDT watch */
      while(sensorsUpdateCheck() != STD_TYPES_OK)
      {
        Serial.println("ERROR");
        warningScreen("Sensors Error");
        actuateOnRelays(false, false);
        vTaskDelay(pdMS_TO_TICKS(500));
      }
      esp_task_wdt_reset();
    }  

    /* Update heater-fan flags */
    if (operatingSensor->temperature < targetTempreature - tempPrecision)
    {
      heaterRelayState = true;
      fanRelayState = false;
    }
    else if (operatingSensor->temperature > targetTempreature + tempPrecision)
    {
      heaterRelayState = false;
      fanRelayState = true;
    }
    else
    {
      heaterRelayState = false;
      fanRelayState = false;
    }
    /* Actuate on the Fan and Heater */
    Serial.println("Heater state: ");
    Serial.println(heaterRelayState);
    Serial.println("Fan state: ");
    Serial.println(fanRelayState);
    actuateOnRelays(heaterRelayState, fanRelayState);

    /* Update the GUI */
    updateGUI(operatingSensor->temperature, operatingSensor->humidity, heaterRelayState, fanRelayState);

    bool clientStatus = debuggingGetClientStatus();

    if(clientStatus != MQTT_CLIENT_CONNECTED)
    {
      updateWIFIStatus(WIFI_DISCONNECTED);
      Serial.println("Reconnecting to MQTT...");
      if (debuggingReconnectClient() == WIFI_STATUS_CONNECTED)
      {
          updateWIFIStatus(WIFI_CONNECTING);
        if (debuggingReconnectClient() == MQTT_CLIENT_CONNECTED)
        {
          updateWIFIStatus(WIFI_CONNECTED);
        }
        else
        {
          updateWIFIStatus(WIFI_DISCONNECTED);
        }
      }
      else
      {
        updateWIFIStatus(WIFI_DISCONNECTED);
      }
    }
    else
    {
      updateWIFIStatus(WIFI_CONNECTED);
    }
    vTaskDelay(pdMS_TO_TICKS(4000));
  }
}

void iotTask(void* parameter)
{
  while(1)
  {
    /* Check the MQTT Client */
    bool clientStatus = debuggingGetClientStatus();
    Serial.println("Server status: ");
    Serial.println(clientStatus);
    if(clientStatus != MQTT_CLIENT_CONNECTED)
    {
      /* Do nothing */
    }
    else
    {
      debuggingCreateAndUploadJson(operatingSensor->temperature, operatingSensor->humidity, heaterRelayState, fanRelayState);
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}