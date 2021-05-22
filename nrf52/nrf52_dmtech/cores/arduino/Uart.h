/*
  Copyright (c) 2016 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _UART_CLASS
#define _UART_CLASS

#include "HardwareSerial.h"
#include "RingBuffer.h"
#include "nrf_uarte.h"

#include <cstddef>


class Uart : public HardwareSerial
{
	public:
		Uart(uint8_t _pinRX, uint8_t _pinTX);
		void begin(unsigned long baudRate);
		void begin(unsigned long baudrate, uint8_t config);
		void end();
		int available();
		int peek();
		int read();
		void flush();
		size_t write(const uint8_t data);
		size_t write(const char * data);

		void IrqHandler();

		operator bool() { return true; }

	private:
		RingBuffer rxBuffer;
    
    uint8_t uc_pinRX;
    uint8_t uc_pinTX;

		nrf_uarte_baudrate_t adaptBaudrate(unsigned long baudRate);
		nrf_uarte_parity_t extractParity(uint8_t config);
};

extern Uart Serial;

#endif
