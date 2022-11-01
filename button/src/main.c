/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOA Pin0  --> Switch/Button
GPIOC Pin13 --> LED
Press the Button to slowdown (Blink LED) 
Michael Daranto - YD3BRB
GPLv3
**********************************************************/

#include "stm32f4xx.h"

// Simple delay
void delay (void){
	for (uint32_t i=0; i < 500000; i++)
		__asm("nop");
}
int main (void) {
	// Turn on the GPIOC & GPIOA peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
	// GPIOA Pin0 input
	GPIOA->MODER&=~GPIO_MODER_MODE0; 
	
	// GPIOA Pin0 PullUp
	GPIOA->PUPDR |=GPIO_PUPDR_PUPD13_0; 
	GPIOA->PUPDR&=~GPIO_PUPDR_PUPD13_1; 
    
	// GPIOA Pin13 output
	GPIOC->MODER|= GPIO_MODER_MODE13_0; 
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;
    
	while (1) {
    	    // Check if someone press the button 
		if (GPIOA->IDR&GPIO_IDR_ID0) {
		GPIOC->ODR ^=GPIO_ODR_OD13;
		delay();
		}
		else {
		GPIOC->ODR ^=GPIO_ODR_OD13;
    		delay();
		delay();
    		}
		}

	// Return 0 to satisfy compiler
	return 0;
}
