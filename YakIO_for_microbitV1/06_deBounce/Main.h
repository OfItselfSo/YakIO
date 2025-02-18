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

#ifndef MAIN_H
#define MAIN_H

#include "YakIO.h"
#include "YakIO_LEDARRAY.h"
#include "YakIO_TIMER.h"
#include "YakIO_CALLBACK.h"

/* Main - your program starts with a call to MainLoop() and all 
 *        global objects should be owned by this class
 * 
 *        WARNING: Do NOT declare class variables on the heap (ie outside of a class)! 
 *        The constructor will NOT be run when the object is created and member variables
 *        will NOT be initialized.
 * 
 *        Instantiate all classes inside some other class. If a class is instantiated
 *        at runtime (as opposed to compile time) then the constructor will run.
 * 
 *        You might wish to review the "03_Danger" sample code to see the bad 
 *        things that happen if you create classes with constructors on the heap.
 *       
 * */
class Main : public YakIO_CALLBACK // we inherit from this class which functions as an interface
{ 
    private:
    
        // a 25 slot array to record button presses
        unsigned char countMap[NUM_LEDS_IN_ARRAY]; 
    
        // this class controls the 5x5 LED display
        YakIO_LEDARRAY ledArray {};
        
        // create an input GPIO so we can read button A
        YakIO_GPIO gpioButtonA {ButtonA, PinDirInput};
        // create an input GPIO so we can read button B
        YakIO_GPIO gpioButtonB {ButtonB, PinDirInput};
        
        // the heartbeat is a 1 millisecond tick that enables us 
        // to do periodic things. TIMER2 is typically used for the heartbeat.
        // We give it a special name but really it is just a Timer
        YakIO_TIMER heartbeatObj {Timer2};
        
        // our button states are polled for debouncing purposes. Note that 
        // they seem to have pull up resistors so a pressed
        // button reads in as a button?State=0 and a unpressed
        // button reads as a button?State = 1
        unsigned buttonAState = 1;
        unsigned buttonBState = 1;
        unsigned buttonAHasBeenPressed = 0; // this latches until reset 
        unsigned buttonBHasBeenPressed = 0; // this latches until reset
        unsigned buttonACounter = 0;        // for debouncing
        unsigned buttonBCounter = 0;        // for debouncing
        
        // the debouncing is done out of the Heartbeat
        #define MIN_HEARTBEATS_FOR_A_BUTTONPRESS 10

        // clear down the array the graphically counts the button presses
        void ClearCountMap(void);
        
    public:
        // this needs to be public because the CreateMainObject() function in program.cpp 
        // calls it. See that code to better understand what is going on here.
        void MainLoop(void);
        // Because we use the Heartbeat pattern so much, the YakIO_CALLBACK
        // interface has a special name for it. Really it is just another callback 
        // function and (for example) we could use something else like Callback5() 
        // if we wanted to
        void Heartbeat(void) override;

};

#endif
