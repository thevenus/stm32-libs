/*
 * dht.h
 *
 *  Created on: Nov 13, 2019
 *      Author: huseyn
 */

#ifndef DHT_H_
#define DHT_H_

struct dht
{
	int temperature;
	int humidity;
};

void dht22_measure();

int dht22_get_temperature();

int dht22_get_humidity();

volatile int data[40];


#endif /* DHT_H_ */
