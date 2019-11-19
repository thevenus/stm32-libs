/*
 * micros.h
 *
 *  Created on: Nov 13, 2019
 *      Author: huseyn
 */

#ifndef DELAY_MICROS_H_
#define DELAY_MICROS_H_
/*
#define delay_micros(us) do { \
asm volatile ("MOV R0,%[loops]\n \
				1: \n \
				SUB R0, #1\n \
				CMP R0, #0\n \
				BNE 1b \t" \
				: : [loops] "r" (16*us) : "memory" \
				); \
} while(0)*/

#include "main.h"

void DWT_Init(void);
void delay_micros(uint32_t us);

#endif /* DELAY_MICROS_H_ */
