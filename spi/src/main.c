/*********************************************************
WeAct BlackPill STM32F411CEU6
HSI= 16MHz
SPI2 - Send One Byte data
* GPIOB Pin13 -> SCK
* GPIOB Pin14 -> MISO
* GPIOB Pin15 -> MOSI
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

void SPI2_Init(void);
void SPI2_Tx(uint8_t onebyte);

int main (void) {
		
	SPI2_Init();
	while (1) {
	    SPI2_Tx('A');			//0x41	0b01000001
	    SPI2_Tx('9');			//0x39	0b00111001 
		SPI2_Tx(255);			//0xFF	0b11111111	   
	    SPI2_Tx(0b10101010);	//0xAA	0b10101010
	}
	// Return 0 to satisfy compiler
	return 0;
}
void SPI2_Init(){
	// Turn on the GPIOB peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	// Reset first Pin13,14,15 
	GPIOB->MODER &=~(GPIO_MODER_MODE13|GPIO_MODER_MODE14|GPIO_MODER_MODE15);
	// Set GPIOB Pin13,14,15 AF 
	GPIOB->MODER |=GPIO_MODER_MODE13_1|GPIO_MODER_MODE14_1|GPIO_MODER_MODE15_1;
	// AF05 - SPI2 see datasheet page 48
	GPIOB->AFR[1]|= GPIO_AFRH_AFSEL13_0|GPIO_AFRH_AFSEL13_2| // b0101
					GPIO_AFRH_AFSEL14_0|GPIO_AFRH_AFSEL14_2|
					GPIO_AFRH_AFSEL15_0|GPIO_AFRH_AFSEL15_2;
	// Turn on APB1! SPI2 connect to APB1 - See datasheet page 15. Why ST like SP than SPI?
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN ;
	// Disable NSS, act as Master
	SPI2->CR1 |= SPI_CR1_SSM|SPI_CR1_SSI|SPI_CR1_MSTR;
	// Enable SPI
	SPI2->CR1 |= SPI_CR1_SPE;
}
void SPI2_Tx(uint8_t onebyte){
	// Wait till SPI Tx ready
	while(!(SPI2->SR & SPI_SR_TXE)){}
		// Send one byte data
		SPI2->DR= onebyte; 
	// Wait till SPI not busy for next job
	while(!(SPI2->SR & SPI_SR_BSY)){} 
}

