/*
 * UART.h
 *
 *  Created on: Apr 4, 2018
 *      Author: ahmedsafwat
 */

#ifndef UART_H_
#define UART_H_

void UART_transmitter_init();
void UART_reciever_init();
void UART_init();
void UART_send_byte(char Data);
char UART_receive_byte();




#endif /* UART_H_ */
