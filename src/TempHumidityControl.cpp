#include <DHT.h>
#include "../lib/TempHumidityControl.h"
#include "../lib/STD_TYPES.h"


DHTSensor* primarySensor = new DHTSensor;
DHTSensor* backupSensor = new DHTSensor;
DHTSensor* operatingSensor;

DHT primaryDHT22(PRIMIARY_SENSOR_PIN, DHT22);
DHT backupDHT22(BACKUP_SENSOR_PIN, DHT22);

/*
@params: void
@return: uint8_t
@desc: This function updates the sensor values and checks for any malfunctions.
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

/*
@param void
@return uint8_t
@desc: This function initializes the DHT22 sensors and checks for any malfunctions.
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

void actuateOnRelays(bool heaterRelayState, bool fanRelayState)
{
  // digitalWrite(HEATER_PIN_IN1, !heaterRelayState);
  // digitalWrite(FAN_PIN_IN2, !fanRelayState);
}