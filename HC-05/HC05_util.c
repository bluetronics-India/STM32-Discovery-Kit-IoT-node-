#include "HC05_util.h"
#include "stm32L475xx.h"

void HC05_putch(char cx)
{
	while(!(UART4->ISR & USART_ISR_TXE));
	UART4->TDR = (uint16_t)cx; 
}
void HC05_newline(void)
{
	HC05_putch(0x0D);
	HC05_putch(0x0A); 
}
void HC05_puts(char *ptr)
{
	while(*ptr){
		HC05_putch(*ptr++);
	}
}
char HC05_getch(void)
{
	while(!(UART4->ISR & USART_ISR_RXNE));
	return((char)UART4->RDR);
}

#define	ENTER	0x0D	// ASCII code of the ENTER character
#define	BS		0x08	// ASCII code of the BACKSPACE character
#define	SPACE	0x20	// ASCII code of the SPACE character

void HC05_gets (char *ptr)
{
	char	cx;
	while((cx = HC05_getch()) != ENTER) { // received an ENTER character?
		*ptr = cx;
		HC05_putch(cx);	// echo the received character
		if(cx == BS) { // received a backspace character?
			HC05_putch(SPACE); // clear the character to be backspaced
			HC05_putch(BS);
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
void HC05_puti(int ix) // output an integer to USART1
{
	char  sarr[12];
	int2ASCII (ix, sarr);
	HC05_puts (sarr);
}

int HC05_geti (void)
{
	char sarr[12];
	HC05_gets(sarr);
	return (ASCII2Int (sarr));
}

void HC05_putf(float f, int p) {
	int u = (int)f;
	float l = (f - u);
	int i = 0;
	while (i < p) {
		l *= 10;
		i++;
	}
	if (u < 0) {
		HC05_putch('-');
		u*=-1;
		l*=-1;
	}
	HC05_puti(u);
	if (p != 0) {
		HC05_putch('.');
		HC05_puti(l);
	}
}
