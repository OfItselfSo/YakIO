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

#ifndef YAKIO_TIMER_H
#define YAKIO_TIMER_H

#include "YakIO_CALLBACK.h"
#include "YakIO_NVIC.h"
#include "YakIO_Utils.h"

// TIMER REGISTER SPECIFIC SECTION
#define TIMERREG_OFFSET_START        0x000 // Start Timer
#define TIMERREG_OFFSET_STOP         0x004 // Stop Timer
#define TIMERREG_OFFSET_COUNT        0x008 // Increment Timer (Counter mode only)
#define TIMERREG_OFFSET_CLEAR        0x00C // Clear time
#define TIMERREG_OFFSET_SHUTDOWN     0x010 // Shut down timer
#define TIMERREG_OFFSET_CAPTURE_0    0x040 // Capture Timer value to CC_0 register
#define TIMERREG_OFFSET_CAPTURE_1    0x044 // Capture Timer value to CC_1 register
#define TIMERREG_OFFSET_CAPTURE_2    0x048 // Capture Timer value to CC_2 register
#define TIMERREG_OFFSET_CAPTURE_3    0x04C // Capture Timer value to CC_3 register
#define TIMERREG_OFFSET_COMPARE_0    0x140 // Compare event on CC_0 match
#define TIMERREG_OFFSET_COMPARE_1    0x144 // Compare event on CC_1 match
#define TIMERREG_OFFSET_COMPARE_2    0x148 // Compare event on CC_2 match
#define TIMERREG_OFFSET_COMPARE_3    0x14C // Compare event on CC_3 match
#define TIMERREG_OFFSET_SHORTS       0x200 // Shortcut register
#define TIMERREG_OFFSET_INTENSET     0x304 // Enable interrupt
#define TIMERREG_OFFSET_INTENCLR     0x308 // Disable interrupt
#define TIMERREG_OFFSET_MODE         0x504 // Timer mode selection
#define TIMERREG_OFFSET_BITMODE      0x508 // Configure the number of bits used by the TIMER
#define TIMERREG_OFFSET_PRESCALER    0x510 // Timer prescaler register
#define TIMERREG_OFFSET_CC_0         0x540 // Capture/Compare register 0
#define TIMERREG_OFFSET_CC_1         0x544 // Capture/Compare register 1
#define TIMERREG_OFFSET_CC_2         0x548 // Capture/Compare register 2
#define TIMERREG_OFFSET_CC_3         0x54C // Capture/Compare register 3

#define TIMER_SHORT_COMPARE0_CLEAR 0b000000000001
#define TIMER_SHORT_COMPARE1_CLEAR 0b000000000010
#define TIMER_SHORT_COMPARE2_CLEAR 0b000000000100
#define TIMER_SHORT_COMPARE3_CLEAR 0b000000001000
#define TIMER_SHORT_COMPARE0_STOP  0b000100000000
#define TIMER_SHORT_COMPARE1_STOP  0b001000000000
#define TIMER_SHORT_COMPARE2_STOP  0b010000000000
#define TIMER_SHORT_COMPARE3_STOP  0b100000000000

#define TIMER_INTEN_COMPARE0_BIT  0b000000010000000000000000
#define TIMER_INTEN_COMPARE1_BIT  0b000000100000000000000000
#define TIMER_INTEN_COMPARE2_BIT  0b000001000000000000000000
#define TIMER_INTEN_COMPARE3_BIT  0b000010000000000000000000

// the Timers on this system
// note the values here are just ID's 1,2,3
enum TIMER {
    Timer0=0x00,
    Timer1=0x01,
    Timer2=0x02
};

// note the value here is carefully set to the value we have to 
// stuff in the register to set the bit mode properly
enum TIMER_BITMODE {
    TIMER_BITMODE_16Bit=0,     // 16 bit timer bit width
    TIMER_BITMODE_08Bit=1,     // 8 bit timer bit width
    TIMER_BITMODE_24Bit=2,     // 24 bit timer bit width
    TIMER_BITMODE_32Bit=3      // 32 bit timer bit width
};

// note the value here is carefully set to the value we have to 
// stuff in the register to set the bit mode properly
enum TIMER_MODE {
    TIMER_MODE_Timer=0,      // Select Timer mode
    TIMER_MODE_Counter=1     // Select Counter mode
};

// the CC[0,1,2,3], COMPARE[0,1,2,3] etc registers in a timer do not
// work in an intuitive way. You would think that you set up to 4 counters,
// get an interrupt for each one and figure out which counter sent it in
// the handler. This is _not_ the way it works. Accepting the Interrupt by
// zeroing the appropriate COMPARE resets the clock for all the timers thus
// you only ever get interrupts from the lowest timer set.
//
// Not at all sure what use they are really. In this class we only ever use
// CC0 and COMPARE0 and ignore the others. The user has no idea of this they
// just set a single count and get the interrupt
//
// See the link below: 
//    https://devzone.nordicsemi.com/f/nordic-q-a/18237/timer-with-two-compared-values


/* YakIO_TIMER - a class to represent and encapsulate Timer
 *     information and actions.
 * 
 * */
class YakIO_TIMER
{ 
  private: 
      unsigned char isInitialized=0;
      enum TIMER timerID;
      YakIO_CALLBACK *callbackInterfacePtr =0;
      enum CALLBACK_ID callbackID = CALLBACK_NONE;
      unsigned int timerRegisterAddress=0;
      void ResetAllShorts();
      void ResetAllINTENs(void);

  public:
      // Constructor to initialize YakIO_TIMER object
      YakIO_TIMER(enum TIMER timerIDIn);
      void TimerShutdown(void);
      void QuickSetup(unsigned int precalerValue, unsigned int countLevelValue, enum CALLBACK_ID callbackIDIn, YakIO_CALLBACK *callbackInterfacePtrIn);
      void QuickSetup(unsigned int precalerValue, unsigned int countLevelValue, enum CALLBACK_ID callbackIDIn, YakIO_CALLBACK *callbackInterfacePtrIn, unsigned int wantStart);
      void SetCallback(enum CALLBACK_ID callbackIDIn, YakIO_CALLBACK *callbackInterfacePtrIn);
      void CallCallback();
      void SetTimer1(void);
      void SetPrescaler(unsigned int precalerValue);
      unsigned int GetPrescaler(void);
      void SetCountLevel(unsigned int countLevelValue);
      unsigned int GetCountLevel(void);
      void TimerStart(void);
      void TimerStop(void);
      void TimerClear(void);
      void SetBitMode(enum TIMER_BITMODE bitModeValue);
      enum TIMER_BITMODE GetBitMode(void);
      void SetMode(enum TIMER_MODE modeValue);
      enum TIMER_MODE GetMode(void);
      unsigned int GetShortCut(void);
      void SetShortCut();
      void SetINTEN();
      void ClearINTEN();
      void ClearCompareEvent(void);
      void ClearAllCallbacks(void);
      void ClearCallbackByID(enum CALLBACK_ID callbackIDIn);
      void EnableTimerIRQ(void);
      void DisableTimerIRQ(void);

};























#endif
