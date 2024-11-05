#include <DHT.h>
#include "../lib/TempHumidityControl.h"
#include "../lib/STD_TYPES.h"
#include <Wire.h>
#include "Adafruit_SHT31.h"

#if SENSOR_TYPE == DHT22
DHTSensor* primarySensor = new DHTSensor;
DHTSensor* backupSensor = new DHTSensor;
DHTSensor* operatingSensor;

DHT primaryDHT22(PRIMIARY_SENSOR_PIN, DHT22);
DHT backupDHT22(BACKUP_SENSOR_PIN, DHT22);

#elif SENSOR_TYPE == SHT31

DHTSensor* operatingSensor = new DHTSensor;

bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

#endif

uint8_t currentHeater = HEATER_PIN_1;
/**
 * @brief Updates the temperature and humidity readings from the primary and backup sensors.
 * 
 * This function reads the temperature and humidity values from the primary and backup sensors,
 * and stores them in the respective sensor objects. It also checks for sensor malfunctions
 * and updates the malfunction flags accordingly. Finally, it determines the operating sensor
 * based on the sensor malfunction flags.
 * 
 * @return STD_TYPES_OK if the sensors were updated successfully and there are no malfunctions,
 *         STD_TYPES_NOK if both sensors have malfunctioned.
 */
uint8_t sensorsUpdateCheck()
{
  #if SENSOR_TYPE == DHT22

  primarySensor->temperature=primaryDHT22.readTemperature();
  primarySensor->humidity=primaryDHT22.readHumidity();
  backupSensor->temperature=backupDHT22.readTemperature();
  backupSensor->humidity=backupDHT22.readHumidity();

  Serial.print("Primary Sensor Temperature: ");
  Serial.println(primarySensor->temperature);
  Serial.print("Primary Sensor Humidity: ");
  Serial.println(primarySensor->humidity);
  Serial.print("Backup Sensor Temperature: ");
  Serial.println(backupSensor->temperature);
  Serial.print("Backup Sensor Humidity: ");
  Serial.println(backupSensor->humidity);

  if(isnan(primarySensor->temperature) || isnan(primarySensor->humidity))
  {
    primarySensor->sensorMalfunctionFlag = DHT22_ERROR;
  }
  else
  {
    primarySensor->sensorMalfunctionFlag = DHT22_OK;
  }

  if(isnan(backupSensor->temperature) || isnan(backupSensor->humidity))
  {
    backupSensor->sensorMalfunctionFlag = DHT22_ERROR;
  }
  else
  {
    backupSensor->sensorMalfunctionFlag = DHT22_OK;
  }

  Serial.print("Primary Sensor Malfunction Flag: ");
  Serial.println(primarySensor->sensorMalfunctionFlag);
  Serial.print("Backup Sensor Malfunction Flag: ");
  Serial.println(backupSensor->sensorMalfunctionFlag);

  if (primarySensor->sensorMalfunctionFlag == DHT22_ERROR && backupSensor->sensorMalfunctionFlag == DHT22_ERROR)
  {
    return STD_TYPES_NOK;
  }
  else if (primarySensor->sensorMalfunctionFlag == DHT22_OK)
  {
    operatingSensor = primarySensor;
  }
  else
  {
    operatingSensor = backupSensor;
  }
  return STD_TYPES_OK;
  #elif SENSOR_TYPE == SHT31
  operatingSensor->temperature=sht31.readTemperature();
  operatingSensor->humidity=sht31.readHumidity();

  Serial.print("Operating Sensor Temperature: ");
  Serial.println(operatingSensor->temperature);
  Serial.print("Operating Sensor Humidity: ");
  Serial.println(operatingSensor->humidity);
  
  if(isnan(operatingSensor->temperature) || isnan(operatingSensor->humidity))
  {
    operatingSensor->sensorMalfunctionFlag = DHT22_ERROR;
  }
  else
  {
    operatingSensor->sensorMalfunctionFlag = DHT22_OK;
  }
  if (operatingSensor->sensorMalfunctionFlag == DHT22_ERROR)
  {
    return STD_TYPES_NOK;
  }

  return STD_TYPES_OK;

  #endif
}

/**
 * @brief Initializes the sensors and actuators.
 * 
 * This function initializes the primary and backup DHT22 sensors and reads the temperature and humidity values from them.
 * It also checks for any sensor malfunctions and sets the appropriate flags.
 * If both sensors have malfunctioned, it returns STD_TYPES_NOK.
 * If the primary sensor is functioning properly, it sets the operatingSensor pointer to point to the primary sensor.
 * If the backup sensor is functioning properly, it sets the operatingSensor pointer to point to the backup sensor.
 * 
 * @return Returns STD_TYPES_OK if the initialization is successful, otherwise returns STD_TYPES_NOK.
 */
uint8_t sensorsActuatorsInit()
{
  pinMode(FAN_PIN_1, OUTPUT);
  pinMode(FAN_PIN_2, OUTPUT);
  pinMode(HEATER_PIN_1, OUTPUT);
  pinMode(HEATER_PIN_2, OUTPUT);
  pinMode(HUMIDIFIER_PIN, OUTPUT);

  digitalWrite(FAN_PIN_1, HIGH);
  digitalWrite(FAN_PIN_2, LOW);
  digitalWrite(HEATER_PIN_1, LOW);
  digitalWrite(HEATER_PIN_2, LOW);
  digitalWrite(HUMIDIFIER_PIN, LOW);

  #if SENSOR_TYPE == DHT22

  primaryDHT22.begin();
  backupDHT22.begin();

  primarySensor->temperature=primaryDHT22.readTemperature();
  primarySensor->humidity=primaryDHT22.readHumidity();
  backupSensor->temperature=backupDHT22.readTemperature();
  backupSensor->humidity=backupDHT22.readHumidity();
  
  primarySensor->sensorMalfunctionFlag = DHT22_OK;
  backupSensor->sensorMalfunctionFlag = DHT22_OK;

  primarySensor->sensorID = 1;
  backupSensor->sensorID = 2;

  if(isnan(primarySensor->temperature) || isnan(primarySensor->humidity))
  {
    primarySensor->sensorMalfunctionFlag = DHT22_ERROR;
  }

  if(isnan(backupSensor->temperature) || isnan(backupSensor->humidity))
  {
    backupSensor->sensorMalfunctionFlag = DHT22_ERROR;
  }

  if (primarySensor->sensorMalfunctionFlag == DHT22_ERROR && backupSensor->sensorMalfunctionFlag == DHT22_ERROR)
  {
    return STD_TYPES_NOK;
  }
  else if (primarySensor->sensorMalfunctionFlag == DHT22_OK)
  {
    operatingSensor = primarySensor;
  }
  else
  {
    operatingSensor = backupSensor;
  }
  return STD_TYPES_OK;

  #elif SENSOR_TYPE == SHT31

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) 
  {   
      Serial.println("Couldn't find SHT31");
      while (1) 
        delay(1);
      // Set to 0x45 for alternate i2c addr
  }

  operatingSensor->temperature=sht31.readTemperature();
  operatingSensor->humidity=sht31.readHumidity();

  Serial.print("Operating Sensor Temperature: ");
  Serial.println(operatingSensor->temperature);
  Serial.print("Operating Sensor Humidity: ");
  Serial.println(operatingSensor->humidity);
  
  if(isnan(operatingSensor->temperature) || isnan(operatingSensor->humidity))
  {
    operatingSensor->sensorMalfunctionFlag = DHT22_ERROR;
  }
  else
  {
    operatingSensor->sensorMalfunctionFlag = DHT22_OK;
  }
  if (operatingSensor->sensorMalfunctionFlag == DHT22_ERROR)
  {
    return STD_TYPES_NOK;
  }

  return STD_TYPES_OK;

  #endif

}

/**
 * @brief Actuates the relays based on the provided states.
 * 
 * This function is responsible for controlling the relays that control the heater and fan.
 * The `heaterRelayState` parameter determines whether the heater relay should be turned on or off.
 * The `fanRelayState` parameter determines whether the fan relay should be turned on or off.
 * 
 * @param heaterRelayState The state of the heater relay. Set to `true` to turn on the heater, or `false` to turn it off.
 * @param fanRelayState The state of the fan relay. Set to `true` to turn on the fan, or `false` to turn it off.
 */
void actuateOnRelays(bool heaterRelayState, bool isStateChanged)
{
  if (heaterRelayState)
  {
    switch(currentHeater)
    {
      case HEATER_PIN_1:
        digitalWrite(HEATER_PIN_1, HIGH);
        digitalWrite(HEATER_PIN_2, LOW);
        if (isStateChanged == true)
        {
          currentHeater = HEATER_PIN_2;
        }
        break;
      case HEATER_PIN_2:
        digitalWrite(HEATER_PIN_2, HIGH);
        digitalWrite(HEATER_PIN_1, LOW);
        if (isStateChanged == true)
        {
          currentHeater = HEATER_PIN_1;
        }
        break;
      default:
        digitalWrite(HEATER_PIN_1, HIGH);
        digitalWrite(HEATER_PIN_2, LOW);
        if (isStateChanged == true)
        {
          currentHeater = HEATER_PIN_1;
        }
        break;
    }
  }
  else
  {
    digitalWrite(HEATER_PIN_1, LOW);
    digitalWrite(HEATER_PIN_2, LOW);
  }
  Serial.print("Current heater: ");
  Serial.println(currentHeater);
}

/**
 * @brief Adds a water drop to the reservoir.
 * 
 * This function turns on the water pump for a specified duration to add a water drop to the reservoir.
 * 
 * @param duration The duration for which the water pump should be turned on.
*/
void addWaterDrop(uint32_t duration)
{
  digitalWrite(HUMIDIFIER_PIN, HIGH);
  delay(duration);
  digitalWrite(HUMIDIFIER_PIN, LOW);
  Serial.println("Water Drop Added");
}

void turnOnHumidifier()
{
  digitalWrite(HUMIDIFIER_PIN, HIGH);
}

void turnOffHumidifier()
{
  digitalWrite(HUMIDIFIER_PIN, LOW);
}

/**
 * @brief Switches the state of the fans.
 * 
 * This function toggles the state of the fans to decrease the load on the fans.
 */
void switchFans()
{
  digitalWrite(FAN_PIN_1, !digitalRead(FAN_PIN_1));
  digitalWrite(FAN_PIN_2, !digitalRead(FAN_PIN_2));
}

