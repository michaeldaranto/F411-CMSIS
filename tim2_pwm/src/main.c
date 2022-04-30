/*********************************************************
WeAct BlackPill STM32F411CEU6
TIM2-CH3 Output Compare PWM - GPIOA Pin2
HSI=16MHz, APB1ENR=16MHz
Test with Saleae Logic Analyzer
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

void GPIOA_Init (void);
void TIM2_Init(void);

int main (void) {

	GPIOA_Init();
	TIM2_Init();

	while (1) {
					
	//	TIM2->CCR3=99;  // _|`|_____________________ 10%
		TIM2->CCR3=499; // ````````````|____________ 50%
	}
	// Return 0 to satisfy compiler
	return 0;
}

void TIM2_Init (void) {
	// HSI=16MHz
	RCC->APB1ENR |=RCC_APB1ENR_TIM2EN|RCC_APB1ENR_PWREN;
	// Prescaler 16Mhz/1 =16MHz
	TIM2->PSC= 0;
	// Auto Reload Register for 16KHz 
	TIM2->ARR= 1000-1;
	// Clear counter
	TIM2->CNT=0;
	// Down COunter 
	TIM2->CR1|=TIM_CR1_DIR;
	// CC3S as output
	TIM2->CCMR2 &=~TIM_CCMR2_CC3S;
	// PWM Mode
	TIM2->CCMR2 &=~TIM_CCMR2_OC3M;
	TIM2->CCMR2|= TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2;
	// Enable CH3
	TIM2->CCER |= TIM_CCER_CC3E;
	//Enable counter
	TIM2->CR1|=TIM_CR1_CEN;
}
void GPIOA_Init (void) {
	// Turn on the GPIOA peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Set GPIOA Pin2 AF 
	GPIOA->MODER|= GPIO_MODER_MODE2_1;
	GPIOA->MODER&=~GPIO_MODER_MODE2_0;
	// AF01 TIM2 CH3 
	GPIOA->AFR[0] &=~GPIO_AFRL_AFRL2 ;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_0;
}
