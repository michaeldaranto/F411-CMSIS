/*********************************************************
WeAct BlackPill STM32F411CEU6
HSI= 16MHz
SPI2 - Send packet data 
DMA1 Stream4 Channel 0 -  RM0383 page 170
* GPIOB Pin13 -> SCK
* GPIOB Pin14 -> MISO
* GPIOB Pin15 -> MOSI
Michael Daranto - YD3BRB
GPLv3
**********************************************************/
#include "stm32f4xx.h"

uint8_t packet[]={'A',255,0b10101010,0xF0,0x7A,'B','C'};

void RCC_Init(void);
void SPI2_Init(void);
void DMA1_Stream4_Init(void);
void SPI2_DMA1_Tx(uint32_t maddr,uint32_t ndata);

int main (void) {
	
	RCC_Init();
	SPI2_Init();
	DMA1_Stream4_Init();

	while (1) {
	 
	 SPI2_DMA1_Tx((uint32_t)packet,sizeof(packet));
	 
			// Wait till stream enable again	
			while((DMA1_Stream4->CR & DMA_SxCR_EN)){}
			__asm("nop");
			}
	// Return 0 to satisfy compiler
	return 0;
}
void SPI2_Init(void){
	
	// Reset first Pin13,14,15 
	GPIOB->MODER &=~(GPIO_MODER_MODE13|GPIO_MODER_MODE14|GPIO_MODER_MODE15);
	// Set GPIOB Pin13,14,15 AF 
	GPIOB->MODER |=GPIO_MODER_MODE13_1|GPIO_MODER_MODE14_1|GPIO_MODER_MODE15_1;
	// AF05 - SPI2 see datasheet page 48
	GPIOB->AFR[1]|= GPIO_AFRH_AFSEL13_0|GPIO_AFRH_AFSEL13_2| // b0101
					GPIO_AFRH_AFSEL14_0|GPIO_AFRH_AFSEL14_2|
					GPIO_AFRH_AFSEL15_0|GPIO_AFRH_AFSEL15_2;
	
	// Disable NSS, act as Master
	SPI2->CR1 |= SPI_CR1_SSM|SPI_CR1_SSI|SPI_CR1_MSTR;
	// DMA Tx Buffer
	SPI2->CR1|=SPI_CR1_BR_0;
	// DMA ENable
	SPI2->CR2 |=SPI_CR2_TXDMAEN;
	// Enable SPI
	SPI2->CR1 |= SPI_CR1_SPE;
}

void DMA1_Stream4_Init(void){
	
	// Make sure EN=0 
	DMA1_Stream4->CR &=~DMA_SxCR_EN;
	while((DMA1_Stream4->CR) & DMA_SxCR_EN){}	
	// Configuration Register 
	DMA1_Stream4->CR &=~DMA_SxCR_CHSEL;		// CH 0
	DMA1_Stream4->CR = DMA_SxCR_MINC|		// Memory Increment mode
				DMA_SxCR_DIR_0|		// Memory to peripheral	
				DMA_SxCR_TCIE;		// Transfer complete interrupt enable
						
	// Direct mode disabled
	DMA1_Stream4->FCR &=~DMA_SxFCR_DMDIS;
	// Peripheral address
	DMA1_Stream4->PAR= (uint32_t)&SPI2->DR;
	// Enable NVIC
	NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}

void DMA1_Stream4_IRQHandler(void){

	// Clear DMA flag
	DMA1_Stream4->CR &=~DMA_SxCR_EN;
	// Stream 4 clear transfer complete interrupt flag
	DMA1->HIFCR |= DMA_HIFCR_CTCIF4;
	NVIC_ClearPendingIRQ(DMA1_Stream4_IRQn);
}

void SPI2_DMA1_Tx(uint32_t maddr,uint32_t ndata){

				// Clear flag before enable Stream
				DMA1->HIFCR |=DMA_HIFCR_CTCIF4;
				// Memory address				
				DMA1_Stream4->M0AR=maddr;
				// Number of data
				DMA1_Stream4->NDTR=ndata;
				// Stream Enable
				DMA1_Stream4->CR|=DMA_SxCR_EN;
			}
void RCC_Init(void){
	// Turn on the GPIOB peripheral & DMA1 clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_DMA1EN;	
	// Turn on APB1! (SPI2 connect to APB1 - datasheet page 15) 
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN ;
}
