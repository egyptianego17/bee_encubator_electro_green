#ifndef _TEMPHUMIDITYCONTROL_H
#define _TEMPHUMIDITYCONTROL_H

#define PRIMIARY_SENSOR_PIN GPIO_NUM_0
#define BACKUP_SENSOR_PIN GPIO_NUM_27

typedef struct{
  float temperature;
  float humidity;  
  bool sensorMalfunctionFlag; /* 0 -> NO ERROR, 1 -> ERROR */
} DHTSensor;

extern DHTSensor* operatingSensor;

uint8_t sensorsUpdateCheck();
uint8_t DHT22Init();

#endif

