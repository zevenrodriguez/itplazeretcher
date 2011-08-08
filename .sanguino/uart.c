/*
  Serial.c - Serial library for Wiring
  Based on Hernando Barragan's original C implementation
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "buffer.h"
#include "uart.h"

/*******************************************************************************
 UART API
*******************************************************************************/

buffer_t uart0_rxBuffer;
buffer_t uart0_txBuffer;
static char uart0_rxData[UART_BUFFER_LENGTH];
static char uart0_txData[UART_BUFFER_LENGTH];

#if defined(__AVR_ATmega644P__)
buffer_t uart1_rxBuffer;
buffer_t uart1_txBuffer;
char* uart1_rxData;
char* uart1_txData;
#endif

void uart_init(uint8_t uart, uint32_t baudrate)
{
  if(0 == uart){
	//set baud rate
	UBRR0H = ((F_CPU / 16 + baudrate / 2) / baudrate - 1) >> 8;
	UBRR0L = ((F_CPU / 16 + baudrate / 2) / baudrate - 1);
	
    // reset config for UART0
	UCSR0A = 0;
	UCSR0B = 0;
	UCSR0C = 0;
	
	//configure UART0
    UCSR0B = _BV(RXEN0)|_BV(TXEN0)|_BV(RXCIE0)|_BV(TXCIE0);
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);

    // init buffers
    buffer_init(&uart0_rxBuffer, uart0_rxData, sizeof(uart0_rxData));
    buffer_init(&uart0_txBuffer, uart0_txData, sizeof(uart0_txData));
  }
#if defined(__AVR_ATmega644P__)
  else
  {
	//set baud rate
	UBRR1H = ((F_CPU / 16 + baudrate / 2) / baudrate - 1) >> 8;
	UBRR1L = ((F_CPU / 16 + baudrate / 2) / baudrate - 1);

    // reset config for UART1
	UCSR1A = 0;
	UCSR1B = 0;
	UCSR1C = 0;
	
	//configure UART1
    UCSR1B = _BV(RXEN1)|_BV(TXEN1)|_BV(RXCIE1)|_BV(TXCIE1);
	UCSR1C = (1<<UCSZ10) | (1<<UCSZ11);
    
    // allocate and init buffers
    uart1_rxData = (char*) malloc(UART_BUFFER_LENGTH);
    uart1_txData = (char*) malloc(UART_BUFFER_LENGTH);
    buffer_init(&uart1_rxBuffer, uart1_rxData, sizeof(uart1_rxData));
    buffer_init(&uart1_txBuffer, uart1_txData, sizeof(uart1_txData));
  }
#endif
}

int uart_read(uint8_t uart)
{
  if(0 == uart){
    return buffer_get(&uart0_rxBuffer);
  }
#if defined(__AVR_ATmega644P__)
  return buffer_get(&uart1_rxBuffer);
#endif
}

uint8_t uart_available(uint8_t uart)
{
  if(0 == uart){
    return buffer_available(&uart0_rxBuffer);
  }
#if defined(__AVR_ATmega644P__)
  return buffer_available(&uart1_rxBuffer);
#endif
}

void uart_write(uint8_t uart, char *buf, uint8_t len)
{
  uint8_t i;
  uint8_t ints;
  // return if nothing to send
  if (len == 0){
    return;
  }
  // record interrupts
  ints = SREG & 0x80;
  // clear interrupts
  cli();
  // prepare for interrupt handler
  if(0 == uart){
    // append to tx buffer
    for(i = 0; i < len; ++i){
      buffer_put(&uart0_txBuffer, buf[i]);
    }
    // transmit first byte
    UCSR0A |= _BV(TXC0);
    while (bit_is_clear(UCSR0A, UDRE0)){
      continue;
    }
    UDR0 = buffer_get(&uart0_txBuffer);
  }
#if defined(__AVR_ATmega644P__)
  else
  {
    // append to tx buffer
    for(i = 0; i < len; ++i){
      buffer_put(&uart1_txBuffer, buf[i]);
    }
    // transmit first byte
    UCSR1A |= _BV(TXC1);
    while (bit_is_clear(UCSR1A, UDRE1)){
      continue;
    }
    UDR1 = buffer_get(&uart1_txBuffer);
  }
#endif
  // reenable interrupts
  SREG |= ints;
}

void uart_flush(uint8_t uart)
{
  if (0 == uart)
  {
    buffer_flush(&uart0_rxBuffer);
  }
#if defined(__AVR_ATmega644P__)
  else
  {
    buffer_flush(&uart1_rxBuffer);
  }
#endif
}

/*******************************************************************************
 Interrupt Handlers
*******************************************************************************/

// UART0 byte received interrupt handler 
ISR(USART0_RX_vect)
{
  char c;
  // fetch incoming character
  c = UDR0;
  // check for framing error
  if (bit_is_clear(UCSR0A, FE0)) {
    // put character in RX buffer
    buffer_put(&uart0_rxBuffer, c);
  }
}

// UART0 transmit complete interrupt
ISR(USART0_TX_vect)
{
  // return if nothing left to send
  if (0 == buffer_available(&uart0_txBuffer)){
    return;
  }
  // grab character from TX buffer
  UDR0 = buffer_get(&uart0_txBuffer);
}

EMPTY_INTERRUPT(SIG_UART0_TRANS);

#if defined(__AVR_ATmega644P__)

// UART1 byte received interrupt handler
ISR(USART1_RX_vect)
{
  char c;
  // fetch incoming character
  c = UDR1;
  // check for framing error
  if (bit_is_clear(UCSR1A, FE1)) {
    // put character in RX buffer
    buffer_put(&uart1_rxBuffer, c);
  }
}

// UART1 transmit complete interrupt
ISR(USART1_TX_vect)
{
  // return if nothing left to send
  if (0 == buffer_available(&uart1_txBuffer)){
    return;
  }
  // grab character from TX buffer
  UDR1 = buffer_get(&uart1_txBuffer);
}

EMPTY_INTERRUPT(SIG_UART1_TRANS);

#endif
