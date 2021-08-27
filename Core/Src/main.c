#include "stm32h7xx.h"
#include "printUSART.h"
#include "timer.h"
#include "adc.h"

#define ADC_CH62

// ADC3_INP6 on PF10
// ADC3_INP2 on PF9
// ADC3 on AHB4

int buff[2];
int flag = 0;
int flag_EOS = 0;

int main(void){

	USART3_Init();
	ADC3_CH6_Init();
	ADC3_CH6_Cont();
	TIM2_CH1_Init();
	TIM2_CH1_Freq_DC(1000, 0.5);
	printf("Start of Conversions:\n");

	while(1){
		if(flag_EOS == 1){
			printf("\nPOT DATA");
			printf("\nData: %f, %f\n", ((float)buff[0] / 65535.0 * 3.3), ((float)buff[1] / 65535.0 * 3.3));		// print value in data register
			flag_EOS = 0;
		}

	}
}

void TIM2_IRQHandler(void){
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~TIM_SR_UIF;
	}
}

void ADC3_IRQHandler(void){
	if(ADC3->ISR & ADC_ISR_EOC){
		ADC3->ISR |= ADC_ISR_EOC;		// write a 1 to clear
		if(flag == 0){
			buff[0] = (int)ADC3->DR;
			flag = 1;
//			printf("Conversion: %f\n", ((int)ADC3->DR / 65535.0 * 3.3));
		}
		else{
			buff[1] = (int)ADC3->DR;
			flag = 0;
//			printf("Conversion: %f\n", ((int)ADC3->DR / 65535.0 * 3.3));
		}
	}
	if((ADC3->ISR & ADC_ISR_EOS)){ // wait for both conversions to complete
		ADC3->ISR |= ADC_ISR_EOS;
		flag_EOS = 1;
//		printf("Sequence: %f, %f\n\n", (buff[0] / 65535.0 * 3.3), (buff[1] / 65535.0 * 3.3));

	}
}




