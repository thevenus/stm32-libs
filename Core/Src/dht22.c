/*
 * dht.c
 *
 *  Created on: Nov 13, 2019
 *      Author: huseyn
 */

#include "micros.h"
#include "dht22.h"

void dht22_measure()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	DWT_Init();

	// ===starting communication===
	// sending start signal

	GPIO_InitStruct.Pin = DHT22_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);


	HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET);
	delay_micros(1000);

	HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET);
	delay_micros(30);

	// configure DHT22_Pin:PC10 for Input
	GPIO_InitStruct.Pin = DHT22_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);

	// ===checking response===
	uint8_t check = 0;
	delay_micros(40);

	if (!HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)) {
		delay_micros(80);
		if (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)) {
			check = 1;
//			printf("\n\rEverything is fine\n\r");
		}

	}

	// waiting till the sensor sends low to the pin
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
				data[i] = 0;
			// otherwise the bit is 1
			else
				data[i]=1;

			while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin));
		}
	}


}









