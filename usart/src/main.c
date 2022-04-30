/*********************************************************
WeAct BlackPill STM32F411CEU6
GPIOC Pin13 --> LED

USART1 TX -->GPIOA Pin9, RX -->GPIOA Pin10
* HSI=16 MHz,APB2ENR = 16MHz
* Baud rate = 115200 
* Stop bit=1
* no flow control
* data =8 bit 		
* 
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

void USART1_Init(void);
void USART1_Tx_Char(uint8_t c);
void USART1_Tx_String(char *s);
void delay (void);

int main (void) {
	
	
	USART1_Init();

	while (1) {
		
		USART1_Tx_String("Hello World! \r\n");
		USART1_Tx_String("Yessss!!! STM32F411CE\r\n");
		USART1_Tx_String("BlackPill SIpppp !!! \r\n");

		delay();
	}
	// Return 0 to satisfy compiler
	return 0;
}

void USART1_Init(void){
	
	// Turn on GPIOA peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Turn on USART1 peripheral
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// GPIOA Pin9 AF Pin10 AF
	GPIOA->MODER &=~(GPIO_MODER_MODE9|GPIO_MODER_MODE10);
	GPIOA->MODER |=GPIO_MODER_MODE9_1|GPIO_MODER_MODE10_1;
	// Set High Speed for GPIOA Pin9 & 10 
	GPIOA->OSPEEDR |=GPIO_OSPEEDR_OSPEED9|GPIO_OSPEEDR_OSPEED10;
	
	// AFRH AF7 Alternate function Mapping - page 47 datasheet
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL9|GPIO_AFRH_AFSEL10);
	GPIOA->AFR[1]|=	GPIO_AFRH_AFSEL9_0|
					GPIO_AFRH_AFSEL9_1|
					GPIO_AFRH_AFSEL9_2|		
					GPIO_AFRH_AFSEL10_0|
					GPIO_AFRH_AFSEL10_1|
					GPIO_AFRH_AFSEL10_2;	 
	
	// OVER8=1, TX & RX Enable
	USART1->CR1|= USART_CR1_OVER8|USART_CR1_TE|USART_CR1_RE;
	
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


void USART1_Tx_Char(uint8_t c){
	while (!(USART1->SR & USART_SR_TC));
	USART1->DR=(c);
	}

void USART1_Tx_String(char *s)	{
	while(*s!='\0')
	{
	USART1_Tx_Char(*s);
	s++;
		}
	}

//Simple delay
void delay (void){
	for (uint32_t i=0; i < 500000; i++)
		__asm("nop");
}

// Turn on the GPIOC peripheral
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    
	// Set GPIOC Pin13 Output
//	GPIOC->MODER|= GPIO_MODER_MODE13_0;
//	GPIOC->MODER&=~GPIO_MODER_MODE13_1;
//	USART1->DR=data[tx1];
	

