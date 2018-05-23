/*
Circular LED library V1.0
2010 Copyright (c) Seeed Technology Inc. All right reserved.

Original Author: LG

Modify: Mihail Stoyanov (mihail.stoyanov@arm.com) for ARM mbed, 2014-07-30
Modify: Mo Chen (mo.chen@arm.com) for ARM mbed, 2017-06-16
User can choose which Io to be used.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "Circular_LED.h"

Circular_LED::Circular_LED(PinName pinClk, PinName pinDta) : __pinClk(pinClk), __pinDta(pinDta)
{
    __led_state = 0x00;
}

void Circular_LED::latchData()
{
    __pinDta = 0;
    wait_us(10);

    for(int i = 0; i < 4; i++) {
        __pinDta = 1;
        __pinDta = 0;
    }

}

void Circular_LED::send16bitData(uint16_t data)
{
    for(int i = 0; i < 16; i++) {
        unsigned int state = data & 0x8000 ? 1 : 0;
        __pinDta = state;

        __pinClk = !__pinClk;
        // state = __pinClk ? 0 : 1;
        // __pinClk = state;

        data <<= 1;
    }
}

void Circular_LED::setState(uint32_t state)
{
    uint16_t st;
    
    __led_state = state;
    send16bitData(CMDMODE);

    for (int i = 0; i < 12; i++) {
        st = (state & 0x0001) ? ON : SHUT;
        send16bitData(st);
        state = state>>1;
    }
    
    send16bitData(CMDMODE);
    
    for (int i = 12; i < 24; i++) {
        st = (state & 0x0001) ? ON : SHUT;
        send16bitData(st);
        state = state >> 1;
    }

    latchData();
}

void Circular_LED::setLevel(uint8_t level)
{

    if (level > 24) {
        return;
    }
    
    for (int i = 0; i < 24; i++) {
        // Set state bit for each LED
        bool st = (i < level);
        __led_state = st ? (__led_state | (0x01 << i)) : (__led_state & ~(0x01 << i));
    }

    setState(__led_state);
}

void Circular_LED::setSingleLed(uint8_t num, bool st)
{
    if (num > 23) {
        return;
    }

    __led_state = st ? (__led_state | (0x01<<num)) : (__led_state & ~(0x01<<num));
    setState(__led_state);
}
