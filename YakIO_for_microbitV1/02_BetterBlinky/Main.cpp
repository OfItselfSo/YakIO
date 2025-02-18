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

// EXAMPLE code to use the YakIO library to blink a LED on the BBC microbit at 
// a rate of 1Hz. Unlike in the 01_Blinky example we do this with callback functions
// triggered by interrupts in Timers. We also lead with an interesting display on the 
// 5 x 5 LED array in order to demonstrate how to send patterns to the LEDS.

// Define some patterns for the LED arrays. Note that these are being declared on the heap.
// We could put them inside the MainLoop() member function if we wished or declare over
// in the Main.h as a member variable. It is being done this way to demonstrate that it is 
// OK to create variables on heap. It is the instantation of class variables with constructors
// that is the problem. See the 03_Danger example for details.
unsigned char allLEDs[] = 
         {1,1,1,1,1,
          1,1,1,1,1,
          1,1,1,1,1,
          1,1,1,1,1,
          1,1,1,1,1}; 

unsigned char bigSquare[] = 
         {1,1,1,1,1,
          1,0,0,0,1,
          1,0,0,0,1,
          1,0,0,0,1,
          1,1,1,1,1}; 

unsigned char smallSquare[] = 
         {0,0,0,0,0,
          0,1,1,1,0,
          0,1,0,1,0,
          0,1,1,1,0,
          0,0,0,0,0}; 

/* MainLoop. This is where the user program starts. This function should
 *     contain a loop that never exits. We can NEVER return from here!
 * */
void Main::MainLoop(void)
{    
    // This is our second example. It is more complicated in that it 
    // defines a Heartbeat function triggered every 1 MilliSec by a 
    // timer to take care of periodic maintenance functions. We use it to
    // refresh the LED array.
    
    // we also set up a second timer to trigger a call to a dedicated 
    // callback function, Callback0(), every 1/2 second. We use this to
    // toggle the center LED (row 3, col 3)
    
    // #
    // # We do setup now
    // #

    // Set our heartbeat going, the display is refreshed from here
    // but you can add your own polled stuff. This is really just
    // Timer2 but we use it so commonly we give it a special name.
    // For ease of reference, here is the line of code below
    
    //     heartbeatObj.QuickSetup(4, 1000, HEARTBEAT, this);
    
    // NOTE: you have to know what the pameters mean. In this case the 
    // "4" is the prescaler value. The 16MHz clock of the microbit is 
    // divided down by 2^4 (or 16) giving a base clock rate of 1Mhz. The
    // second value is the counter. The 1000 means that at our 1Mhz cycle
    // time in the timer it will count up to 1000 before triggering the 
    // interrupt. This gives us a 1 MilliSecond call rate.
    
    // In this example the Main() function inherits from YakIO_CALLBACK.
    // This means that if we implement the functions defined in YakIO_CALLBACK
    // then other classes can consider us to _be_ a YakIO_CALLBACK object and
    // don't actually need to know what else we might be. This nicely abstracts 
    // the callback functionality.
    
    // The "HEARTBEAT" param identifies the name of a callback function which 
    // will be called when this timer interrupt triggers. The "this" pointer
    // allows the timer to know which instance of YakIO_CALLBACK to 
    // execute the callback in. See the comments on Main::Heartbeat()
    // for more information
    heartbeatObj.QuickSetup(4, 1000, HEARTBEAT, this);

    // Configure our second timer. If you do the math you can see that 
    // here the 16MHz clock is divided down by 512 to 31250 Hz. The 15625
    // counter ensures CALLBACK_0 is called every half second. 
    
    // WARNING 1: Timers 1 and 2 are 16 bit timers. The maxiumum they can 
    // count up to before rolling over is 65535. If you need to count
    // higher, either divide the clock frequency further or use Timer 0
    // which is a 32 bit timer and can count up to 4294967295
    
    // WARNING 2: The only acceptable prescaler values are in the range
    // 0-9 (inclusive) so 31250 Hz (16Mhz/2^9) is the slowest you can prescale 
    // any timer down to. Be sure to remember this. 
    
    // call QuickSetup on the timer, but use the polymorphic variant that 
    // configures it but leaves it stopped
    timer1Obj.QuickSetup(9, 15625, CALLBACK_0, this, 0);

    // #
    // # The Heartbeat timer is running now and the timer1Obj is configured
    // # but not started. Note that we are not yet in the main loop.
    // # We do some stuff to demonstrate a few more techniques
    // #

    // Send patterns to the LED arrays. The only reason this works
    // and the screen is not totally blank is because the Heartbeat
    // timer has started and it is refreshing the LED array for us.
    // This illustrates why it is better to do simple, quick, repetitive
    // things like this out of a timer rather than coding it in the mainloop 
    // as was done in example 01_Blinky

    // turn all LEDs on, the LED blink timer was created but not started
    // and so the center LED should not blink during this sequence
    ledArray.SetBinaryImage(allLEDs);
    // delay for 1 second
    DELAY_MILLI_SEC(1000);
    // send a big square
    ledArray.SetBinaryImage(bigSquare);
    // delay for 1 second
    DELAY_MILLI_SEC(1000);
    // send a small square
    ledArray.SetBinaryImage(smallSquare);
    // delay for 1 second
    DELAY_MILLI_SEC(1000);
     // clear it down
    ledArray.ClearImage();
    // delay for 1 second
    DELAY_MILLI_SEC(1000);
    
    // now start the timer that blinks the center LED, the 
    // center LED should start blinking
    timer1Obj.TimerStart();
    
    
    // #
    // # We enter the main control loop 
    // #
        
    // Note that we are executing code here at the full speed of the 
    // Nordic CPU used in the microbit (16 Mhz on V1, 32Mhz on V2)
    // Also note that we never leave the loop below!
    
    while(1)
    {
        // we don't have anything to do here - but you could add 
        // your own code. 
        
        // You should see the center LED (row 3, col 3) continue to
        // blink due to the actions of the timers

    } // bottom of while(1)
} // bottom of Main::MainLoop()

/* Callback0 - this is a callback function which gets called when Timer1 
 *    triggers. We used enum CALLBACK_ID.CALLBACK_0 when we created the 
 *    timer, therefore, this function MUST be named Callback0(). 
 * 
 *    The reason this works is that our Main class inherits from 
 *    YakIO_CALLBACK (class Main : public YakIO_CALLBACK) and 
 *    the YakIO_CALLBACK class functions as an interface. If the timer 
 *    object is set to use CALLBACK_ID.CALLBACK_0 the timer will know to call
 *    Callback0() in the target class.
 * 
 *    NOTE: You are in an INTERRUPT in here! Remember that the mainloop() 
 *    is stalled while this function is executing - so do NOT call really 
 *    long running things in here. Return as quickly as possible. 
 *    Even though you have access to all of the class  variables and 
 *    functions you are in an INTERRUPT in here!
 *
 * */
void Main::Callback0(void)
{
    // because we created the ledArray() object as a member variable in 
    // the Main() class we have full access to it here. 
    
    // You might be tempted to create the ledArray() object on the heap
    // in order to make it global. Do NOT do this - bad things happen.
    // See the 03_Danger example code.
    
    // toggle the LED the hard way so as to demonstrate the get/set calls
    unsigned int ledState = ledArray.GetLEDState(3,3);  
    if(ledState !=0) ledArray.SetLEDState(3,3,0);  
    else ledArray.SetLEDState(3,3,1);  
    
    // Note that you do not have to do anything to acknowledge or
    // cancel the interrupt. This is done for you by the object that
    // initiated this call
}

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



