/*
 * @file: dht22.c
 * @author: Fuad Mammadzade, Huseyn Gambarov
 * @brief: Source file for library
 * 		This file contains:
 * 		* Definitions of functions for measuring and getting data
 *
 * */

#include "micros.h"
#include "dht22.h"
#include <math.h>

void  DHT22_Measure(DHT22_InitTypeDef * Configuration)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	uint32_t DHT22_Pin = Configuration -> Pin;
	GPIO_TypeDef * DHT22_GPIO_Port = Configuration -> GPIO_Port;
	DWT_Init();

	// ===starting communication===
	// sending start signal

	// configure DHT22_Pin for Output
	GPIO_InitStruct.Pin = DHT22_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);


	HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET);
	delay_micros(1000);

	HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET);
	delay_micros(30);

	// after start signal sensor will send response signal and data
	// thus the Pin connected to the Sensor must be set input

	// configure DHT22_Pin for Input
	GPIO_InitStruct.Pin = DHT22_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);

	// ===checking response===
	uint8_t check = 0;

	// after 40us pin must be low
	// otherwise there is a problem
	delay_micros(40);

	if (!HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)) {
		delay_micros(80);
		if (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)) {
			check = 1;
		}

	}

	// waiting till the sensor finishes sending response signal
	while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin));



	// if sensor responses correctly then data transmission will begin
	if (check == 1){
		// ===beginning data transmission===

		for (int i = 0; i < 40; i++){
			// waiting until pin goes high
			while (!HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin));

			// if the pin is low after 40us then the bit is 0
			delay_micros(40);

			if (!HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin))
				__dht__received__binary__[i] = 0;
			// otherwise the bit is 1
			else
				__dht__received__binary__[i]=1;

			// wait until the pin goes low
			while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin));
		}
	}


}

//
float DHT22_GetTemperature(){

//	uint8_t tempBin[16];
	float tempDecimal = 0;

	// converting binary to decimal
	uint8_t exp = 0;
	for (uint8_t j = 31; j > 16; j--){
		tempDecimal += pow(2, exp) * __dht__received__binary__[j];
		exp++;
	}

	// resulting decimal is 10 times more than the real temperature
	return tempDecimal / 10.0;

}

float DHT22_GetHumidity(){

	float rhDecimal = 0;

    // converting binary to decimal

	uint8_t exp = 0;
	for (uint8_t j = 15; j > 0; j--){
		rhDecimal += pow(2, exp) * __dht__received__binary__[j];
		exp++;
	}

	// resulting decimal is 10 times more than the real temperature
	return rhDecimal / 10.0;

}









