#include <stdio.h>
#include <stm32f0xx.h>
#include <stm32f051x8.h>
#include <GPIO.h>
#include <DS18B20.h>

void delay_mks(uint32_t  mks){

	SysTick->LOAD = 1*mks;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x1;
	while (!((SysTick->CTRL) & SysTick_CTRL_COUNTFLAG_Msk)){};
	SysTick->CTRL = 0;
}

int main(void) {
//	int i = 0;
	
//	uint16_t data = 0x1234;
//	uint32_t address;
	
	//HSI (8 MHz) on
	RCC->CR |= RCC_CR_HSION;
	while (!(RCC->CR & RCC_CR_HSIRDY_Msk)) {}
		
	RCC->CFGR &= ~RCC_CFGR_SW_HSI;
		
	//configuring prescelers
	//AHB = 1, SYSCLK not  divided
	RCC->CFGR &=~ RCC_CFGR_HPRE_Pos;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	//APB1 = 1, HCLK not divided
	RCC->CFGR &=~ RCC_CFGR_PPRE_Pos;
	RCC->CFGR |= RCC_CFGR_PPRE_DIV1;
		
	//PLL output selected as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	// PLL configuration
	RCC->CFGR |= RCC_CFGR_PLLMUL12; //PLL input clock x 12
	RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV1; //PREDIV input clock divided by 1
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2; //HSI clock divided by 2 selected as PLL entry clock source
	//final frequency - 48 MHz
	RCC->CR |= RCC_CR_PLLON;
	
	FLASH->ACR = 1;	
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	//PA0 - button
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1; //without PULL_DOWN button works only ones
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR2_0;
	
	//////////////////////////////////////////////////////
				
	//Enable clocking UART1 (48 MHz)
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 
	
	// PA9 - USART1-TX, PB10 - USART1-RX
	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR9_0;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9_0;
	GPIOA->MODER |= GPIO_MODER_MODER10_1;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_10;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR10_0;
	
	//choosing of alternate function for GPIO for USART
	GPIOA->AFR[1] |= (0x1UL << GPIO_AFRH_AFSEL9_Pos);
	GPIOA->AFR[1] |= (0x1UL << GPIO_AFRH_AFSEL10_Pos);
	//GPIOB->AFR[1] = 0x00000110
	
	//USART1 congiguration
//	USART1->BRR = 0x9C2; //baud rate 38400 (PLL)
	USART1->BRR = 0x9C2; //baud rate 9600 (PLL)
	
	USART1->CR2 = 0;
	USART1->CR3 = 0;
	USART1->CR1 |= USART_CR1_OVER8; //Oversampling by 8-bit
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE ; // enable receiver and transmitter
	
	USART1->CR1 |= USART_CR1_UE; //USART enable
	
	__NVIC_EnableIRQ(USART1_IRQn); // enable button interrupt
	////////////////////////////////////////
	
	////////////////////////////////////////
	
	//Button interruption (it really works)
//	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //enable selection of source for the exti interruption
	//interrupt setting for button
	EXTI->IMR = 0;
	EXTI->IMR |= EXTI_IMR_MR0;// Configure the corresponding mask bit in the EXTI_IMR register
	EXTI->RTSR |= EXTI_RTSR_TR0;//Configure the Trigger Selection bits of the Interrupt line on rising edge
	EXTI->FTSR &= ~EXTI_FTSR_TR0;//Configure the Trigger Selection bits of the Interrupt line NOT on falling edge
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;  ////unnecessary coz default value is 0000
	
	__NVIC_EnableIRQ(EXTI0_1_IRQn); // enable button interrupt
	///////////////////////////////////////
	
	delay_mks(1000000);
	USART1->TDR = 0x00;
	
	while (1) {
	
	}

}

void EXTI0_1_IRQHandler (void){ //button interrupt
	
	USART1->CR1 |= USART_CR1_TXEIE; //start USART transmission 
	
	EXTI->PR = EXTI_PR_PR0;
	
	__NVIC_ClearPendingIRQ(EXTI0_1_IRQn);//exit from button interrupt
	
	
}

void USART1_IRQHandler(void){
	
	USART1->ISR |= USART_ISR_TXE;
	
	USART1->TDR = 0x7A;
	delay_mks(1000000);
	
	__NVIC_ClearPendingIRQ(USART1_IRQn);//exit from USART interrupt
	
}


