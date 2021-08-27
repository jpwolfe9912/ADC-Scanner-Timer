/*
 * timer.h
 *
 *  Created on: Jul 1, 2021
 *      Author: jeremywolfe
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32h7xx.h"

void TIM2_CH1_Init(void);
void TIM2_CH1_Freq_DC(float freq, float DC);

#endif /* INC_TIMER_H_ */
