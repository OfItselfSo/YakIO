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
#include "YakIO_GPIO.h"

    /* constructor
     *
     * inputs:
     *    gpioPinIn - the gpioPin that this object represents
     * */
    YakIO_GPIO::YakIO_GPIO(enum GPIOPin gpioPinIn, enum GPIOPinDir gpioPinDir)
    {
        // set the gpio pin this objects represents, this can never
        // be changed
        gpioPin = gpioPinIn;

        // set this so we know we have run through the constructor. Creating
        // objects on the heap will NOT run the constructor
        isInitialised =1;

        // set the address of the cnf register for this gpioPin
        // note that the gpioPin is an enum but the value of the
        // elements in this enum are carefully set to be the
        // same as the GPIO offset value so manipulations like the
        // one below work.
        cnfRegisterAddress = REGISTER_GPIO + GPIOREG_OFFSET_PIN_CNF_BASE + (gpioPin*BYTES_IN_REGISTER);

        // now set things up, some of these are just defaults
        SetGPIODir(gpioPinDir);
    }

    /* GetGPIOPin - gets the GPIO pin value we are configured with
     *
     * returns:
     *   the GPIOPin enum we are currently configured with
     * */
    enum GPIOPin YakIO_GPIO::GetGPIOPin(void)
    {
        return gpioPin;
    }

    /* GetGPIOState - gets the state of the GPIO
     *
     *     NOTE this value is only relevant if the GPIO is set as an input
     *
     * returns:
     *   0 - the input state is low, 1 the input state is high
     * */
    unsigned int YakIO_GPIO::GetGPIOState(void)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return 0;

        // get the current register state
        unsigned int registerState = (* (unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_IN));
        // detect the value
        if((registerState & (0x01<<gpioPin)) == 0) return 0;
        else return 1;
    }

    /* SetGPIOState - sets the state of the GPIO
     *
     *     NOTE this action is only relevant if the GPIO is set as an output
     *
     *     Note: Due to the overhead of passing in a variable, this function call
     *       is about 25% slower than calls to SetGPIOStateHigh(), SetGPIOStateLow()
     *       a toggle rate of about 300Khz has been observed on a 16MHz V1
     *
     * inputs:
     *   gpioState - 0 pin is low, 1 the pin is high
     * */
    void YakIO_GPIO::SetGPIOState(unsigned int gpioState)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return;

        // set the output state
        if(gpioState==0)(*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_OUTCLR)) = (0x01<<gpioPin);
        else (*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_OUTSET)) = (0x01<<gpioPin);
    }

    /* SetGPIOStateHigh - sets the state of the GPIO Pin high
     *
     *     NOTE this action is only relevant if the GPIO is set as an output
     *
     *     Note: Since there is no overhead passing in a variable, this function call
     *       is about 25% faster than calls to SetGPIOState(val)
     *       a toggle rate of about 410Khz has been observed on a 16MHz V1
     *
     * */
    void YakIO_GPIO::SetGPIOStateHigh()
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return;

        // set the output state
        (*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_OUTSET)) = (0x01<<gpioPin);
    }

    /* SetGPIOStateLow- sets the state of the GPIO pin low
     *
     *     NOTE this action is only relevant if the GPIO is set as an output
     *
     *     Note: Since there is no overhead passing in a variable, this function call
     *       is about 25% faster than calls to SetGPIOState(val)
     *       a toggle rate of about 410Khz has been observed on a 16MHz V1
     *
     * */
    void YakIO_GPIO::SetGPIOStateLow()
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return;

        // set the output state
        (*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_OUTCLR)) = (0x01<<gpioPin);
    }

    /* ToggleGPIOState - toggles the state of the GPIO
     *
     *     NOTE this action is only relevant if the GPIO is set as an output
     *
     *     Note: Since there is overhead passing in a variable and also reading
     *      the state register this function has a toggle rate of about 100Khz
     *      on a 16MHz V1
     * */
    void YakIO_GPIO::ToggleGPIOState(void)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return;

        if(GetGPIOState()==0) SetGPIOState(1);
        else SetGPIOState(0);
    }

    /* SetGPIODriveMode - sets the drive mode of an output GPIO
     *
     * inputs:
     *    gpioDriveMode - the drive mode to set
     * */
    void YakIO_GPIO::SetGPIODriveMode(GPIOPinDriveMode gpioDriveMode)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return;

        // set the current register state
        (* (unsigned volatile *) (cnfRegisterAddress)) &= GPIO_CNF_REGISTER_DRIVEMODE_MASK;  // clear them all to 0
        (* (unsigned volatile *) (cnfRegisterAddress)) |= gpioDriveMode; // set them high where needed
    }

    /* GetGPIODir - gets the drive mode of a GPIO.
     *
     *     NOTE this queries the register NOT whatever value was set
     *     on object creation. Thus if something else changed this
     *     value you will see the true value possibly not what you set
     *
     * returns:
     *   a GPIOPinDriveMode  value indicating the current drive mode state
     * */
    GPIOPinDriveMode YakIO_GPIO::GetGPIODriveMode(void)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return S0S1;

        // get the current register state
        unsigned int registerState = (* (unsigned volatile *) (cnfRegisterAddress));  // Get register
        // conduct the test
        if((registerState & (~GPIO_CNF_REGISTER_DRIVEMODE_MASK)) == D0H1) return D0H1;
        else if((registerState & (~GPIO_CNF_REGISTER_DRIVEMODE_MASK)) == D0S1) return D0S1;
        else if((registerState & (~GPIO_CNF_REGISTER_DRIVEMODE_MASK)) == H0H1) return H0H1;
        else if((registerState & (~GPIO_CNF_REGISTER_DRIVEMODE_MASK)) == S0H1) return S0H1;
        else if((registerState & (~GPIO_CNF_REGISTER_DRIVEMODE_MASK)) == H0S1) return H0S1;
        else return S0S1;
    }

    /* SetGPIOPullUpDown - sets the pull up/down of an input GPIO
     *
     * inputs:
     *    gpioPullUpDown - the pull up/down to set
     * */
    void YakIO_GPIO::SetGPIOPullUpDown(GPIOPinPullUpDown gpioPullUpDown)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return;

        // Pins 0,1,2,5,11 are all hardwired on the microbit PCB to pullup
        // so we do not change anything in the registers
        if(gpioPin==Pin0) return;
        if(gpioPin==Pin1) return;
        if(gpioPin==Pin2) return;
        if(gpioPin==Pin5) return;
        if(gpioPin==Pin11) return;

        // set the current register state
        (* (unsigned volatile *) (cnfRegisterAddress)) &= GPIO_CNF_REGISTER_PULLUPDOWN_MASK;  // clear them all to 0
        (* (unsigned volatile *) (cnfRegisterAddress)) |= gpioPullUpDown; // set them high where needed
    }

    /* GPIOPinPullUpDown - gets the pull up/down of a GPIO.
     *
     *     NOTE this queries the register NOT whatever value was set
     *     on object creation. Thus if something else changed this
     *     value you will see the true value possibly not what you set
     *
     * returns:
     *   a GPIOPinPullUpDown  value indicating the current pull up/down state
     * */
    GPIOPinPullUpDown YakIO_GPIO::GetGPIOPullUpDown(void)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return PullUpDownDisabled;

        // Pins 0,1,2,5,11 are all hardwired on the microbit PCB to pullup
        // so we report this state as that
        if(gpioPin==Pin0) return PullUpDownPullup;
        if(gpioPin==Pin1) return PullUpDownPullup;
        if(gpioPin==Pin2) return PullUpDownPullup;
        if(gpioPin==Pin5) return PullUpDownPullup;
        if(gpioPin==Pin11) return PullUpDownPullup;

        // get the current register state
        unsigned int registerState = (* (unsigned volatile *) (cnfRegisterAddress));  // Get register
        // conduct the test
        if((registerState & (~GPIO_CNF_REGISTER_PULLUPDOWN_MASK)) == PullUpDownPulldown) return PullUpDownPulldown;
        else if((registerState & (~GPIO_CNF_REGISTER_PULLUPDOWN_MASK)) == PullUpDownPullup) return PullUpDownPullup;
        else return PullUpDownDisabled;
    }

    /* SetGPIODir - sets the direction (input or output) of a GPIO
     *
     * inputs:
     *    gpioPinDir - a GPIOPinDir value indicating the direction
     * */
    void YakIO_GPIO::SetGPIODir(GPIOPinDir gpioPinDir)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return;

        // set the current register state
        (* (unsigned volatile *) (cnfRegisterAddress)) &= GPIO_CNF_REGISTER_DIRECTION_MASK;  // clear them all to 0
        (* (unsigned volatile *) (cnfRegisterAddress)) |= gpioPinDir; // set them high where needed

        // since we are a dedicated GPIO port in this class set the input buffer connect mode appropriately
        SetGPIOInputConnect(PinInputBufferDis);
    }

    /* GetGPIODir - gets the direction (input or output) of a GPIO.
     *
     *     NOTE this queries the register NOT whatever value was set
     *     on object creation. Thus if something else changed this
     *     value you will see the true value possibly not what you set
     *
     * returns:
     *   a GPIOPinDir value indicating the current direction state
     * */
    GPIOPinDir YakIO_GPIO::GetGPIODir(void)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return PinDirInput;

        // get the current register state
        unsigned int registerState = (* (unsigned volatile *) (cnfRegisterAddress));  // Get register
        // conduct the test
        if((registerState & (~GPIO_CNF_REGISTER_DIRECTION_MASK)) == 0) return PinDirInput;
        else return PinDirOutput;
    }

    /* SetGPIODriveMode - sets the input buffer connect state of an input GPIO
     *
     * inputs:
     *    gpioPinConnect - a GPIOInputConnectr value indicating the state
     * */
    void YakIO_GPIO::SetGPIOInputConnect(GPIOInputConnect gpioPinConnect)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return;

        // set the current register state
        (* (unsigned volatile *) (cnfRegisterAddress)) &= GPIO_CNF_REGISTER_INPUTCONN_MASK;  // clear them all to 0
        (* (unsigned volatile *) (cnfRegisterAddress)) |= gpioPinConnect; // set it high where needed
    }

    /* GetGPIODir - gets the direction of a GPIO.
     *
     *     NOTE this queries the register NOT whatever value was set
     *     on object creation. Thus if something else changed this
     *     value you will see the true value possibly not what you set
     *
     * returns:
     *   a GPIOInputConnect value indicating the current direction state
     * */
    GPIOInputConnect YakIO_GPIO::GetGPIOInputConnect(void)
    {
        // never operate if we are not initialised
        if(isInitialised!=1) return PinInputBufferEna;

        // get the current register state
        unsigned int registerState = (* (unsigned volatile *) (cnfRegisterAddress));  // Get register
        // conduct the test
        if((registerState & (~GPIO_CNF_REGISTER_INPUTCONN_MASK)) == 0) return PinInputBufferDis;
        else return PinInputBufferEna;
    }



