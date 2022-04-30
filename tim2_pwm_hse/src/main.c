/*********************************************************
WeAct BlackPill STM32F411CEU6
TIM2-CH3 Output Compare PWM - GPIOA Pin2
HSE=25MHz,PCLK=50MHz (Max),TIM2CLK =2xPCLK= 100MHz (Max)
Test with Saleae Logic Analyzer
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

void GPIOA_Init (void);
void HSE_Init(void);
void TIM2_Init(void);

int main (void) {

	GPIOA_Init();
	HSE_Init();
	TIM2_Init();

	while (1) {
					
	//	TIM2->CCR3=9;  // _|`|_____________________ 10% Approximately
		TIM2->CCR3=49; // ````````````|____________ 50%
	}
	// Return 0 to satisfy compiler
	return 0;
}

void TIM2_Init (void) {
	// HSE=25MHz, PCLK=50MHz (Max), TIM2CLK = 100MHz
	RCC->APB1ENR |=RCC_APB1ENR_TIM2EN|RCC_APB1ENR_PWREN;
	// Prescaler 100Mhz/1 =100MHz
	TIM2->PSC= 0;
	// Auto Reload Register for 1MKHz 
	TIM2->ARR= 100-1;
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
void HSE_Init(void){
	
	// For 96+Mhz	
	FLASH->ACR |=FLASH_ACR_LATENCY_2WS;
	// HSE = 25MHZ Xtal onboard
	RCC->CR |=RCC_CR_HSEON;
	while ((RCC->CR & RCC_CR_HSERDY)==0);
	// PLL Source HSE = 25MHz 
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	// Reset PLLM
	RCC->PLLCFGR &=~RCC_PLLCFGR_PLLM;
	// /M 12 --> 25MHz/12  --> 2.08333333333 MHz (nice number :)
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_2|RCC_PLLCFGR_PLLM_3;
	// Reset PLLN
	RCC->PLLCFGR &=~RCC_PLLCFGR_PLLN;
	// *N 96 --> (25MHz/12)*96 --> 200MHz  PLLN = 1100000
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_5|RCC_PLLCFGR_PLLN_6;
	// Reset PLLP
	RCC->PLLCFGR &=~RCC_PLLCFGR_PLLP;
	// /P 2  --> ((25MHz/12)*96)/2 --> 100MHz  PLLP= 00
	RCC->PLLCFGR &=~RCC_PLLCFGR_PLLP; //sure! lol
	// PLL is ready to go!
	RCC->CR |=RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY)==0);
	// APB1= HCLK/2 --> 50MHz
   	RCC->CFGR |=RCC_CFGR_PPRE1_DIV2;    
	// TIM2CLK = 2xPCLK2 = 2x50MHz =100MHz
	RCC->DCKCFGR &=~RCC_DCKCFGR_TIMPRE; // Proforma :)
	//PLL Selected as system clock 
	RCC->CFGR |=RCC_CFGR_SW_PLL;
}
