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

#include "YakIO.h"
#include "YakIO_TIMER.h"

// the IRQ_TIMER?_handler's are non-member functions. They have no idea of 
// what class they should work on. The pointers below are set in the 
// constructor of a Timer Object. 
// 
// This allows the timer IRQ functions to find the proper timer object. 
// For example if the IRQ_TIMER0_handler() is called it is hard coded to 
// look at timer_ptr0 to find the class it should work with. 
//
// These values are exclusively local in scope and should not be messed with by 
// external operations, or by anything really.
// 
// NOTE: also see the discussion of the IRQ_TIMER?_handlers in the comments on
// those functions for additional information

YakIO_TIMER *timer_ptr0 = NULL;
YakIO_TIMER *timer_ptr1 = NULL;
YakIO_TIMER *timer_ptr2 = NULL;

    /* Constructor - initializes the object
     * 
     * inputs:
     *    timerIDIn - the timer that this object represents
     * */
    YakIO_TIMER::YakIO_TIMER(enum TIMER timerIDIn)
    {
        isInitialized =1;
        
        // remember this now
        timerID = timerIDIn;
        
        // set the pointer to this instance so the interrupt
        // handlers can see it and find the right timer object
        if(timerIDIn==Timer1) timer_ptr1 = this;
        else if(timerIDIn==Timer2) timer_ptr2 = this;
        else timer_ptr0 = this;
        
        // set the address of the base register for this timer
        if(timerIDIn==Timer1) timerRegisterAddress = REGISTER_TIMER1;
        else if(timerIDIn==Timer2) timerRegisterAddress = REGISTER_TIMER2;
        else timerRegisterAddress = REGISTER_TIMER1;
        
        // make sure the timer is stopped
        TimerStop();
        // clear down the shorts register
        ResetAllShorts();
        // make sure we trigger no interrupts
        ResetAllINTENs();        
    }
    
    /* ResetAllShorts - reset all relevant bits in the SHORTS register
     * 
     * */
    void YakIO_TIMER::ResetAllShorts(void)
    {
        // the bits we need to clear are the bitwise NOT of this
        unsigned int bitVal = TIMER_SHORT_COMPARE0_CLEAR | TIMER_SHORT_COMPARE1_CLEAR |
                               TIMER_SHORT_COMPARE2_CLEAR | TIMER_SHORT_COMPARE3_CLEAR |
                               TIMER_SHORT_COMPARE0_STOP | TIMER_SHORT_COMPARE1_STOP |
                               TIMER_SHORT_COMPARE2_STOP | TIMER_SHORT_COMPARE3_STOP;
                               
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_SHORTS)) = (~bitVal);
    }
    
    /* SetShortCut - sets the TIMER_SHORT_COMPARE0_CLEAR in the SHORTS register
     *    So that the the COMPARE0 counter gets cleared every time we trigger
     *    an interrupt
     * 
     *    We only ever use COMPARE0. Using multiple compare registers does not work
     *    the way you probably think they should. See the link below for more info
     *    https://devzone.nordicsemi.com/f/nordic-q-a/18237/timer-with-two-compared-values
     * 
     * */
    void YakIO_TIMER::SetShortCut()
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_SHORTS)) |= TIMER_SHORT_COMPARE0_CLEAR;            
    }    
    
    /* GetShortCut - gets the TIMER_SHORT_COMPARE0_CLEAR value of the SHORTS register
     * 
     * returns
     *        returns 1 if the shortcut is set or 0 if not 
     * */
    unsigned int YakIO_TIMER::GetShortCut(void)
    {
        // we must be initialized
        if(isInitialized==0) return 0;
        
        // get the value from the appropriate register
        unsigned int bitVal = (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_SHORTS));
        
        bitVal &= TIMER_SHORT_COMPARE0_CLEAR;
        if(bitVal!=0) return 1;
        else return 0;       
    }        
    
    /* ResetAllINTENs - reset all relevant bits in the INTEN registers
     * 
     * */
    void YakIO_TIMER::ResetAllINTENs(void)
    {
        // we use a CLR register so we can just set these bits directly
        unsigned int bitVal = TIMER_INTEN_COMPARE0_BIT | TIMER_INTEN_COMPARE1_BIT |
                               TIMER_INTEN_COMPARE2_BIT | TIMER_INTEN_COMPARE3_BIT;

                               
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_INTENCLR)) = bitVal;
    }
    
    /* SetINTEN - sets the TIMER_INTEN_COMPARE0_BIT in the INTENSET register
     *    So as to enable an interrupt every time our count matches the COMPARE0 counter
     * 
     *    We only ever use COMPARE0. Using multiple compare registers does not work
     *    the way you probably think they should. See the link below for more info
     *    https://devzone.nordicsemi.com/f/nordic-q-a/18237/timer-with-two-compared-values
     * 
     * */
    void YakIO_TIMER::SetINTEN()
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_INTENSET)) |= TIMER_INTEN_COMPARE0_BIT;            
    }        
    
    /* ClearINTEN - Clears the TIMER_INTEN_COMPARE0_BIT in the INTENCLR register
     *    So as to enable an interrupt every time our count matches the COMPARE0 counter
     * 
     *    We only ever use COMPARE0. Using multiple compare registers does not work
     *    the way you probably think they should. See the link below for more info
     *    https://devzone.nordicsemi.com/f/nordic-q-a/18237/timer-with-two-compared-values
     * 
     * */
    void YakIO_TIMER::ClearINTEN()
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // we use a CLR register so we can just set these bits directly
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_INTENCLR)) |= TIMER_INTEN_COMPARE0_BIT;            
    }        
    
    /* SetCallback - sets the callback object and function within that object. 
     *     The callback object must inherit from YakIO_CALLBACK
     * 
     * inputs:
     *    callbackInterfacePtrIn - the "this" pointer of the object to receive
     *       the callback
     *    callbackIDIn - the callback id to use. Essentially this identifies the function name within the
     *       callback interface object
     * 
     * */
    void YakIO_TIMER::SetCallback(enum CALLBACK_ID callbackIDIn, YakIO_CALLBACK *callbackInterfacePtrIn)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the callback interface pointer, this is the "this" pointer
        // of the objects that wants to receive the callback
        callbackInterfacePtr = callbackInterfacePtrIn;
        // set the callbackID
        callbackID = callbackIDIn;
    }
    
    /* CallCallback - calls the callback function set on this object
     * 
     * */
    void YakIO_TIMER::CallCallback()
    {
        if(isInitialized==0) return;
        // we have to have this
        if(callbackInterfacePtr==NULL) return;
        
        // figure out what callback function to call and call it
        if(callbackID == CALLBACK_0) callbackInterfacePtr->Callback0();
        else if(callbackID == CALLBACK_1) callbackInterfacePtr->Callback1();
        else if(callbackID == CALLBACK_2) callbackInterfacePtr->Callback1();
        else if(callbackID == CALLBACK_3) callbackInterfacePtr->Callback1();
        else if(callbackID == HEARTBEAT) callbackInterfacePtr->Heartbeat();        
    }
        
    /* ClearAllCallbacks - clear all callbacks
     * 
     * */
    void YakIO_TIMER::ClearAllCallbacks(void)
    {
        // run through our list of callbacks
        // we only have one at the moment so this will do it
        callbackInterfacePtr=NULL;
        callbackID=CALLBACK_NONE;
    }
    
    /* ClearCallbackByID - clear a callback by ID
     * 
     * inputs:
     *    callbackIDIn - clear the callback with this id
     * */
    void YakIO_TIMER::ClearCallbackByID(enum CALLBACK_ID callbackIDIn)
    {
        // run through our list of callbacks
        if(callbackID==CALLBACK_NONE) return;
        // we only have one at the moment so this will do it
        if(callbackID==callbackIDIn)
        {        
            callbackInterfacePtr=NULL;
            callbackID=CALLBACK_NONE;
        }
    }
        
    /* EnableTimerIRQ - enable the timers IRQ in the NVIC
     * 
     * */
    void YakIO_TIMER::EnableTimerIRQ(void)
    {
        // we must be initialized
        if(isInitialized==0) return;  
              
        // enable the proper interrupt
        if(timerID==Timer0) EnableIRQ(IRQ_TIMER0);
        else if(timerID==Timer1) EnableIRQ(IRQ_TIMER1);
        else if(timerID==Timer2) EnableIRQ(IRQ_TIMER2);
    }
    
    /* DisableTimerIRQ - disable the timers IRQ in the NVIC
     * 
     * */
    void YakIO_TIMER::DisableTimerIRQ(void)
    {
        // we must be initialized
        if(isInitialized==0) return;  
              
        // enable the proper interrupt
        if(timerID==Timer0) DisableIRQ(IRQ_TIMER0);
        else if(timerID==Timer1) DisableIRQ(IRQ_TIMER1);
        else if(timerID==Timer2) DisableIRQ(IRQ_TIMER2);
    }
    
    /* QuickSetup - a function to quickly setup the timer. A lot of the timer
     *    setup is boilerplate and this minimizes that. Always assumes the 
     *    maximum bit size for the timer
     * 
     * Always starts the timer.
     * 
     * inputs:
     *   precalerValue - the value to divide down the 16Mz frequency (range of 0-9 is acceptable)
     *   countLevelValue - the value we count up to before triggering the interrupt
     *   callbackIDIn - the callback id to use. This identifies the function name that receives a call when the interrupt happens
     *   callbackInterfacePtrIn = the address of the object which receives the call when the interrupt happens. Usually
     *       the "this" pointer of the object calling the QuickSetup() but it does not have to be
     * */
    void  YakIO_TIMER::QuickSetup(unsigned int precalerValue, unsigned int countLevelValue, enum CALLBACK_ID callbackIDIn, YakIO_CALLBACK *callbackInterfacePtrIn)
    {
		// just call this with an implied wantStart=1
		QuickSetup(precalerValue, countLevelValue, callbackIDIn, callbackInterfacePtrIn, 1);
	}
	
    /* QuickSetup - a function to quickly setup the timer. A lot of the timer
     *    setup is boilerplate and this minimizes that. Always assumes the 
     *    maximum bit size for the timer
     * 
     * inputs:
     *   precalerValue - the value to divide down the 16Mz frequency (range of 0-9 is acceptable)
     *   countLevelValue - the value we count up to before triggering the interrupt
     *   callbackIDIn - the callback id to use. This identifies the function name that receives a call when the interrupt happens
     *   callbackInterfacePtrIn = the address of the object which receives the call when the interrupt happens. Usually
     *       the "this" pointer of the object calling the QuickSetup() but it does not have to be
     *   wantStart - if nz we start the timer. if z we do setup but leave it not stopped
     * */
    void  YakIO_TIMER::QuickSetup(unsigned int precalerValue, unsigned int countLevelValue, enum CALLBACK_ID callbackIDIn, YakIO_CALLBACK *callbackInterfacePtrIn, unsigned int wantStart)
    {
        // we must be initialized
        if(isInitialized==0) return;        

        // set the callback on the Timer
        SetCallback(callbackIDIn, callbackInterfacePtrIn);
        // make sure the timer is stopped
        TimerStop();
        // Timers 1 and 2 are 16 bit timers, timer 0 can be 24 or 32 bit
        // we just keep things simple by setting this to the max
        if(timerID==Timer0) SetBitMode(TIMER_BITMODE_32Bit);
        else SetBitMode(TIMER_BITMODE_16Bit);
        //SetBitMode(TIMER_BITMODE_16Bit);
        // put it in timer mode (as opposed to counter)
        SetMode(TIMER_MODE_Timer);
        // set the prescaler to divide down the 16Mz base frequency
        // this is always 16000000/(2^countLevelValue) 
        SetPrescaler(precalerValue);
        // trigger an event every time we get to this value 
        SetCountLevel(countLevelValue);
        // set up a short cut to clear the COMPARE counter every time we trigger
        SetShortCut();
        // explicitly set the counter to 0, we do not want to start at some other value
        TimerClear();
        // enable an interrupt every time our count matches the counter
        SetINTEN();
        // we can trigger interrupts all we want but they will not call anything unless
        // we also enable the TIMERs interrupt in the NVIC
        EnableTimerIRQ();
        
        // do we want to start?
        if(wantStart !=0) 
        {
			// start the timer and start triggering interrupts
			TimerStart();
		}
		else {} // just leave it stopped
	
    }
        
    /* TimerShutdown - shuts down the timer and clears all interrupts and 
     *     callbacks.
     * */
    void YakIO_TIMER::TimerShutdown(void)
    {
        // make sure the timer is stopped
        TimerStop();
        // clear all callbacks
        ClearAllCallbacks();
        // disable the IRQ
        DisableTimerIRQ();
    }
        
    /* TimerStart - starts the timer. 
     * 
     * Note: The timer should be fully configured before starting. You 
     *    should not change the parameters while is is running. Bad
     *    things will happen
     *  
     * */
    void YakIO_TIMER::TimerStart(void)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_START)) = 1;            
    }    

    /* TimerStop - stops the timer. 
     * 
     * Note: The timer should be stopped before configuring. You 
     *    should not change the parameters while is is running. Bad
     *    things will happen
     *  
     * */
    void YakIO_TIMER::TimerStop(void)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_STOP)) = 1;            
    }    
    
   /* TimerClear - clears down the timers counter
     * 
     * */
    void YakIO_TIMER::TimerClear(void)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_CLEAR)) = 1;            
    }    
    
    /* SetPrescaler - sets the prescaler value to divide down the clock
     *    frequency
     * 
     * Note: the prescaler value can only be between 0 and 9 (inclusive)
     *    This is NOT checked.
     * 
     * 
     * inputs:
     *         precalerValue - a value between 1 and 9. Clock freq will 
     *        be divided by 2^prescalerValue
     * */
    void YakIO_TIMER::SetPrescaler(unsigned int precalerValue)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_PRESCALER)) = precalerValue;            
    }    
    
    /* GetPrescaler - gets the prescaler value which divides down the clock
     *    frequency
     * 
     * returns
     *        returns the current prescaler value
     * */
    unsigned int YakIO_TIMER::GetPrescaler(void)
    {
        // we must be initialized
        if(isInitialized==0) return 0;
        
        // get the value from the appropriate register
        unsigned int prescalerVal = (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_PRESCALER));
        return prescalerVal;
    }    
    
    /* SetBitMode - sets the bit mode value. This determines the maximum
     *    value that can be set in the CountLevel register
     * 
     * Note: for 16 bit timers (timers 1 and 2) this value cannot be
     *    enum TIMER_BITMODE.TIMER_BITMODE_32Bit or TIMER_BITMODE.TIMER_BITMODE_24Bit
     * 
     * 
     * inputs:
     *         bitModeValue - a TIMER_BITMODE value indicating the mode
     * */
    void YakIO_TIMER::SetBitMode(enum TIMER_BITMODE bitModeValue)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_BITMODE)) = bitModeValue;            
    }    
    
    /* GetBitMode - gets the bit mode state
     * 
     * returns
     *        returns a TIMER_BITMODE value indicating the mode
     * */
    enum TIMER_BITMODE YakIO_TIMER::GetBitMode(void)
    {
        // we must be initialized
        if(isInitialized==0) return TIMER_BITMODE_16Bit;
        
        // get the value from the appropriate register
        unsigned int modeVal = (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_BITMODE));
        if(modeVal==TIMER_BITMODE_32Bit) return TIMER_BITMODE_32Bit;
        else if(modeVal==TIMER_BITMODE_24Bit) return TIMER_BITMODE_24Bit;
        else if(modeVal==TIMER_BITMODE_08Bit) return TIMER_BITMODE_08Bit;
        else return TIMER_BITMODE_16Bit;
    }    
        
    /* SetMode - sets the mode value. This determines if we operate as 
     *    a timer or a counter
     * 
     * inputs:
     *         ModeValue - a TIMER_MODE value indicating the mode
     * */
    void YakIO_TIMER::SetMode(enum TIMER_MODE modeValue)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_MODE)) = modeValue;            
    }    
    
    /* GetMode - gets the mode state
     * 
     * returns
     *        returns a TIMER_MODE value indicating the mode
     * */
    enum TIMER_MODE YakIO_TIMER::GetMode(void)
    {
        // we must be initialized
        if(isInitialized==0) return TIMER_MODE_Timer;
        
        // get the value from the appropriate register
        unsigned int modeVal = (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_MODE));
        if(modeVal==TIMER_MODE_Counter) return TIMER_MODE_Counter;
        else return TIMER_MODE_Timer;
    }    
        
    /* SetCountLevel - sets the CountLevel value to set. This is the value the timer
     *    counts up to before triggering an interrupt
     * 
     * Note: for 16 bit timers (timers 1 and 2) this value must be between 0 and 65,536
     *    For 32 bit timers (timer 0) this value must be between 0 and 4,294,967,296
     * 
     *    This is NOT checked.
     * 
     *    We only ever use COMPARE0. Using multiple compare registers does not work
     *    the way you probably think they should. See the link below for more info
     *    https://devzone.nordicsemi.com/f/nordic-q-a/18237/timer-with-two-compared-values
     * 
     * inputs:
     *         countLevelValue - a value to count up to
     * */
    void YakIO_TIMER::SetCountLevel(unsigned int countLevelValue)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // set the value in the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_CC_0)) = countLevelValue;            
    }    
    
    /* GetCountLevel - gets the CountLevel value. This is the value the timer
     *    counts up to before triggering an interrupt
     * 
     *    We only ever use COMPARE0. Using multiple compare registers does not work
     *    the way you probably think they should. See the link below for more info
     *    https://devzone.nordicsemi.com/f/nordic-q-a/18237/timer-with-two-compared-values
     * 
     * returns
     *        returns the current CountLevel value
     * */
    unsigned int YakIO_TIMER::GetCountLevel(void)
    {
        // we must be initialized
        if(isInitialized==0) return 0;
        
        // get the value from the appropriate register
        unsigned int CountLevelVal = (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_CC_0));
        return CountLevelVal;
    }    
    
    /* ClearCompareEvent() - We call this in the interrupt handler to clear it 
     *      if we do not we can never receive another
     * */
    void YakIO_TIMER::ClearCompareEvent(void)
    {
        // we must be initialized
        if(isInitialized==0) return;
        
        // get the value from the appropriate register
        (*(unsigned volatile *) (timerRegisterAddress+TIMERREG_OFFSET_COMPARE_0)) = 0;
    }    
        
    /* IRQ_TIMER?_handlers
     * 
     * Note: the address of these functions are set in the flash by the linker. 
     *       If this function exists then this functions address will be used
     *       if it does not exist then the interrupt will be directed to a
     *       default handler (an error routine) which just displays an error
     *       shape on the LEDArray. The flash address is not runtime settable.
     * 
     *       The name really matters here. See the discussion in YakIO.cpp
     * 
     *   Do NOT define these anywhere else. This class needs them here.
     * */
    void IRQ_TIMER0_handler(void)
    {
        if(timer_ptr0==NULL) return;
        // we have a pointer, call the callback
        timer_ptr0->CallCallback();
        // reset the timer, we MUST do this or we never get another
        timer_ptr0->ClearCompareEvent();
    }
    void IRQ_TIMER1_handler(void)
    {
        if(timer_ptr1==NULL) return;
        // we have a pointer, call the callback
        timer_ptr1->CallCallback();
        // reset the timer, we MUST do this or we never get another
        timer_ptr1->ClearCompareEvent();
    }
    void IRQ_TIMER2_handler(void)
    {
        if(timer_ptr2==NULL) return;
        // we have a pointer, call the callback
        timer_ptr2->CallCallback();
        // reset the timer, we MUST do this or we never get another
        timer_ptr2->ClearCompareEvent();
    }
