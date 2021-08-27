/*
 * timer.c
 *
 *  Created on: Jul 1, 2021
 *      Author: jeremywolfe
 *
 *      TIM1_CH2 on PA0
 */

#include "timer.h"

int div = 0;
int freqSlow = 0;

void TIM2_CH1_Init(void){

	RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;

	GPIOA->MODER &= ~0xFFFF;
	GPIOA->MODER |= GPIO_MODER_MODE0_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_0;

	TIM2->EGR |= TIM_EGR_UG;
	TIM2->CNT	= 0;										// start the counter at 0

	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR2 |= TIM_CR2_MMS_1;
	NVIC_EnableIRQ(TIM2_IRQn);

	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; 	// enable CH1 as PWM mode on CCMR1
	TIM2->CCER |= TIM_CCER_CC1E;							// enables channel 1 as output
	TIM2->CR1 = TIM_CR1_CEN;								// enables the counter

}

void TIM2_CH1_Freq_DC(float freq, float DC){
	freq = (int)(64000000 / freq);
	if(freq <= 65536){
		TIM2->PSC = 0;
		TIM2->ARR = freq - 1;
		TIM2->CCR1 = (int)(freq * DC) - 1;
	}
	else{
		freqSlow = freq;
		while(freqSlow > 65536){
			div++;
			freqSlow = freq / div;
		}
		TIM2->PSC = div - 1;
		TIM2->ARR = freqSlow - 1;
		TIM2->CCR1 = (int)(freqSlow * DC) - 1;
	}

}





