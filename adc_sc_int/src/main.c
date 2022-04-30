/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOC Pin13 --> LED
ADC1 IN3 GPIOA Pin3  
Single channel, Single Conversion with Interrupt
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

void GPIOC_Init(void);
void ADC_Init(void);
void delay(void);

uint16_t data;
int main (void) {
	
	GPIOC_Init();
	ADC_Init();
	
	while (1) {
		// Start conversion
		ADC1->CR2 |= ADC_CR2_SWSTART;
		delay();
	}
	return 0;
}
void ADC_Init(void){
		
	// Turn on GPIOA Peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Enable ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 
	// Set Mode Analog
	GPIOA->MODER |= GPIO_MODER_MODE3;
	// Disable ADC
	ADC1->CR2 &=~(ADC_CR2_ADON|ADC_CR2_SWSTART);
	// 1st conversion IN3
	ADC1->SQR3 |= ADC_SQR3_SQ1_0|ADC_SQR3_SQ1_1;
	// Interupt Enable
	ADC1->CR1 |= ADC_CR1_EOCIE;
	NVIC_EnableIRQ(ADC_IRQn);
	// ADC ON
	ADC1->CR2 |= ADC_CR2_ADON;
}

void GPIOC_Init(void) {
	// Turn on the GPIOC peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	// Set GPIOC Pin13 Output
	GPIOC->MODER|= GPIO_MODER_MODE13_0;
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;
}	

void ADC_IRQHandler(void){
	data=ADC1->DR;

	// for 12bit ADC, range 0-0x0FFF (Default Right Alignment) CR2-ALIGN
	// Pin A3 connect to 3.3V --> max value, LED Off
	// Pin A3 connect to GND -->  min value, LED On
	if (data > 0x07FF)
		// LED off (active low)
		GPIOC->ODR |=GPIO_ODR_OD13;
		else
		// LED ON
		GPIOC->ODR &=~GPIO_ODR_OD13;	
}

//Simple delay
void delay (void){
	for (uint32_t i=0; i < 500000; i++)
		__asm("nop");
}
