/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "Gamecube_N64.h"

//================================================================================
// Gamecube/N64 I/O functions
//================================================================================

bool callonce = true;
joybus_port_t _port;

uint8_t gc_n64_send_get(const uint8_t pin, uint8_t* command, const uint8_t commandLen,
    uint8_t* report, const uint8_t reportLen){
    // get the port mask and the pointers to the in/out/mode registers
    //uint8_t bitMask = digitalPinToBitMask(pin);
    //uint8_t port = digitalPinToPort(pin);
    //volatile uint8_t* modePort = portModeRegister(port);
    //volatile uint8_t* outPort = portOutputRegister(port);
    //volatile uint8_t* inPort = portInputRegister(port);

    // don't want interrupts getting in the way
    //uint8_t oldSREG = SREG;
    //cli();

    // send the command
    gc_n64_send(pin, command, commandLen);//, modePort, outPort, bitMask);

    // read in data
    uint8_t receivedBytes = gc_n64_get(pin, report, reportLen, false);//, modePort, outPort, inPort, bitMask);

    // end of time sensitive code
    //SREG = oldSREG;

    // return received length
    return receivedBytes;
}

void gc_n64_send(const uint8_t pin, uint8_t* buff, uint8_t len){
    if (callonce) {
        set_sys_clock_khz(130000, true);
        joybus_port_init(&_port, pin, pio0, -1, -1);
        callonce = false;
    }
    joybus_send_bytes(&_port, buff, len);
    //for (int i = 0; i < (len - 1); i++) {
    //    joybus_send_byte(&_port, buff[i], false);
    //}
    //joybus_send_byte(&_port, buff[len - 1], true);
}

uint8_t gc_n64_get(const uint8_t pin, uint8_t* buff, uint8_t len, bool timeout){
    if (callonce) {
        set_sys_clock_khz(130000, true);
        joybus_port_init(&_port, pin, pio0, -1, -1);
        callonce = false;
    }
    uint8_t receivedBytes = joybus_receive_bytes(&_port, buff, len, 50, timeout);
    return receivedBytes;
}

void gc_n64_reset(void){
    joybus_port_reset(&_port);
}
