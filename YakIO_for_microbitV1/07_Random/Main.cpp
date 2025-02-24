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

// EXAMPLE code to use the YakIO library to generate a random number. For the purposes of 
// demonstration it can do this two ways: via polling or via a callback triggered by an 
// interrupt. The WANT_RNG_VIA_INTERRUPT define controls this

// However obtained, our random number is stored in a class member variable and the 5x5 LED
// array is updated with an approximation of that random numbers value.

// Define some patterns for the LED arrays.
unsigned char largeSquare[] = 
         {1,1,1,1,1,
          1,0,0,0,1,
          1,0,0,0,1,
          1,0,0,0,1,
          1,1,1,1,1}; 

unsigned char ledValues[] = 
         {0,0,0,0,0,
          0,0,0,0,0,
          0,0,0,0,0,
          0,0,0,0,0,
          0,0,0,0,0}; 

/* MainLoop. This is where the user program starts. This function should
 *     contain a loop that never exits. We can NEVER return from here!
 * */
void Main::MainLoop(void)
{    
    
    // #
    // # We do setup now
    // #

    // Set our heartbeat going, the display is refreshed from here
    // but you can add your own polled stuff. See the 02_BetterBlinky
    // example for detailed comments.    
    heartbeatObj.QuickSetup(4, 1000, HEARTBEAT, this);

    // configure our random number generator. We can get our random numbers
    // via polling or via a callback triggered by an interrupt. The 
    // WANT_RNG_VIA_INTERRUPT define determines this
    
#define WANT_RNG_VIA_INTERRUPT 0    
    
    // do we want to get it via an interrupt?
    if(WANT_RNG_VIA_INTERRUPT !=0)
    {
        // yes, we do, Callback0() will be triggered each time a new 
        // random number is ready
        rngObj.SetCallback(CALLBACK_0, this);
        rngObj.RngStart(); 
    }
    else
    {
        // no, just get it via polling in the main loop
        rngObj.RngStart(); 
    }

    // Send patterns to the LED arrays. The only reason this works
    // and the screen is not totally blank is because the Heartbeat
    // timer has started and it is refreshing the LED array for us.
    ledArray.SetBinaryImage(largeSquare);
    // delay for 1 second
    DELAY_MILLI_SEC(1000);

     // clear it down
    ledArray.ClearImage();
    // delay for 1 second
    DELAY_MILLI_SEC(1000);
    
    
    // #
    // # We enter the main control loop 
    // #
        
    // Note that we are executing code here at the full speed of the 
    // Nordic CPU used in the microbit (16 Mhz on V1, 32Mhz on V2)
    // Also note that we never leave the loop below!
         
    while(1)
    {
        // delay for 2 seconds so that we do not distract the view with
        // to many screen flashes
        DELAY_MILLI_SEC(2000);
        
        // are we polling? if so get the new random number now, otherwise
        // the value will be automatically set via an interrupt callback
        if(WANT_RNG_VIA_INTERRUPT ==0) rngVal = rngObj.GetRngValue();
                
        // at this point we will have a random number value in rngVal
        // it will have been obtained by direct polling or set via a 
        // interrupt calling back to Callback0(). Which way this happens
        // depends on the setting of the WANT_RNG_VIA_INTERRUPT define
        
        // we wish to display something on the screen that represents 
        // our random number. Since it is always in the range of 0-255, 
        // we just divide it by 10 to give us an approximate range of 
        // 0-25 and display that many leds. That way we can at least see 
        // the random number change

        unsigned int loopCount = rngVal/10;
        // set a dummy array to 1's and 0's to send to the display 
        for(int i=0; i<NUM_LEDS_IN_ARRAY; i++)
        {
            if(loopCount>0) 
            {
                ledValues[i]=(char)1;
                loopCount=loopCount-1;
            }
            else ledValues[i]=0;
        }
        // note the only reason this shows anything is because the LED
        // array is continously being refreshed in the Heartbeat()
        ledArray.SetBinaryImage(ledValues);

    } // bottom of while(1)
} // bottom of Main::MainLoop()

/* Callback0 - this is a callback function which gets called when the 
 *    random  number generator triggers. We used enum CALLBACK_ID.CALLBACK_0 
 *    when we created the object, therefore, this function MUST be named Callback0(). 
 * 
 *    See the 02_BetterBlinky example for a complete discussion.
 * 
 *    NOTE: You are in an INTERRUPT in here! Remember that the mainloop() 
 *    is stalled while this function is executing - so do NOT call really 
 *    long running things in here. Be Quick!
 *
 * */
void Main::Callback0(void)
{
    // because we created the rngObj() object as a member variable in 
    // the Main() class we have full access to it here. 
    
    // You might be tempted to create the rngObj() object on the heap
    // in order to make it global. Do NOT do this - bad things happen.
    // See the 03_Danger example code.
    
    // Get the next random number. We know one is ready otherwise we 
    // would not have been called.
    rngVal = rngObj.GetRngValue();
    
    // Note that you do not have to do anything to acknowledge or
    // cancel the interrupt. This is done for you by the object that
    // initiated this call
}

/* Heartbeat - this is a callback function which gets called when the timer 
 *    triggers. We used enum CALLBACK_ID.HEARTBEAT when we created the 
 *    timer therefore this function MUST be named Heartbeat(). 
 * 
 *    See the 02_BetterBlinky example for a complete discussion.
 * 
 *    NOTE: You are in an INTERRUPT in here! Remember that the mainloop() 
 *    is stalled while this function is executing - do NOT call really 
 *    long running things in here. Be Quick!
 * 
 * */
void Main::Heartbeat(void)
{
    // The heartbeat is a 1 millisecond tick that enables us 
    // to do periodic things. TIMER2 is typically used for the heartbeat
        
    // Now show the next row in our LEDS. The LEDs are multiplexed in 
    // a complex way to save on I/O lines and are not just a simple
    // "turn them on and they stay on" type control like you probably 
    // think they are. See the 02_BetterBlinky example for detailed 
    // comments.

    ledArray.RefreshLEDArray();    
    
    // note that you do not have to do anything to acknowledge or
    // cancel the interrupt. This is done for you by the object that
    // initiated this call

}



