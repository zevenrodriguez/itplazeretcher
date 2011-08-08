/*
  buffer.c - Buffer library for Wiring & Arduino
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

#include <inttypes.h>

#include "buffer.h"

/*******************************************************************************
 Buffer Routines
*******************************************************************************/

void buffer_init(buffer_t* b, char* data, uint16_t length)
{
  b->len = length;
  b->cnt = 0;
  b->in  = data;
  b->out = data;
  b->buf = data;
}

void buffer_put(buffer_t* b, char c)
{
  // return if full
  if (b->cnt >= b->len) {
    return;
  }
  // place in buffer
  *b->in++ = c;
  b->cnt++;
  // wrap around to beginning, if needed
  if (b->in >= b->buf + b->len) {
    b->in = b->buf;
  }
}

uint16_t buffer_get(buffer_t* b)
{
  char c;
  // return -1 if empty
  if (b->cnt == 0) {
    return -1;
  }
  // pop char off buffer
  c = *b->out++;
  b->cnt--;
  // wrap around to beginning, if needed
  if (b->out >= b->buf + b->len) {
    b->out = b->buf;
  }
  // return popped char
  return c;
}

uint8_t buffer_available(buffer_t* b)
{
  return b->cnt; 
}


uint8_t buffer_look(buffer_t* b)
{
  // return if empty
  if (b->cnt == 0) {
    return 0;
  }
  return *b->out;
}

uint8_t buffer_find(buffer_t* b, char c)
{
  char* p;
  uint8_t count;
  // return if empty
  if (b->cnt == 0) {
    return 0;
  }
  // non-empty, start search
  count = 0;
  p = b->out;
  while (p != b->in) {
    // return char if found
    if (*p == c){
      return c;
    }
    // keep looking
    count++;
    p++;
    // return 0 if end is reached
    if (count >= b->cnt) {
      return 0;
    }
    // wrap around to beginning, if needed
    if (p > b->buf + b->len) {
      p = b->buf;
    }
  }
  return 0;
}

void buffer_flush(buffer_t* b)
{
  b->cnt = 0;
  b->in  = b->buf;
  b->out = b->buf;
}


