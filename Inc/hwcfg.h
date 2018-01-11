/*
red >>>> VDD
black >> GND
White >> PC11
*/


#include "stm32f0xx_hal.h"

#ifndef __HWCFG_H
#define __HWCFG_H

#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define DS18B20_DQ_Pin GPIO_PIN_11
#define DS18B20_DQ_GPIO_Port GPIOC

#define LED_ON()                       HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define LED_OFF()                      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);

#ifdef __GNUC__
	/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
	   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif
