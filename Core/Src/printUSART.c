/*
 * PrintUSART.c
 *
 *  Created on: Apr 9, 2021
 *      Author: jeremywolfe
 *      Datasheet: https://www.st.com/resource/en/datasheet/stm32f411re.pdf
 *      Reference Manual: https://www.st.com/resource/en/reference_manual/dm00119316-stm32f411xc-e-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
 */

// TX3 on PD8
// RX3 on PD9
// Port D is on AHB4
// USART3 is on APB1
// USART3 is on AF7

#include <printUSART.h>

void USART3_Init(void){

	RCC->AHB4ENR |= RCC_AHB4ENR_GPIODEN; 											// enable the clock for port D
	RCC->APB1LENR |= RCC_APB1LENR_USART3EN; 										// enable the clock for USART3

	GPIOD->AFR[1] 	|= GPIO_AFRH_AFSEL8_0 | GPIO_AFRH_AFSEL8_1 | GPIO_AFRH_AFSEL8_2;	// set pin D8 as alternate function
	GPIOD->AFR[1] 	|= GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2;	// set pin D9 as alternate function
	GPIOD->MODER 	= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;							// set PA2 and PA3 as alternate function

	USART3->BRR 	= 0x022C; 															// set baud rate to 115200
	USART3->CR1 	|= USART_CR1_UE; 									// enable the receiver, transmitter, and USART
	USART3->CR1		&= ~USART_CR1_TE;
	//while(!(USART3->ISR & USART_ISR_TEACK)){}
	USART3->CR1 	|= USART_CR1_TE | USART_CR1_RE;

}

int USART3_Write(int ch){
	while (!(USART3->ISR & USART_ISR_TXE_TXFNF)){}	// waits for TX buffer to become empty
	USART3->TDR = ch;								// transfers the value of the data register into ch
//	while (!(USART3->ISR & USART_ISR_TC)) {}		// waits for TX to complete
	return 0;
}

int USART3_Read(void) {
	while (!(USART3->ISR & USART_ISR_RXNE_RXFNE)) {}
	return USART3->RDR;
}

/*	This is required to use printf											*/
/*	This basically tells the compiler what to do when it encounters printf	*/
/*	I honestly can't fully explain what is going on but it works			*/
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//	#define GETCHAR_PROTOTYPE int __io_getchar (void)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//	#define GETCHAR_PROTOTYPE int fgetc(FILE * f)
#endif

PUTCHAR_PROTOTYPE{
	USART3_Write(ch);
	return ch;
}
