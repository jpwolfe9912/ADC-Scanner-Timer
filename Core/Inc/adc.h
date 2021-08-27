/*
 * adc.h
 *
 *  Created on: Jul 1, 2021
 *      Author: jeremywolfe
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32h7xx.h"
#include "printUSART.h"

#define ADC_ISR_LDORDY 0x1 << 12U
#define TS_CAL2		((uint16_t*)((uint32_t)0x1FF1E840))
#define TS_CAL1		((uint16_t*)((uint32_t)0x1FF1E820))

#define ADC_CH6
#define ADC_CH62

void ADC3_CH6_Init(void);
void ADC3_CH6_Cont(void);
int ADC3_Calibration(void);

#endif /* INC_ADC_H_ */
