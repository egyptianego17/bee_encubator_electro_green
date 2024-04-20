#include <Arduino.h>
#include <esp_task_wdt.h>
#include "../lib/GUI.h"
#include "../lib/TempHumidityControl.h"
#include "../lib/STD_TYPES.h"

const uint8_t targetTempreature = 25;
const uint8_t tempPrecision = 0.5;
bool heaterRelayState = false;
bool fanRelayState = false;

void setup(void) {
  Serial.begin(115200);
  LCDInit();
  if (DHT22Init() == STD_TYPES_NOK)
  {
    esp_task_wdt_init(5, true); 
    esp_task_wdt_add(NULL); //add current thread to WDT watch
    while(sensorsUpdateCheck() != STD_TYPES_OK)
    {
      Serial.println("ERROR");
      warningScreen("Temp Sensors Error");
      delay(500);
    }
    esp_task_wdt_reset();
  }
  delay(5000);
}

void loop() {
  if (sensorsUpdateCheck() == STD_TYPES_NOK)
  {
    esp_task_wdt_init(5, true); 
    esp_task_wdt_add(NULL); //add current thread to WDT watch
    while(sensorsUpdateCheck() != STD_TYPES_OK)
    {
      Serial.println("ERROR");
      warningScreen("Temp Sensors Error");
      delay(500);
    }
    esp_task_wdt_reset();
  }  

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
          
  updateGUI(operatingSensor->temperature, operatingSensor->humidity, heaterRelayState, fanRelayState);
  delay(2000);
}

