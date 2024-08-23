#include <Arduino.h>
#include <esp_task_wdt.h>
#include "../lib/GUI.h"
#include "../lib/TempHumidityControl.h"
#include "../lib/WIFI.h"
#include "../lib/MQTT.h"
#include "../lib/Debugging_MQTT.h"
#include "../lib/OTA.h"
#include "../lib/STD_TYPES.h"

const uint8_t targetTempreature = 36.5;
const uint8_t targetHumidity = 65;
const uint8_t humidityPrecision = 5;
const uint8_t tempPrecision = 1;
uint32_t waterLastDrop = 0;
bool heaterRelayState = false;
uint8_t wifiStatus = WIFI_CONNECTING;

void iotTask(void* parameter);
void controlFans(void* parameter);
void controlHeaters(void* parameter);
void pumpControl(void* parameter);
void GUITask(void* parameter);

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
  sensorsActuatorsInit();
  delay(2500);
  xTaskCreate(controlFans, "Fans Control Task", 8000, NULL, 1, NULL);
  xTaskCreate(controlHeaters, "Heaters Control Task", 8000, NULL, 2, NULL);
  xTaskCreate(pumpControl, "Pump Control Task", 8000, NULL, 3, NULL);
  xTaskCreate(GUITask, "GUI Task", 8000, NULL, 4, NULL);
  xTaskCreate(iotTask, "WiFi and MQTT Task", 8000, NULL, 5, NULL);
}

void loop() 
{
}

void controlFans(void* parameter)
{
  while(1)
  {
    switchFans();
    String message = "Fan 1: " + String(digitalRead(FAN_PIN_1)) + " Fan 2: " + String(digitalRead(FAN_PIN_2));
    Serial.println(message);
    vTaskDelay(pdMS_TO_TICKS(FANS_SWITCH_INTERVAL));
  }
}

void controlHeaters(void* parameter)
{
  bool isStateChanged = false;
  uint32_t heaterWorkTime = millis();;
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
    if ((operatingSensor->temperature < targetTempreature - tempPrecision) && (heaterRelayState == false))
    {
      isStateChanged = true;
      heaterRelayState = true;
    }
    else if((operatingSensor->temperature < targetTempreature - tempPrecision) && (heaterRelayState == true) && (millis()- heaterWorkTime < 1000*60))
    {
      isStateChanged = false;
      heaterRelayState = true;
    }
    else if((operatingSensor->temperature < targetTempreature - tempPrecision) && (heaterRelayState == true) && (millis()-heaterWorkTime > 1000*60))
    {
      heaterWorkTime = millis();
      isStateChanged = true;
      heaterRelayState = true;
    }
    else if (operatingSensor->temperature > targetTempreature + tempPrecision)
    {
      heaterRelayState = false;
    }
  
    /* Actuate on the Fan and Heater */
    String message = "Heater: " + String(heaterRelayState);
    Serial.println(message);
    actuateOnRelays(heaterRelayState, isStateChanged);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void pumpControl(void* parameter){
  waterLastDrop = millis();
  while(1)
  {
    if ((operatingSensor->humidity < targetHumidity - humidityPrecision) && (millis() - waterLastDrop > WATER_DROP_INTERVAL))
    {
      addWaterDrop(PUMP_DELAY_TIME);
      waterLastDrop = millis();
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void GUITask(void* parameter)
{
  while(1)
  {
    /* Update the GUI */
    updateGUI(operatingSensor->temperature, operatingSensor->humidity, heaterRelayState);
    updateWIFIStatus(wifiStatus);
    vTaskDelay(pdMS_TO_TICKS(4000));
  }
}

void iotTask(void* parameter)
{
  if (WIFIInit() == WIFI_STATUS_CONNECTED && debuggingMQTTInit() == MQTT_CLIENT_CONNECTED)
  {
    wifiStatus = WIFI_CONNECTED;
    String latestfirmwareVersion = getFirmwareVersion();

    /* Get the latest stable firmware version and print it */
    if (latestfirmwareVersion.length() > 0) {
        Serial.print("Latest stable firmware version: ");
        Serial.println(latestfirmwareVersion);
        if (latestfirmwareVersion == VERSION)
        {
          Serial.println("Firmware is up to date.");
        }
        else
        {
          Serial.println("Firmware is outdated, start updating...");
          OTAUpdate(latestfirmwareVersion);
        }
    } else {
        Serial.println("Failed to get firmware version.");
    }
  }
  else
  {
    wifiStatus = WIFI_DISCONNECTED;
  }
  while(1)
  {
    /* Check the MQTT Client */
    bool clientStatus = debuggingGetClientStatus();
    if(clientStatus != MQTT_CLIENT_CONNECTED)
    {
      wifiStatus = WIFI_DISCONNECTED;
      Serial.println("Reconnecting to MQTT...");
      if (reconnectWiFi() == WIFI_STATUS_CONNECTED)
      {
        wifiStatus = WIFI_CONNECTING;
        if (debuggingMQTTInit() == MQTT_CLIENT_CONNECTED && debuggingReconnectClient() == MQTT_CLIENT_CONNECTED)
        {
          wifiStatus = WIFI_CONNECTED;
        }
        else
        {
          wifiStatus = WIFI_DISCONNECTED;
        }
      }
      else
      {
        wifiStatus = WIFI_DISCONNECTED;
      }
    }
    else
    {
      String message = "Heater: " + String(heaterRelayState);
      debuggingSerialPrint(message);
      message = "Operating Sensor ID: " + String(operatingSensor->sensorID);
      debuggingSerialPrint(message);
      message = "Time since last water drop: " + (String((millis() - waterLastDrop)/1000));
      debuggingSerialPrint(message);
      message = "Up time: " + (String(millis()/1000));
      debuggingSerialPrint(message);
      debuggingCreateAndUploadJson(operatingSensor->temperature, operatingSensor->humidity, heaterRelayState);
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}