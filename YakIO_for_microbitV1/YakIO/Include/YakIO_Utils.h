/// +------------------------------------------------------------------------------------------------------------------------------+
/// ¦                                                   TERMS OF USE: MIT License                                                  ¦
/// +------------------------------------------------------------------------------------------------------------------------------¦
/// ¦Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation    ¦
/// ¦files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,    ¦
/// ¦modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software¦
/// ¦is furnished to do so, subject to the following conditions:                                                                   ¦
/// ¦                                                                                                                              ¦
/// ¦The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.¦
/// ¦                                                                                                                              ¦
/// ¦THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE          ¦
/// ¦WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR         ¦
/// ¦COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,   ¦
/// ¦ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                         ¦
/// +------------------------------------------------------------------------------------------------------------------------------+

#ifndef YAKIO_UTILS_H
#define YAKIO_UTILS_H

#include "YakIO.h"
#include "YakIO_NVIC.h"

// NOTE: these are all non-member C++ functions

void EnableIRQ(int irqNum);
void DisableIRQ(int irqNum);
void ClearPendingIRQ(int irqNum);

// A note on DELAYS. Software "loop style" delays are tricky to time. The big problem
// is that C++ function calls with parameters are slow (lots of state needs to be saved 
// onto the stack etc.
//
// Thus any looping routine which accepts a variable as the delay time is usually
// swamped by the overhead of the call itself. In order to support user defined milli-second 
// delays we have the macro below. Just use it like you would a function. 
//
// Example: for a 15 millisecond delay just use:
//      DELAY_MILLI_SEC(15);
// Since there are no function calls in the delay loop things are much more consistent
// 
// Also note, having said all that, "loop style" delays should not be considered accurate.
// Adding in any other code (ie a function call) between calls to the delay will significantly
// skew the timings. If you need acccurate triggers of an event, use a timer and and an interrupt.

// the 1333 value in the macro below is just a trial and error thing that makes it work out to 1 milliSec at 16MHz
// the "asm" keyword just directly inserts assembler into the C++ code. In this case we are inserting a no-op 
// instruction. The "volatile" just tells the compiler not to remove this loop even if it looks like it isn't doing
// anything. 
// See: https://developer.arm.com/documentation/dui0375/g/Using-the-Inline-and-Embedded-Assemblers-of-the-ARM-Compiler/Inline-assembly-language-syntax-with-the---asm-keyword-in-C-and-C--
#define DELAY_MILLI_SEC(msToDelay)        for(unsigned int iDELAYMS =0; iDELAYMS<(1333*msToDelay); iDELAYMS++) asm volatile ("nop")
#endif
