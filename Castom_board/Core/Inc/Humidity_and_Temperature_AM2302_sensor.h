/*
 * Humidity_and_Temperature_AM2302_sensor.h
 *
 *  Created on: Sep 17, 2020
 *      Author: odemki
 */

#ifndef INC_HUMIDITY_AND_TEMPERATURE_AM2302_SENSOR_H_
#define INC_HUMIDITY_AND_TEMPERATURE_AM2302_SENSOR_H_

void DelayMicro(__IO uint32_t micros);
void AM2302_init(void);
void AM2302_GetData(uint8_t *data);

#endif /* INC_HUMIDITY_AND_TEMPERATURE_AM2302_SENSOR_H_ */
