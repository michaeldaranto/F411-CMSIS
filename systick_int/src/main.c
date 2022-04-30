/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOC Pin13 --> LED
    HSI=16MHz
    AHB=16MHz
    SysTick CLKSOURCE=AHB=16MHz
    SysTick Interupt to Blink LED 
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

int main(void) {

__disable_irq();
	// Turn on the GPIOC peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // Set GPIOC Pin13 Output
	GPIOC->MODER|= GPIO_MODER_MODE13_0;
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;

	// Default HSI RC = 16MHz, 1S tick (change to 1600000 for 100mS tick)
	SysTick->LOAD = 16000000-1;
	SysTick->VAL = 0UL;

	// CLKSOURCE = AHB = 16MHz, SysTick Interupt enable
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|
					SysTick_CTRL_TICKINT_Msk|
					SysTick_CTRL_ENABLE_Msk;
__enable_irq();

	while(1) {
		// Do nothing here :)
		}
	return 0; 
}
// Every 1S, SysTick knock the door
void SysTick_Handler(void){
	GPIOC->ODR ^=GPIO_ODR_OD13;
}

