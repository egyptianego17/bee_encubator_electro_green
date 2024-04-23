#ifndef _TEMPHUMIDITYCONTROL_H
#define _TEMPHUMIDITYCONTROL_H

#define PRIMIARY_SENSOR_PIN         GPIO_NUM_0
#define BACKUP_SENSOR_PIN           GPIO_NUM_27
#define HEATER_PIN_IN1              GPIO_NUM_4
#define FAN_PIN_IN2                 GPIO_NUM_19

typedef struct{
  float temperature;
  float humidity;  
  bool sensorMalfunctionFlag; /* 0 -> NO ERROR, 1 -> ERROR */
} DHTSensor;

extern DHTSensor* operatingSensor;

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
uint8_t sensorsUpdateCheck();

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
uint8_t sensorsActuatorsInit();

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
void actuateOnRelays(bool heaterRelayState, bool fanRelayState);

#endif

