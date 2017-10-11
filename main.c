/*
 * GPS-UART-BUF.c
 *
 * Created: 9/23/2017 5:37:46 PM
 * Author : madiv
 */ 
#define F_CPU 1000000UL 
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "AT-SMS.h"

#define FOSC 2000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR (((FOSC / (BAUD * 16UL))) - 1)

static FILE uart0_output = FDEV_SETUP_STREAM(USART0_Transmit, NULL, _FDEV_SETUP_WRITE);
static FILE uart1_output = FDEV_SETUP_STREAM(USART1_Transmit, NULL, _FDEV_SETUP_WRITE);
//static FILE uart1_input = FDEV_SETUP_STREAM(NULL, USART1_Receive, _FDEV_SETUP_READ);
static FILE uart0_input = FDEV_SETUP_STREAM(NULL, USART0_Receive, _FDEV_SETUP_READ);

int CheckSMS();
void checkOKstatus();

int a; int i; char w; int y;
char input;
char buff[250];
int main( void )
{
	USART1_Init(MYUBRR);
	USART0_Init(MYUBRR);
	DDRB |= (1<<DDB1); //set PORTB1 as output
	
	
 	stdin = &uart0_input;
 	stdout = &uart0_output;
//	sei();
	
	_delay_ms(1000);
/*	printf("\r\n STARTING SYSTEM \r\n");*/

	while(1) 
	{
		CheckSMS();
//		int w = a;
		fdev_close();
		stdout = &uart1_output;
		stdin = &uart0_input;
		printf("\r\nPrinting buffer\n");
		printf("\r\nValue of Buffer [0] = %d", buff[0]);
		i=0;
// 		while (i<250)
// 		{
// 			printf("%c", buff[i]);
// 			i++;
// 		}
		fdev_close();
		stdout = &uart0_output;
		stdin = &uart0_input;
		_delay_ms(2000);
	}
	return 0;
}

int CheckSMS()
{
	//char w; 
	y=0;
	a=0;
	printf("AT\r\n");
	checkOKstatus();
	
	printf("AT+CMGF=1\r\n");
	checkOKstatus();
	
	printf("AT+CMGL=\"REC UNREAD\"\r\n");
//	w = getchar();
// 	while( a < 2) // if 2*
// 	{
// 		w = getchar();
// 		buff[y] = w;
// 		y++;
// 		if (w == 0x02A) // if w = *
// 		{a++;}
//  		else{}
// 	}
// 	while (w!=0x04F) // if w = O
// 	{
// 		w = getchar();
// 		buff[y] = w;
// 		y++;
// 	}
// 	while (w!=0x04B) // if w = K
// 	{
// 		w = getchar();
// 		buff[y] = w;
// 		y++;
// 	}
	while (a < 2) //skip the <LF>
	{
		w = getchar();
		if (w==0x0A)
		{ a++; } 
		else
		{}
	}
	w = getchar();
	
	if (w==0x02B) // if w = +
	{	
		w = getchar();
		while (w !=  0x0A) //w is not <LF> 
		{ w = getchar();}
			
		w = getchar();
		if (w == 0x030)//w is '0' 
		{ buff[0] = 1;  } 
		else if(w == 0x031)//w is '1' 
		{ buff[0] = 2;  } 
		else{buff[0] = 9;}
	} 
	else if(w==0x04F) // if w = 'O'
	{
		w = getchar();
		if (w==0x04B) // if w = 'K'
		{ buff[0] = 3; } 
		else
		{ buff[0] = 4; }
	}
	else
	{buff[0] = 5;}
// 	printf("AT+CMGF=1\r\n");
// 	printf("AT+CMGD=1,4\r\n"); //clearing all SMS in storage AREA
	return *buff;
}

void checkOKstatus()
{
		w = getchar();
		while (w!=0x04F) // if w = O
		{	w = getchar();	}
			
		while (w!=0x04B) // if w = K
		{	w = getchar();	}
}
