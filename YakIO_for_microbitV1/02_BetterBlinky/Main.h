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
        // in the 01_Blinky example, the ledArray class was declared on the stack in 
        // the MainLoop() member function. We want the callbacks to have 
        // access to it - so we declare it here. It can be private. When the 
        // callbacks happen, they execute within the instantiated instance 
        // and so have complete access to private member variables.

        // this class controls the 5x5 LED display
        YakIO_LEDARRAY ledArray {};
        
        // the heartbeat is a 1 millisecond tick that enables us 
        // to do periodic things. TIMER2 is typically used for the heartbeat.
        // We give it a special name but really it is just a Timer
        YakIO_TIMER heartbeatObj {Timer2};
        
        // A generic timer we can use to get periodic interrupts. In this example
        // we will use it to toggle the LED at row 3, col 3 at 1Hz 
        YakIO_TIMER timer1Obj {Timer1};

    public:
        // this needs to be public because the CreateMainObject() function in program.cpp 
        // calls it. See that code to better understand what is going on here.
        void MainLoop(void);
        // These are our callback functions. In this example the two timers will be 
        // calling them every time an interrupt is triggered in them. Note the 
        // "override" here. We want to execute the code in these functions NOT the ones  
        // in the YakIO_CALLBACK interface
        void Callback0 (void) override;
        // Because we use the Heartbeat pattern so much, the YakIO_CALLBACK
        // interface has a special name for it. Really it is just another callback 
        // function and (for example) we could use something else like Callback5() 
        // if we wanted to
        void Heartbeat(void) override;

};

#endif
