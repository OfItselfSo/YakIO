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

#include "Main.h"
#include "YakIO_LEDARRAY.h"

// EXAMPLE code to instantiate an example class on the heap, on the 
// stack and as a member variable in order to demonstrate unequivocally
// that if you instantiate a class on the heap, the constructor does not 
// run and member variables are not initialized.

// A pattern for the LED arrays. See 02_BetterBlinky for a full discussion
// Variables such as this on the heap do get initialized
unsigned char allLEDs[] = 
         {1,1,1,1,1,
          1,1,1,1,1,
          1,1,1,1,1,
          1,1,1,1,1,
          1,1,1,1,1}; 

// instantiate a class on the heap to illustrate the execution of the 
// constructor. Note it is set to (row=3,col=3), the default value is (row=1,col=1)
LEDAddress laObjectOnHeap{3,3};

/* MainLoop. This is where the user program starts. This function should
 *     contain a loop that never exits. We can NEVER return from here!
 * */
void Main::MainLoop(void)
{    
    // This is our third example. It is designed to show what happens if 
    // you instantiate a class on the heap.
    
    // #
    // # We do setup now
    // #

    // Set our heartbeat going, the display is refreshed from there
    // See the 02_BetterBlinky example code for an extensive discussion
    // of the Heartbeat    
    heartbeatObj.QuickSetup(4, 1000, HEARTBEAT, this);

    // #
    // # The Heartbeat timer is running now 
    // #

    // instantiate a class on the stack to illustrate the execution of the 
    // constructor. Note it is set to (row=2,col=2), the default value is (row=1,col=1)
    LEDAddress laObjectOnStack{2,2};

    // Send a pattern to the LED array. The only reason this works
    // and the screen is not totally blank is because the Heartbeat
    // timer has started and it is refreshing the LED array for us.

    // turn all LEDs on
    ledArray.SetBinaryImage(allLEDs);
    // delay for 1 second
    DELAY_MILLI_SEC(1000);
     // clear it down
    ledArray.ClearImage();
    // delay for 1 second
    DELAY_MILLI_SEC(1000);
    
    // #
    // # now light up our LEDs based on their storage addresses. Each object
    // # was given a separate LED address to store when it was created but
    // # the default is (row=1, col=1)
    // # 
    
    // the stack version should activate the LED at (row=2,col=2)
    ledArray.SetLEDState(laObjectOnStack.GetRowVal(), laObjectOnStack.GetColVal(), 1);
    // the heap version should activate the LED at (row=3,col=3)
    ledArray.SetLEDState(laObjectOnHeap.GetRowVal(), laObjectOnHeap.GetColVal(), 1);
    // the member variable version should activate the LED at (row=4,col=4)
    ledArray.SetLEDState(laObjectAsMember.GetRowVal(), laObjectAsMember.GetColVal(), 1);
    
             
    // #
    // # We enter the main control loop 
    // #
            
    while(1)
    {
        // because the addresses (row, col) of LEDS we activate were 
        // activated from the values stored in instantiated classes, we  
        // would expect to see the LED array in the state as shown 
        // below where LEDS (2,2) and (3,3) and (4,4) are lit up.
        
//      0,0,0,0,0,
//      0,1,0,0,0,
//      0,0,1,0,0,
//      0,0,0,1,0,
//      0,0,0,0,0, 

        // instead we see the following...
 
//      0,0,0,0,0,
//      0,1,0,0,0,
//      0,0,0,0,0,
//      0,0,0,1,0,
//      0,0,0,0,0, 
 
        // note that LED (3,3) is not activated. These are the values used
        // in the object that was created on the heap. 
        
        // The constructor DOES NOT run!!!
        
        // Actually, it is even worse. The default values are (1,1) so the 
        // initialization of the member variables inside of the LEDAddress  
        // class did not even work. If it had, we would see something like
        
//      1,0,0,0,0,
//      0,1,0,0,0,
//      0,0,0,0,0,
//      0,0,0,1,0,
//      0,0,0,0,0, 
        
        // with LED (1,1) lit up as shown. But we do not - this means the 
        // member varibles did not even properly initialize either
        
        // WARNING!
        // WARNING!
        // WARNING!

        // YOU MUST NOT INSTANTIATE CLASSES ON THE HEAP USING THE YAKIO LIBRARY!
        
        // If you do, the constructor will NOT run and your member variables will 
        // not be initialized. Declaring them as member variables in a class 
        // or in a member function is OK. Everything works as expected.
        
        // NOTE this does not seem to apply to non-class variables declared on the 
        // heap. As can be seen above, the allLEDs[] array initialized just fine.
        // Only classes are the problem.
        
 
    } // bottom of while(1)
} // bottom of Main::MainLoop()

/* Heartbeat - this is a callback function which gets called when the timer 
 *    triggers. We used enum CALLBACK_ID.HEARTBEAT when we created the 
 *    timer therefore this function MUST be named Heartbeat(). 
 * 
 *    The reason this works is that our Main class inherits from 
 *    YakIO_CALLBACK (class Main : public YakIO_CALLBACK) and 
 *    the YakIO_CALLBACK class functions as an interface. If the timer 
 *    object is set to use CALLBACK_ID.HEARTBEAT the timer will know to call
 *    Heartbeat() in the target class.
 *  
 *    NOTE: the heartbeat is really just a timer (usually Timer2) that we
 *    use for common recurring tasks such as updating the LEDs so they display 
 *    correctly. You can add other operations in here such as polling the GPIO's
 *    etc but remember that the mainloop() is stalled while this function is 
 *    executing - so do NOT call really long running things in here. Return as 
 *    quickly as possible. Even though you have access to all of the class
 *    variables and functions you are in an INTERRUPT in here!
 * 
 * */
void Main::Heartbeat(void)
{
    // The heartbeat is a 1 millisecond tick that enables us 
    // to do periodic things. TIMER2 is typically used for the heartbeat
        
    // Now show the next row in our LEDS. The LEDs are multiplexed in 
    // a complex way to save on I/O lines and are not just a simple
    // "turn them on and they stay on" type control like you probably 
    // think they are. The upshot is that in order to appear as
    // a "solid" display we have to refresh the LEDs at a rate faster than
    // the typical human perception. Every 1/1000th second works. It does
    // not hurt to do it faster. Each call to the RefreshLEDArray() 
    // function refreshes 1/3 of the LEDs - just make repeated calls - it 
    // figures out for itself what to do next.
    //
    // If you ever see LEDs that should be on appear to flutter, blink
    // and dim it is a sure sign that your refresh rate is too slow.
    // 
    // If you wish to know more, see Mike Spiveys comprehensive discussion
    //    at: https://spivey.oriel.ox.ac.uk/bare metal/X2000
    ledArray.RefreshLEDArray();    
    
    // note that you do not have to do anything to acknowledge or
    // cancel the interrupt. This is done for you by the object that
    // initiated this call

}



