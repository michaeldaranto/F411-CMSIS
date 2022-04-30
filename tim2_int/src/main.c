/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOC Pin13 --> LED
TIM2 Simple timer with Update Interrupt Enable
HSI=16MHz, APB1ENR=16MHz
 
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

void GPIOC_Init (void);
void TIM2_Init(void);

int main (void) {

	GPIOC_Init();
	TIM2_Init();
	NVIC_EnableIRQ(TIM2_IRQn);

	while (1) {}
		
	// Return 0 to satisfy compiler
	return 0;
}

void TIM2_Init (void) {
	// HSI=16MHz
	RCC->APB1ENR |=RCC_APB1ENR_TIM2EN|RCC_APB1ENR_PWREN;
	// Prescaler 16Mhz/1600 =10000Hz
	TIM2->PSC= 1600-1;
	// Auto Reload Register for 1S
	TIM2->ARR= 10000;
	// Clear counter
	TIM2->CNT=0;
	// Down COunter , Counter Enable
	TIM2->CR1|=TIM_CR1_DIR|TIM_CR1_CEN;
	// Update Interrupt Enable
	TIM2->DIER|=TIM_DIER_UIE;
}
void GPIOC_Init (void) {
	// Turn on the GPIOC peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	// Set GPIOC Pin13 Output
	GPIOC->MODER|= GPIO_MODER_MODE13_0;
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;
}
void TIM2_IRQHandler(void) {
    // Clear UIF bit
	TIM2->SR &=~TIM_SR_UIF;
	// Toggle LED	
    GPIOC->ODR ^=GPIO_ODR_OD13; 
}
