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

// EXAMPLE code to demonstrate how to debounce the buttons and also count button
// presses. The count is made visible by sequentially lighting LEDs on the
// 5 x 5 array. Once the LEDs in the array are all on, they are all turned
// off and the process begins again.

// Simple switch debouncing code has been introduced into the Heartbeat
// routine. This removes that complexity from the MainLoop() and that loop
// can then concentrate on counting and setting the LED's

// Button A is debounced, Button B is not. Both only activate on a pressed
// to unpressed transition.

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

    // set up the LEDs
    ledArray.ClearImage();

    // the countMap is a 1's and 0's representation of what we want the
    // 5x5 LED array to look like. Clear it down now.
    ClearCountMap();

    // #
    // # We enter the main control loop
    // #

    while(1)
    {
        // note we are executing at the full 16Mhz speed of the CPU
        // in here. We are relying on the switch debouncing (or not)
        // to make sure we don't interpret a single button press as
        // multiple independent events.

        // instead of simple counting we set the next value in a 25 slot
        // array called countMap to 1 each time we detect a button release.
        // We then display this countMap on the 5 x 5 LED Array.

        // Every time you release a button you should see a new LED light
        // up in the LED array. When they are all filled, the array just
        // resets to all off and it begins again.

        // ButtonA is de-bounced - see the code in the Heartbeat for
        // how this is done. All we care about here is the state of
        // the buttonAHasBeenPressed variable.
        if (buttonAHasBeenPressed!=0)
        {
            // run through each LED
            for(int i=0; i<NUM_LEDS_IN_ARRAY; i++)
            {
                // found one we have not updated yet?
                if(countMap[i]==0)
                {
                    // yes, leave now
                    countMap[i]=1;
                    break;
                }
                // have we have filled the graphical display? if so
                // just reset
                if(i>=(NUM_LEDS_IN_ARRAY-1)) ClearCountMap();
            }
            // always show the image
            ledArray.SetBinaryImage(countMap);
            // we must reset this
            buttonAHasBeenPressed=0;
        }

        // ButtonB is not de-bounced. As with ButtonA, we get the
        // buttonBHasBeenPressed variable set to 1 on a pressed to
        // unpressed transition. We might see multiple LEDs light
        // up from a single press though as the MainLoop() handles this
        // pretty fast.
        if (buttonBHasBeenPressed!=0)
        {
            // run through each LED
            for(int i=0; i<NUM_LEDS_IN_ARRAY; i++)
            {
                // found one we have not updated yet?
                if(countMap[i]==0)
                {
                    // yes, leave now
                    countMap[i]=1;
                    break;
                }
                // have we have filled the graphical display? if so
                // just reset
                if(i>=(NUM_LEDS_IN_ARRAY-1)) ClearCountMap();
            }
            // always show the image
            ledArray.SetBinaryImage(countMap);
            // we must reset this
            buttonBHasBeenPressed=0;  // must reset
        }

    } // bottom of while(1)
} // bottom of Main::MainLoop()

/* ClearCountMap - clears down our graphical map of the counts we have
 *    received
 * */
void Main::ClearCountMap(void)
{
    for(int i=0; i<NUM_LEDS_IN_ARRAY; i++) countMap[i]=0;
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

    // we are handling the buttons below. Note that ButtonA and ButtonB
    // have 10K pullup resistors on them. This means an unpressed button
    // reads as "high" - ie a 1. A pressed button reads as "low" - ie 0.

    // See the comments in the 05_GPIO_In sample code for a longer
    // discussion.

    // Button A is debounced - some switches are noisy and cycle quickly
    // between off and on as they are change state and the voltage arcs
    // between the contacts. We do NOT want to have every little bounce
    // in the signal be interpreted as a separate button press.

    // Below is an example of a simple debouncing algorythm.

    // 1) We trigger only when the switch is released
    // 2) When released we check to see that the switch was actually pressed
    //      for a least a minimum number of Heartbeat ticks.
    // 3) If the switch was held down for the minimum time and then released
    //      we set a flag.
    // 4) The flag we set stays set until reset by something in the MainLoop
    //      that has noticed it was pressed and has handled the press.
    // 5) This is done so that a continuous press does not trigger an endless
    //      sequence of actions. Only a definitive "press and release" event
    //      will trigger an action.

    // Admittedly this code does look a bit verbose - compiles down to almost
    // nothing though
    if(gpioButtonA.GetGPIOState() == 0)
    {
        // button pressed, count this
        buttonACounter++;
        // has the button been held for enough consecutive heartbeats?
        if(buttonACounter>=MIN_HEARTBEATS_FOR_A_BUTTONPRESS)
        {
            // yes, it has, record state as a proper low
            buttonAState=0;
        }
        else
        {
            // no it has not, record state still as a high
            buttonAState=1;
        }
    }
    else
    {
        // button not pressed, but were we pressed and now let up?
        if(buttonACounter>=MIN_HEARTBEATS_FOR_A_BUTTONPRESS)
        {
            // yes, it was, set this flag on the release
            buttonAHasBeenPressed=1;
        }
        else {} // just leave it, if buttonAHasBeenPressed is on it is up to the app to reset it
        //reset both of these
        buttonACounter=0;
        buttonAState=1;
    }

    // Button B is not debounced. We still look for the pressed to
    // unpressed transition but if the reset over in MainLoop() gets
    // to it too quickly, a single press and release on the switch can
    // look like mulitple rapid press and releases.
    unsigned int currentState = gpioButtonB.GetGPIOState();
    if ((currentState == 1) && (buttonBState==0))
    {
        buttonBHasBeenPressed=1;
    }
    buttonBState=currentState;

    // note that you do not have to do anything to acknowledge or
    // cancel the interrupt. This is done for you by the object that
    // initiated this call

}



