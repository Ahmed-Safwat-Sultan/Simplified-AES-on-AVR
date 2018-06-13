/*
 * UART.c
 *
 *  Created on: Apr 4, 2018
 *      Author: ahmedsafwat
 */

#include <avr/io.h>



void UART_transmitter_init(){
	UCSRB |= (1 << TXEN); //enable transmission mode
	UCSRB |= (1 << UDRIE);// enable interrupt of UDRE flag
	// 8 bit frame ---- one stop bit ---- no parity
	UCSRC = (1 << UCSZ0) | (1 << UCSZ1) | (1 << URSEL);
	UBRRL = 51; // baud rate ~9600
}

void UART_reciever_init(){
	UCSRB |= (1 << RXEN); // Enable receiver mode
	UCSRB |= (1 << RXCIE); // Enable interrupt on RXC flag
	UCSRC = (1 << UCSZ0) | (1 << UCSZ1) | (1 << URSEL); //same
	UBRRL = 51; // baud rate ~9600
}

void UART_init(){
	UCSRB = (1<<RXEN) | (1<<TXEN);//Enable transmitter and receiver
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);//same
	UBRRL = 0x33;
}


void UART_send_byte(unsigned char Data){
	while(!(UCSRA &(1<<UDRE))); // while shift register flag != 1 (not empty))
		UDR = Data;

}


char UART_receive_byte(){
	while( !(UCSRA & (1<<RXC) ));

	return UDR;

}


