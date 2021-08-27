/*
 * printUSART.h
 *
 *  Created on: Jun 20, 2021
 *      Author: jeremywolfe
 */

#ifndef INC_PRINTUSART_H_
#define INC_PRINTUSART_H_

#include "stm32h7xx.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"

void USART3_Init(void);
int USART3_Write(int ch);
int USART3_Read(void);

#endif /* INC_PRINTUSART_H_ */
