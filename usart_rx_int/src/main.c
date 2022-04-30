/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOC Pin13 --> LED
USART1 RX Interrupt -->GPIOA Pin10
* If ASCII char Rx < 5, Turn On LED 
* Minicom : press 3, LED On. Press 9, LED Off
* HSI=16 MHz,APB2ENR = 16MHz
* Bps/Par/Bits : 115200/-/8N1 
* Hardware Flow Control = No
* Software Flow Control = No
* 
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

void GPIOC_Init(void);
void USART1_Init(void);
void delay (void);

int main (void) {

	GPIOC_Init();
	USART1_Init();
	NVIC_EnableIRQ(USART1_IRQn);

	while (1) {
		//Do nothing here :)
		}
	// Return 0 to satisfy compiler
	return 0;
}

void USART1_Init(void){
	
	// Turn on GPIOA peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Turn on USART1 peripheral
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// GPIOA Pin10 AF
	GPIOA->MODER &=~GPIO_MODER_MODE10;
	GPIOA->MODER |=GPIO_MODER_MODE10_1;
	// Set High Speed Pin10 
	GPIOA->OSPEEDR |=GPIO_OSPEEDR_OSPEED10;
	
	// AFRH AF7 Alternate function Mapping - page 47 datasheet
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10;
	GPIOA->AFR[1]|=	GPIO_AFRH_AFSEL10_0|
					GPIO_AFRH_AFSEL10_1|
					GPIO_AFRH_AFSEL10_2;	 
	
	// OVER8=1, RX Enable, RX interrupt enable 
	USART1->CR1|= USART_CR1_OVER8|USART_CR1_RE|USART_CR1_RXNEIE;
	
	// See RM0383 page 518-527
	// HSI=16MHz, FPCLK=16MHz, Baud rate = 115200 (Actual 115108), OVER8=1 (DIV_Fraction[2:0] bits)
	// Tx/Rx baud = fCK/(8*(2-OVER8)*USARTDIV)
	// 115200 = 16000000/8*USARTDIV
	// USARTDIV = 16000000/(8*115108)= 17.375
	// Mantisa = 17
	// If OVER8=1, fraction * 8
	// Fraction = 8*0.375= 3 
		
	USART1->BRR &=~USART_BRR_DIV_Mantissa;
	USART1->BRR|=(17 << USART_BRR_DIV_Mantissa_Pos);
	USART1->BRR &=~USART_BRR_DIV_Fraction;
	USART1->BRR|=(3 << USART_BRR_DIV_Fraction_Pos);
	
	// USART1 Enable
	USART1->CR1|= USART_CR1_UE;				
}

void USART1_IRQHandler(void) {
	// Turn On LED if Rx data < 0x35
	if (USART1->DR < 0x35) 
			GPIOC->ODR &=~GPIO_ODR_OD13;  
			else GPIOC->ODR |=GPIO_ODR_OD13; 
		// Clear Bit RXNE for the next ticket show
		USART1->SR &=~USART_SR_RXNE;
 }
void GPIOC_Init(void){
	// Turn on the GPIOC peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	// Set GPIOC Pin13 Output
	GPIOC->MODER|= GPIO_MODER_MODE13_0;
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;
}

//Simple delay
void delay (void){
	for (uint32_t i=0; i < 500000; i++)
		__asm("nop");
}

	


