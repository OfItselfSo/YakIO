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

#ifndef YAKIO_GPIO_H
#define YAKIO_GPIO_H
#include "YakIO.h"

// GPIO REGISTER SPECIFIC SECTION
#define GPIOREG_OFFSET_OUT 0x504           // Write GPIO port
#define GPIOREG_OFFSET_OUTSET 0x508        // Set individual bits in GPIO port
#define GPIOREG_OFFSET_OUTCLR 0x50C        // Clear individual bits in GPIO port
#define GPIOREG_OFFSET_IN 0x510            // Read GPIO port
#define GPIOREG_OFFSET_DIR 0x514           // Direction of GPIO pins
#define GPIOREG_OFFSET_DIRSET 0x518        // DIR set register
#define GPIOREG_OFFSET_DIRCLR 0x51C        // DIR clear register
#define GPIOREG_OFFSET_PIN_CNF_BASE 0x700  // Base of the CNF[?] registers
#define GPIOREG_OFFSET_PIN_CNF0 0x700      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF1 0x704      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF2 0x708      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF3 0x70C      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF4 0x710      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF5 0x714      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF6 0x718      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF7 0x71C      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF8 0x720      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF9 0x724      // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF10 0x728     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF11 0x72C     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF12 0x730     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF13 0x734     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF14 0x738     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF15 0x73C     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF16 0x740     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF17 0x744     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF18 0x748     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF19 0x74C     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF20 0x750     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF21 0x754     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF22 0x758     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF23 0x75C     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF24 0x760     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF25 0x764     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF26 0x768     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF27 0x76C     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF28 0x770     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF29 0x774     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF30 0x778     // Configuration of GPIO pins
#define GPIOREG_OFFSET_PIN_CNF31 0x77C     // Configuration of GPIO pins

// some useful defines
#define GPIO_CNF_REGISTER_INPUTCONN_MASK  0xFFFFFFFD
#define GPIO_CNF_REGISTER_DIRECTION_MASK  0xFFFFFFFE
#define GPIO_CNF_REGISTER_DRIVEMODE_MASK  0xFFFFF8FF
#define GPIO_CNF_REGISTER_PULLUPDOWN_MASK 0xFFFFFFF3

// the direction (input or output) of the GPIO pin
// note the values here are carefully chosen to represent
// the values we set in the CFG[?] register for the
// specified GPIO
enum GPIOPinDir {
    PinDirInput=0x00,
    PinDirOutput=0x01
};

// the input buffer connection state of the GPIO pin
// note the values here are carefully chosen to represent
// the values we set in the CFG[?] register for the
// specified GPIO
enum GPIOInputConnect {
    PinInputBufferDis=0b000,
    PinInputBufferEna=0b010
};

// the pull down/up state of the GPIO pin
// note the values here are carefully chosen to represent
// the values we set in the CFG[?] register for the
// specified GPIO
enum GPIOPinPullUpDown {
     PullUpDownDisabled =0b0000,        // No pull
     PullUpDownPulldown =0b0100,        // Pull down on pin
     PullUpDownPullup   =0b1100         // Pull up on pin
};

// the drive mode, note the values here are carefully chosen
// to represent the values we set in the CFG[?] register
// for the specified GPIO
enum GPIOPinDriveMode {
    S0S1=0x000,     // Standard '0', standard '1'
    H0S1=0x100,     // High drive '0', standard '1'
    S0H1=0x200,     // Standard '0', high drive '1'
    H0H1=0x300,     // High drive '0', high 'drive '1''
    D0S1=0x400,     // Disconnect '0' standard '1'
    D0H1=0x500      // Disconnect '0', high drive '1'
};


// only GPIOs exposed as pins on the card edge of microbit V1 are listed
// the number associated with the enum represents the GPIO
// number and is also the SHL value to set/reset the bits in the GPIO registers
enum GPIOPin {
    Pin0=3,
    Pin1=2,
    Pin2=1,
    Pin3=4,                 // also LED Col1
    Pin4=5,                 // also LED Col2
    ButtonA=17,             // also Pin5
    Pin5=17,                // also BUTTON A
    Pin6=12,                // also LED Col9
    Pin7=11,                // also LED Col8
    Pin8=18,
    Pin9=10,                // also LED Col7
    Pin10=6,                // also LED Col3
    ButtonB=26,             // also PIN_11
    Pin11=26,               // also BUTTON B
    Pin12=20,               // marked as reserved accessiblity but is usable
    Pin13=23,
    Pin14=22,
    Pin15=21,
    Pin16=16,
    Pin19=0,
    Pin20=30
};

/* YakIO_GPIO - a class to represent and encapsulate GPIO pin direction
 *     information and actions
 * */
class YakIO_GPIO
{
  private:
      unsigned int cnfRegisterAddress=0;
      enum GPIOPin gpioPin;
      unsigned int isInitialized =0;

  public:
      // Constructor to initialize YakIO_GPIO object
      YakIO_GPIO(enum GPIOPin gpioPinIn, enum GPIOPinDir gpioPinDir);
      enum GPIOPin GetGPIOPin(void);
      void SetGPIODriveMode(GPIOPinDriveMode gpioDriveMode);
      GPIOPinDriveMode GetGPIODriveMode(void);
      void SetGPIODir(GPIOPinDir gpioPinDir);
      GPIOPinDir GetGPIODir(void);
      unsigned int GetGPIOState(void);
      void SetGPIOPullUpDown(GPIOPinPullUpDown gpioPullUpDown);
      GPIOPinPullUpDown GetGPIOPullUpDown(void);
      void SetGPIOState(unsigned int gpioState);
      void SetGPIOStateHigh();
      void SetGPIOStateLow();
      void SetGPIOInputConnect(GPIOInputConnect gpioPinConnect);
      void ToggleGPIOState();
      GPIOInputConnect GetGPIOInputConnect(void);

};
#endif
