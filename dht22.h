/*
 * @file: dht22.h
 * @author: Fuad Mammadzade, Huseyn Gambarov
 * @brief: Header file for library
 * 		This file contains:
 * 		* Function prototypes for measuring and getting data
 * 		* Data structure for setting Pin and Port of DHT22
 *
 * */

#ifndef DHT_H_
#define DHT_H_

typedef struct
{
	uint32_t Pin;
	GPIO_TypeDef * GPIO_Port;

}DHT22_InitTypeDef;

uint8_t __dht__received__binary__[40];

void DHT22_Measure(DHT22_InitTypeDef * Configuration);

float DHT22_GetTemperature();

float DHT22_GetHumidity();

#endif /* DHT_H_ */
