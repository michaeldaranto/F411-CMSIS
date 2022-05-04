/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOA Pin0  --> Switch/Button 
* 	Push the button will generate interrupt for EXTI0 Line0 
* 	Falling trigger, Input with pullup mode.
GPIOC Pin13 --> LED
* 	The LED will Blink 2x  
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
	
__disable_irq();
	// Turn on the GPIOC,GPIOA, & SYSCFG peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN|RCC_AHB1ENR_GPIOAEN;
    	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    
	// GPIOA Pin0 input with Pullup
	GPIOA->MODER &=~GPIO_MODER_MODE0;
	GPIOA->PUPDR |=GPIO_PUPDR_PUPD0_0;
	    
	// GPIOA Pin13 output
	GPIOC->MODER|= GPIO_MODER_MODE13_0; 
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;
	
	// Set EXTI0 PA0 line interupt	
	SYSCFG->EXTICR[0]&=~ SYSCFG_EXTICR1_EXTI0;
	// SYSCFG->EXTICR[0]|= SYSCFG_EXTICR1_EXTI0_PA;
	
	// Unmask Line 0
	EXTI->IMR|= EXTI_IMR_MR0;
	
	// Falling Triger input Line 0
	EXTI->FTSR|= EXTI_FTSR_TR0;
	
	NVIC_SetPriority(EXTI0_IRQn, 1);
	NVIC_EnableIRQ(EXTI0_IRQn);
	
	// Default LED=Off
	GPIOC->ODR |=GPIO_ODR_OD13;
	
__enable_irq();


while (1) {
    	    // Do nothing 
			}

	// Return 0 to satisfy compiler
	return 0;
}
// Use extern C if compile with C++	
//extern "C" { 
void EXTI0_IRQHandler(void) {
	
	//Blink LED 2x			
	GPIOC->ODR ^=GPIO_ODR_OD13;
	delay();
	GPIOC->ODR ^=GPIO_ODR_OD13;
	delay();
	GPIOC->ODR ^=GPIO_ODR_OD13;
	delay();
	// Set LED=Off before exit			
	GPIOC->ODR |=GPIO_ODR_OD13;
	
	// Clear interrupt pending flag
	EXTI->PR |= EXTI_PR_PR0;          
}
//}
