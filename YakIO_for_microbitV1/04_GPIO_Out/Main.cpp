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

// EXAMPLE code to use demonstrate how to toggle an output pin using the
// YakIO_GPIO class of the YakIO library.

// For the purposes of demonstration we toggle Pin 0 on the microbit's card
// edge connector. You will need an oscilloscope to see the output. If you 
// don't have an oscilloscope, read the comments in the MainLoop() anyways.

// Since we are going to be toggling the state of a pin on card edge 
// connector we might as well try various ways of seeing how fast we can 
// do it. The results are quite interesting (spoiler alert: 2.31 MHz is 
// possible).

// Oh, yeah, the center LED on the microbit is also blinked at 1Hz to give 
// you something to look at. This is not discussed. See the 02_BetterBlinky 
// for details.

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
    
    // create an output GPIO on card edge connector pin 0
    YakIO_GPIO gpioPin0 {Pin0, PinDirOutput};

    // set our Heartbeat going
    heartbeatObj.QuickSetup(4, 1000, HEARTBEAT, this);
    
    // Set up a second timer to trigger a call to a dedicated 
    // callback function, Callback0(), every 1/2 second. We use this to
    // toggle the center LED (row 3, col 3). 
    timer1Obj.QuickSetup(9, 15625, CALLBACK_0, this, 1);

    // set up the LEDs
    ledArray.ClearImage();

                
    // #
    // # We enter the main control loop 
    // #
            
    while(1)
    {
        // note that there is no delay here. We are executing this code 
        // at the full 16MHz speed of the Nordic nRF51822 CPU in here
                
        // Due to the actions of the timers, you should see the center 
        // LED (row 3, col 3) continue to blink at 1Hz. 
        
        // let's do some speed tests on various ways of setting the GPIOs.
        // We are going to see some interesting things! Not all methods
        // are equal. 
        
        // comment in and comment out one method at a time and observe
        // the output on Pin0 with an oscilloscope
        
        // #####
        // METHOD 1 - Simple Toggle. This toggles pin 0 at 106 KHz. The 
        // slow speed is due to the overhead of the function call and 
        // also that the ToggleGPIOState() call has to read the GPIO 
        // register in order to set it to the opposite state
        
        gpioPin0.ToggleGPIOState();
  
        // #####
        // METHOD 2 - Set and Reset. This toggles pin 0 at 310 KHz. Not 
        // reading the GPIO register improves the speed. However, we do  
        // have the overhead of two function calls. Also note that 
        // we are passing in parameters on the calls. As you will see in
        // the next section, this really matters for speed.
 
        //gpioPin0.SetGPIOState(1);
        //gpioPin0.SetGPIOState(0);
        
        // #####
        // METHOD 3 - Set and Reset No-Param. This toggles pin 0 at 413 KHz. 
        // We still have two function calls but we are not passing in 
        // parameters. The parameters cause the compiler a surprising  
        // amount of trouble behind the scenes and not using them gains 
        // us 100 KHz of speed.  

        //gpioPin0.SetGPIOStateHigh();
        //gpioPin0.SetGPIOStateLow();
        
        // #####
        // METHOD 4 - Bare metal. This toggles pin 0 at 2.31 MHz (yes, Mega Hz). 
        // This code was just scraped out of the YakIO_GPIO.cpp class 
        // and is basically what the SetGPIOStateHigh/Low calls run.
         
        // So if you are really, really, optimizing for speed remember that 
        // function calls are slow and you can always go bare metal. 
        
        // Also note the shift of the the 1 bit into position (the 0x01<<GPIO_PIN_SHL_VAL)
        // in the call. The value you shift by is the gpio number which in 
        // this case is 3 not the number of the pin which is 0. See the
        // "enum GPIOPin" definition in YakIO_GPIO.h file for a pin number 
        // to GPIO correlation. The enum val "Pin0" could just have been 
        // used instead (0x01<<Pin0) but then it would not have been as 
        // easy to bring your attention to it
 
        //#define GPIO_PIN_SHL_VAL 3
        //(*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_OUTSET)) = (0x01<<GPIO_PIN_SHL_VAL);  
        //(*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_OUTCLR)) = (0x01<<GPIO_PIN_SHL_VAL);        

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
    // because we created the ledArray() object as a member variable in 
    // the Main() class we have full access to it here. 
    
    // toggle the LED at row=3, col=3
    ledArray.ToggleLEDState(3,3);  
    
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



