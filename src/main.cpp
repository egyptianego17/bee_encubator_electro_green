#include <Arduino.h>
#include <esp_task_wdt.h>
#include "../lib/GUI.h"
#include "../lib/TempHumidityControl.h"
#include "../lib/WIFI.h"
#include "../lib/MQTT.h"
#include "../lib/OTA.h"
#include "../lib/STD_TYPES.h"

const uint8_t targetTempreature = 25;
const uint8_t tempPrecision = 0.5;
bool heaterRelayState = false;
bool fanRelayState = false;

void setup(void) {
  Serial.begin(115200);

  // Timer3_Cfg = timerBegin(3, 80, true);
  // timerAttachInterrupt(Timer3_Cfg, &Timer3_ISR, true);
  // timerAlarmWrite(Timer3_Cfg, 1000, true);
  // timerAlarmEnable(Timer3_Cfg);

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
  MQTTInit();
  OTAUpdate();
  delay(5000);
}

void loop() {
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
      delay(500);
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

  /* Check the MQTT Client */
  bool clientStatus = getClientStatus();
  Serial.println("Server status: ");
  Serial.println(clientStatus);
  if(clientStatus != MQTT_CLIENT_CONNECTED)
  {
    updateWIFIStatus(WIFI_DISCONNECTED);
    Serial.println("Reconnecting to MQTT...");
    if (reconnectWiFi() == WIFI_STATUS_CONNECTED)
    {
        updateWIFIStatus(WIFI_CONNECTING);
      if (reconnectClient() == MQTT_CLIENT_CONNECTED)
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
    createAndUploadJson(operatingSensor->temperature, operatingSensor->humidity);
    updateWIFIStatus(WIFI_CONNECTED);
  }
  delay(3000);
}