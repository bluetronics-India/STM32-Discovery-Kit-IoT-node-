#ifndef HC05_UTIL_H
#define HC05_UTIL_H
#include "stm32L475xx.h"
void HC05_putch(char cx);
void HC05_newline(void);
void HC05_puts(char *ptr);
char HC05_getch(void);
void HC05_gets(char *ptr);
void int2ASCII(int ix, char *ptr);
int ASCII2Int(char *ptr);
void HC05_puti(int ix);
int HC05_geti(void);
void HC05_putf(float f, int p);
#endif
