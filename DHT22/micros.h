/*
 * @file: micros.h
 * @author: Fuad Mammadzade, Huseyn Gambarov
 * @brief: Header file for microseconds delay code
 *
 * */
#ifndef DELAY_MICROS_H_
#define DELAY_MICROS_H_

#include "main.h"

void DWT_Init(void);
void delay_micros(uint32_t us);

#endif /* DELAY_MICROS_H_ */
