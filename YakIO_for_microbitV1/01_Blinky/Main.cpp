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
// a rate of 1Hz. 


/* MainLoop. This is where the user program starts. This function should
 *     contain a loop that never exits. We can NEVER return from here!
 * */
void Main::MainLoop(void)
{    
    // This is our first example. It has been deliberately kept 
    // simple so as to provide a quick start for new users. You would
    // probably not really blink a LED this way. There are much better
    // methods - see the 02_BetterBlinky example. 
    
    // #
    // # We do setup now
    // #
    
    // create the class that controls the 5x5 LED display
    YakIO_LEDARRAY ledArray {};
    // clear down, turn off all LEDs
    ledArray.ClearImage();
    
    // the loop below is set up to do 100 cycles per second
    #define CYCLES_IN_HALF_A_SECOND 50
    // define an internal counter variable
    unsigned int counter =0;
    
    // #
    // # We enter the main control loop 
    // #
        
    // Note that we are executing code here at the full speed of the 
    // Nordic CPU used in the microbit (16 Mhz on V1, 32Mhz on V2)
    // Also note that we never leave the loop below!
    
    // the goal here is to blink the center LED (row 3, col 3) at 
    // 1 Hz. In other words, half a second on and half a second off
    while(1)
    {
        // this delay is carefully constructed to pause for the 
        // specified number of milliseconds - in this case 10
        DELAY_MILLI_SEC(10);
        
        // because of the delay, our loop at this point has now slowed down 
        // to 0.01 seconds per cycle
           
        // Now show the next row in our LEDS. The LEDs are multiplexed in 
        // a complex way to save on I/O lines and are not just a simple
        // "turn them on and they stay on" type control like you probably 
        // think they are. The upshot is that in order to appear as
        // a "solid" display we have to refresh the LEDs at a rate faster than
        // the typical human perception. Every 1/100th second works. It does
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
        
        // increment our counter
        counter = counter + 1;
        // have we done enough?
        if(counter < CYCLES_IN_HALF_A_SECOND) continue;
        // yes, we have done half a seconds worth, so change the state of 
        // the LED at row 3, column 3
        ledArray.ToggleLEDState(3, 3);
        // and reset
        counter = 0;       
            
    } // bottom of while(1)
} // bottom of Main::MainLoop()

