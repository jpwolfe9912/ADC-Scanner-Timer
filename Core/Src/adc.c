/*
 * adc.c
 *
 *  Created on: Jul 1, 2021
 *      Author: jeremywolfe
 *
 *  ADC3_INP3 on PF7
 *  ADC3_INP2 on PF9
 *  try to run it without the timer
 *  bank of potentiometers
 */
#include "adc.h"

void ADC3_CH6_Init(void){
	ADC3->CR &= ~ADC_CR_ADEN;							// ensure that the ADC is off
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOFEN;				// enable clock for port F
	RCC->AHB4ENR |= RCC_AHB4ENR_ADC3EN;					// enable clock for ADC3
	ADC3_COMMON->CCR |= ADC_CCR_CKMODE_0 | ADC_CCR_CKMODE_1;	// (25.4.3) choosing the clock source for ADC

	GPIOF->MODER = GPIO_MODER_MODE10_0 | GPIO_MODER_MODE10_1;	// setting PF10 as alternate function
	GPIOF->MODER = GPIO_MODER_MODE7_0 | GPIO_MODER_MODE7_1;

	ADC3->CR &= ~ADC_CR_DEEPPWD;						// (25.4.6) takes ADC out of deep power down mode

	ADC3->CR |= ADC_CR_ADVREGEN;						// (25.4.6) enable the voltage regulator
	while(!(ADC3->ISR & ADC_ISR_LDORDY)){}				// (25.4.6) waits for reg to startup

	ADC3_COMMON->CCR |= ADC_CCR_TSEN;

	/*	Turning on the ADC (25.4.9)	*/
	ADC3->ISR |= ADC_ISR_ADRDY;							// clear the ADRDY bit
	ADC3->CR |= ADC_CR_ADEN;							// enable the ADC
	while(!(ADC3->ISR & ADC_ISR_ADRDY)){}				// wait for ADC to be ready
	ADC3->ISR |= ADC_ISR_ADRDY;

	ADC3->CR &= ~ADC_CR_ADSTART;


}

void ADC3_CH6_Cont(void){
	ADC3->IER |= ADC_IER_EOCIE | ADC_IER_EOSIE;// | ADC_IER_EOSMPIE;			// enable end of conversion interupts
	NVIC_EnableIRQ(ADC3_IRQn);							// enable on the NVIC

	ADC3->PCSEL |= ADC_PCSEL_PCSEL_3;	// preselect channel
	ADC3->PCSEL |= ADC_PCSEL_PCSEL_2;	// preselect channel

	ADC3->SQR1 |= ADC_SQR1_L_0;			// sequence of 2 channels
	ADC3->SQR1 |= ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_0;		// select channel 6 as first in sequence
	ADC3->SQR1 |= ADC_SQR1_SQ2_1;

	ADC3->SMPR1 |= ADC_SMPR1_SMP3_1; //| ADC_SMPR1_SMP6_1 | ADC_SMPR1_SMP6_2;					// 32.5 clock cycles
	ADC3->SMPR1 |= ADC_SMPR1_SMP2_1; //| ADC_SMPR1_SMP2_1 | ADC_SMPR1_SMP2_2;					// 32.5 clock cycles

//	ADC3->CFGR |= ADC_CFGR_AUTDLY;
	ADC3->CFGR &= ~ADC_CFGR_CONT;						// disable continuous mode
//	ADC3->CFGR |= ADC_CFGR_CONT;						// disable continuous mode

	ADC3->CFGR |= ADC_CFGR_EXTEN_0;						// trigger on rising edge
	ADC3->CFGR |= ADC_CFGR_EXTSEL_0 | ADC_CFGR_EXTSEL_1 | ADC_CFGR_EXTSEL_3;// trigger on event 9, tim1_trgo2
	ADC3->CR |= ADC_CR_ADSTART;							// start conversions
}

int ADC3_Calibration(void){
	/*	Calibrating the ADC	*/
	ADC3->CR &= ~ADC_CR_ADEN;							// ensure that the ADC is off
	ADC3->CR |= ADC_CR_ADCAL;							// start calibration
	while(ADC3->CR & ADC_CR_ADCAL){}					// wait until ADCAL is 0 and cal is complete

	printf("\nCalibration is complete");
	return (int)ADC3->CALFACT;
}
