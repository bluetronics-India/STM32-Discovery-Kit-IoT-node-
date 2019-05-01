#include <stm32L475xx.h>
void initUSART1 (void)
{
// Configure PB6 & PB7 for alternate function
	GPIOB->MODER &= !(GPIO_MODER_MODE6 + GPIO_MODER_MODE7); // clear bits bit 12~15
	GPIOB->MODER |= GPIO_MODER_MODE6_1 + GPIO_MODER_MODE7_1; 
// Assign PB6 & PB7 pins to alternate function 7
	GPIOB->AFR[0] &= !(GPIO_AFRL_AFSEL6 + GPIO_AFRL_AFSEL7); // clear bits 24~31
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL6_2 + GPIO_AFRL_AFSEL6_1 + GPIO_AFRL_AFSEL6_0 +
								  GPIO_AFRL_AFSEL7_2 + GPIO_AFRL_AFSEL7_1 + GPIO_AFRL_AFSEL7_0);
	USART1->CR1 &= !(USART_CR1_M1 + USART_CR1_M0); // select 8 data bits
// assume the USART input clock is 80 MHz
	USART1->BRR = 0x2B6; // set baud rate to 115200 baud
	USART1->CR2 &= !(USART_CR2_STOP + USART_CR2_MSBFIRST); // select one stop bit
	USART1->CR3 &= !(USART_CR3_ONEBIT); // select 3 samples
// enable USART1, enable transmitter, receiver	
	USART1->CR1 |= USART_CR1_UE + USART_CR1_TE + USART_CR1_RE; 
}
void putchUSART1(char cx)
{
	while(!(USART1->ISR & USART_ISR_TXE)); 
	USART1->TDR = (uint16_t)cx; 
}
void newlineUSART1 (void)
{
	putchUSART1(0x0D);
	putchUSART1(0x0A); 
}
void putsUSART1(char *ptr)
{
	while(*ptr){
		putchUSART1(*ptr++);
	}
}
char getchUSART1(void)
{
	while(!(USART1->ISR & USART_ISR_RXNE));
	return((char)USART1->RDR);
}
#define	ENTER	0x0D	// ASCII code of the ENTER character
#define	BS		0x08	// ASCII code of the BACKSPACE character
#define	SPACE	0x20	// ASCII code of the SPACE character
void getsUSART1 (char *ptr)
{
	char	cx;
	while((cx = getchUSART1()) != ENTER) { // received an ENTER character?
		*ptr = cx;
		putchUSART1(cx);	// echo the received character
		if(cx == BS) { // received a backspace character?
			putchUSART1(SPACE); // clear the character to be backspaced
			putchUSART1(BS);
			ptr--;
		} 
		else ptr++;
	}
	*ptr = 0;		// terminate the string with a NULL character
}
void int2ASCII (int ix, char *ptr)
{
	char 	sign, *p;
	uint16_t	len, j;
	sign		= '+';
	len		= 0;
	p		= ptr;
	if(ix < 0) { // find the magnitude for a negative number
		sign 	= '-';
		ix 	= -ix;
	}
	do { // convert every decimal digit into ASCII code
		*ptr		= (ix % 10) + '0';
		ptr++;
		len++;
		ix		/= 10;
	} while (ix != 0);
	if(sign == '-') {
		*ptr = '-';
		ptr++;
		len++;
	}
	for (j = 0; j < len/2; j++) { // reverse the string by swapping
		p[len]	= p[j];	// p[len] is a temporary storage
		p[j]		= p[len - j - 1];
		p[len -j - 1] = p[len];
	}
	p[len] = 0; 	// terminate the string with a NULL character
}
int ASCII2Int (char *ptr)
{
	int 	ix = 0;
	char		sign = '+';
	if(*ptr == '-') {
		sign = '-';
		ptr++;	// move to the first decimal digit
	}
	while (*ptr != 0) {
		ix = ix * 10 + *ptr - '0';
		ptr++;
	}
	if (sign == '-')
		ix	= - ix;
	return ix;
}
void USART1_puti (int ix) // output an integer to USART1
{
	char  sarr[12];
	int2ASCII (ix, sarr);
	putsUSART1 (sarr);
}

int USART1_geti (void)
{
	char sarr[12];
	getsUSART1(sarr);
	return (ASCII2Int (sarr));
}

void USART1_putf(float f, int p) {
	int u = (int)f; 
	float l = (f - u); 
	int i = 0; 
	while (i < p) { 
		l *= 10; 
		i++; 
	} 
	if (u < 0) { 
		putchUSART1('-'); 
		u*=-1; 
		l*=-1; 
	} 
	USART1_puti(u); 
	if (p != 0) { 
		putchUSART1('.'); 
		USART1_puti(l); 
	} 
}




