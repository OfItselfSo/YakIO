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

// EXAMPLE code to demonstrate how to read an input pin using the
// YakIO_GPIO class of the YakIO library. One way to demonstrate the 
// read mechanism is to accept as input a signal we, ourselves, generate.

// For the purposes of demonstration, we use a timer to toggle Pin 0 on 
// the microbit's card edge connector at a rate of 1Hz. Pin 0 is then  
// connected externally to Pin 2 on the card edge connector. The MainLoop() 
// reads the state of Pin 2 and sets the center LED to reflect that state.

// You need to connect Pin 0 to Pin 2. Use alligator clips or bannana jacks
// if you have them - otherwise, just touch a wire to both.

// If Pin 0 is connected to Pin 2 on the card edge connector
// then the center LED should blink at 1Hz

/* MainLoop. This is where the user program starts. This function should
 *     contain a loop that never exits. We can NEVER return from here!
 * */
void Main::MainLoop(void)
{    
    // there are a lot of things going on in this example. If you don't
    // understand something you should probably review the code in the 
    // 02_BetterBlinky sample. 
    
    // #
    // # We do setup now
    // #
    
    // set our Heartbeat going
    heartbeatObj.QuickSetup(4, 1000, HEARTBEAT, this);
    
    // Set up a second timer to trigger a call to a dedicated 
    // callback function, Callback0(), every 1/2 second. 
    timer1Obj.QuickSetup(9, 15625, CALLBACK_0, this, 1);

    // create an input GPIO on card edge connector pin 2
    YakIO_GPIO gpioPin2 {Pin2, PinDirInput};

    // set up the LEDs
    ledArray.ClearImage();
            
    // #
    // # We enter the main control loop 
    // #
    
    // set up a variable
    unsigned int pin0State;
            
    while(1)
    {
        // note we are executing at the full 16Mhz speed of the CPU
        // in here. We could be doing other things as well. The code
        // below really doesn't need to execute as fast as it is.

        // Get the current state of pin 2 on the card edge connector.
        // Pin 2 is connected by a wire to pin 0 which is being toggled
        // at 1Hz by the timer operating the Callback0() function
        pin0State = gpioPin2.GetGPIOState();
        
        // set our LED array pin (3,3) to always reflect the state 
        // of Pin 2
        ledArray.SetLEDState(3,3,pin0State);
        
        // NOTE: what happens if you disconnect the wire? The LED will 
        // always be ON indicating that pin 2 reads as high. 
        
        // IMPORTANT: Pins 0,1,2 have 10MOhm pull-up resistors on the PCB
        // and Pins 5 and 11 (the buttons) have 10KOhm resistors on the PCB.
        // Left unconnected, they always read as 1 "high". You have to 
        // actively pull them "low" to get them to read as 0. You can
        // configure the PullUp/PullDown state for the other Pins and this  
        // brings resistors internal to the CPU into play but because of the 
        // external PullUps put in place by the designers of the microbit,
        // Pins 0,1,2,5,11 are always pulled up no matter what you do. The 
        // YakIO_GPIO class ignores any attempt to change the PullUp state
        // of Pins 0,1,2,5,11. 
        
    } // bottom of while(1)
} // bottom of Main::MainLoop()

/* Callback0 - this is a callback function which gets called when Timer1 
 *    triggers. 
 *   
 *    See the 02_BetterBlinky sample code for a full explanation of 
 *    how this works.
 * 
 *    You have full access to the class member variables here (even 
 *    the private ones) but remember you are in an INTERRUPT -
 *    the MainLoop() is stalled while this code runs. Be Quick!
 * 
 * */
void Main::Callback0(void)
{
    // because we created the gpioPin0() object as a member variable in 
    // the Main() class we have full access to it here. 
    
    // The timer is set up to call this function every 1/2 second
    
    // toggle the state of pin 0 on the card edge connector
    gpioPin0.ToggleGPIOState();  
    
    // Note that you do not have to do anything to acknowledge or
    // cancel the interrupt. This is done for you by the object that
    // initiated this call
}

/* Heartbeat - this is the Heartbeat callback function 
 * 
 *    See the 02_BetterBlinky sample code for a full explanation of 
 *    how this works.
 * 
 *    You have full access to the class member variables here (even 
 *    the private ones) but remember you are in an INTERRUPT -
 *    the MainLoop() is stalled while this code runs. Be Quick!
 * 
 * */
void Main::Heartbeat(void)
{
    // The heartbeat is a 1 millisecond tick that enables us 
    // to do periodic things. TIMER2 is typically used for the heartbeat
        
    // the 5 x 5 LED array does not work the way you probably think it 
    // does. We need to refresh it constantly to get it to display.
    // See the 02_BetterBlinky sample code for a full explanation.
    ledArray.RefreshLEDArray();    
    
    // note that you do not have to do anything to acknowledge or
    // cancel the interrupt. This is done for you by the object that
    // initiated this call

}



