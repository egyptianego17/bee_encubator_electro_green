#include <DHT.h>
#include "../lib/TempHumidityControl.h"
#include "../lib/STD_TYPES.h"


DHTSensor* primarySensor = new DHTSensor;
DHTSensor* backupSensor = new DHTSensor;
DHTSensor* operatingSensor;

DHT primaryDHT22(PRIMIARY_SENSOR_PIN, DHT22);
DHT backupDHT22(BACKUP_SENSOR_PIN, DHT22);

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
  primarySensor->temperature=primaryDHT22.readTemperature();
  primarySensor->humidity=primaryDHT22.readHumidity();
  backupSensor->temperature=backupDHT22.readTemperature();
  backupSensor->humidity=backupDHT22.readHumidity();

  Serial.println("Primary Sensor Temperature: ");
  Serial.println(primarySensor->temperature);
  Serial.println("Primary Sensor Humidity: ");
  Serial.println(primarySensor->humidity);
  Serial.println("Backup Sensor Temperature: ");
  Serial.println(backupSensor->temperature);
  Serial.println("Backup Sensor Humidity: ");
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

  Serial.println("Primary Sensor Malfunction Flag: ");
  Serial.println(primarySensor->sensorMalfunctionFlag);
  Serial.println("Backup Sensor Malfunction Flag: ");
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
    
  // pinMode(HEATER_PIN_IN1, OUTPUT);
  // pinMode(FAN_PIN_IN2, OUTPUT);

  primaryDHT22.begin();
  backupDHT22.begin();

  primarySensor->temperature=primaryDHT22.readTemperature();
  primarySensor->humidity=primaryDHT22.readHumidity();
  backupSensor->temperature=backupDHT22.readTemperature();
  backupSensor->humidity=backupDHT22.readHumidity();

  primarySensor->sensorMalfunctionFlag = DHT22_OK;
  backupSensor->sensorMalfunctionFlag = DHT22_OK;

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
void actuateOnRelays(bool heaterRelayState, bool fanRelayState)
{
  // digitalWrite(HEATER_PIN_IN1, !heaterRelayState);
  // digitalWrite(FAN_PIN_IN2, !fanRelayState);
}