int main(void) {
//	int i = 0;
	
	uint16_t data = 0x1234;
	uint32_t address;
	
	//HSI (8 MHz) on
	RCC->CR |= RCC_CR_HSION;
	while (!(RCC->CR & RCC_CR_HSIRDY_Msk)) {}
		
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
	
	GPIOA->MODER |= GPIO_MODER_MODER0_0;
	
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
	////////////////////////////////////////
	
	data = 0;
	
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = data>>8;
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = data>>0;
		
	data = 0x1234;
		
	address = (uint32_t)&data;
	
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = data>>8;
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = data>>0;
			
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = address>>24;
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = address>>16;
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = address>>8;
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = address>>0;
		
	data = 0;
	
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = data>>8;
	while ((USART1->ISR & USART_ISR_TXE) == 0) {}
	USART1->TDR = data>>0;
	
	while (1) {
	
	}

}
