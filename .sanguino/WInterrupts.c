/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Wiring project - http://wiring.uniandes.edu.co

  Copyright (c) 2004-05 Hernando Barragan

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
  
  Modified 24 November 2006 by David A. Mellis
*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "WConstants.h"
#include "wiring_private.h"

volatile static voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS];

#define MCUCR EICRA
#define GICR EIMSK

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode)
{
	if(interruptNum < EXTERNAL_NUM_INTERRUPTS)
	{
		intFunc[interruptNum] = userFunc;

		//clear the config for the change settings
		MCUCR &= ~(B00000011 << (interruptNum * 2));

		//set our mode.
		MCUCR |= (mode << (interruptNum * 2));

		// Enable the interrupt.
		GICR |= (1 << interruptNum);
	}
}

void detachInterrupt(uint8_t interruptNum)
{
	if(interruptNum < EXTERNAL_NUM_INTERRUPTS)
	{
		// Disable the interrupt.
		GICR &= ~(1 << interruptNum);

		intFunc[interruptNum] = 0;
	}
}

SIGNAL(SIG_INTERRUPT0) {
  if(intFunc[EXTERNAL_INT_0])
    intFunc[EXTERNAL_INT_0]();
}

SIGNAL(SIG_INTERRUPT1) {
  if(intFunc[EXTERNAL_INT_1])
    intFunc[EXTERNAL_INT_1]();
}

SIGNAL(SIG_INTERRUPT2) {
  if(intFunc[EXTERNAL_INT_2])
    intFunc[EXTERNAL_INT_2]();
}
