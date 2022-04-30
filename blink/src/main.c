/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOC Pin13 --> LED
Blink LED 
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

//Simple delay
void delay (void){
	for (uint32_t i=0; i < 500000; i++)
		__asm("nop");
}
int main (void) {
	// Turn on the GPIOC peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	    
	// Set GPIOC Pin13 Output
	GPIOC->MODER|= GPIO_MODER_MODE13_0;
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;

	while (1) {
	    //XOR to toggle
    	    GPIOC->ODR ^=GPIO_ODR_OD13; 
    	    delay();
	}
	// Return 0 to satisfy compiler
	return 0;
}
