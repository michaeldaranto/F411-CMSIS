/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOC Pin13 --> LED
    HSE= 25MHz  
    AHB=100MHZ 
    SysTick CLKSOURCE = AHB/8 =12.5MHz
    SysTick Pool delay to Blink LED 
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

int main (void) {

	// Turn on the GPIOC peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // Set GPIOC Pin13 Output
	GPIOC->MODER|= GPIO_MODER_MODE13_0;
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;

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
    
	//APB1= HCLK/2 --> 50MHz
	RCC->CFGR |=RCC_CFGR_PPRE1_DIV2;    
	//SW PLLCLK
	RCC->CFGR |=RCC_CFGR_SW_PLL;
	
	//100Ms
	SysTick->LOAD = 1250000UL-1UL;
	SysTick->VAL = 0UL;
	
	//AHB/8 = 12.5MHz
	SysTick->CTRL =SysTick_CTRL_ENABLE_Msk;
	
        while (1) {
		// Check if the counter down to zero
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) 
			// Toggle with XOR magic! 
			GPIOC->ODR ^=GPIO_ODR_OD13;
	    	}
	// Return 0 to satisfy compiler
	return 0;
}
