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

uint8_t sensorsUpdateCheck();
uint8_t sensorsActuatorsInit();
void actuateOnRelays(bool heaterRelayState, bool fanRelayState);

#endif

