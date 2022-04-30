/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOA Pin0  --> Switch1/Button1
*  	Push the button will generate interrupt for EXTI0 Line0  
* 	Falling trigger, Input with pullup mode.
GPIOB Pin14 --> Switch2/Button2
* 	Push the button will generate interrupt for EXTI14 Line14
* 	Falling trigger, input with pullup mode.
GPIOC Pin13 --> LED (Active Low)
* 	Button1 : Blink 1x 
* 	Button2 : Blink 2x 
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
	// Turn on the GPIOC,GPIOA,GPIOB, & SYSCFG peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN|
			RCC_AHB1ENR_GPIOAEN|
			RCC_AHB1ENR_GPIOBEN;
   
        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    
	// GPIOA Pin0 input with Pullup
	GPIOA->MODER &=~GPIO_MODER_MODE0;
	GPIOA->PUPDR |=GPIO_PUPDR_PUPD0_0;
	
	// GPIOB Pin14 input with Pullup
	GPIOB->MODER &=~GPIO_MODER_MODE14;
	GPIOB->PUPDR |=GPIO_PUPDR_PUPD14_0;
	
	    
	// GPIOC Pin13 output
	GPIOC->MODER|= GPIO_MODER_MODE13_0; 
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;
	
	// Set EXTI0 PA0 line interupt	
	SYSCFG->EXTICR[0]&=~ SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0]|= SYSCFG_EXTICR1_EXTI0_PA;
		
	// Set EXTI14 PB14 line interupt	
	SYSCFG->EXTICR[3]&=~SYSCFG_EXTICR4_EXTI14;
	SYSCFG->EXTICR[3]|= SYSCFG_EXTICR4_EXTI14_PB;
	
	
	// Unmask Line 0 & Line 14
	EXTI->IMR|= EXTI_IMR_MR0|EXTI_IMR_MR14;
	
	// Falling Triger input Line 0 & Line 14
	EXTI->FTSR|= EXTI_FTSR_TR0| EXTI_FTSR_TR14;
	
	// EXTI Line 0
	NVIC_SetPriority(EXTI0_IRQn, 1);
	// EXTI Line 10-15
	NVIC_SetPriority(EXTI15_10_IRQn, 1);
	
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	// Default LED=Off
	GPIOC->ODR |=GPIO_ODR_OD13;
	
__enable_irq();


while (1) {
    	    // Do nothing 
			}

	// Return 0 to satisfy compiler
	return 0;
}
	
void EXTI0_IRQHandler(void) {
	
	//Blink LED 1x			
	GPIOC->ODR ^=GPIO_ODR_OD13;
	delay();
	GPIOC->ODR ^=GPIO_ODR_OD13;
	delay();
	
	// Set LED=Off before exit			
	GPIOC->ODR |=GPIO_ODR_OD13;
	
	// Clear interrupt pending flag
	EXTI->PR |= EXTI_PR_PR0;          
}
void EXTI15_10_IRQHandler(void) {
	
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
	EXTI->PR |= EXTI_PR_PR14;          
}


