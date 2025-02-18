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

#ifndef YAKIO_LEDARRAY_H
#define YAKIO_LEDARRAY_H

#include "YakIO.h"
#include "YakIO_Utils.h"
#include "YakIO_GPIO.h"

/* The onboard LEDS are highly multiplexed. In order
 * to show an image on the 5x5 grid we need to send to the 
 * GPIO_out register a sequence of 3 bytes with a short delay in between
 * each byte. The viewer does not notice this. 
 * 
 * See the "Basics of LEDs" section at https://spivey.oriel.ox.ac.uk/bare metal/X2000
 * 
 * In YakIO this sequential quick display is normally done by calling 
 * RefreshLEDArray() in succession from a heartbeat function driven by a timer.
 * 
 * */
#define NUM_GPIOROWS_IN_LED_IMAGE 3
#define NUM_LEDS_IN_ARRAY 25

// this defines all bits used for the rows and columns of the LED array
// the lowest 9 bits are Cols 1 to 9 in ascending order and the highest
// three bits are rows 1 to 3 in ascending order
#define GPIO_LED_MASK  0b000000000000000000001111111111110000

// the row and column bits for each row
#define LEDROW1 0b00000000000000000011111111110000
#define LEDROW2 0b00000000000000000101111111110000
#define LEDROW3 0b00000000000000001001111111110000

// the number of SHL operations to get a 1 into the proper bit position. 
// Just used for clarity of programming
#define C1_SHL  4
#define C2_SHL  5
#define C3_SHL  6
#define C4_SHL  7
#define C5_SHL  8
#define C6_SHL  9
#define C7_SHL  10
#define C8_SHL  11
#define C9_SHL  12

class YakIO_LEDARRAY 
{ 
  private: 
    // there are 3 row pins for the LEDs. Every call to 
    // RefreshLEDArray shows the next row.NUM_LEDS_IN_ARRAY
    unsigned int currentRow =0;
    // this is the image that backs the LED display
    unsigned char backingStore[25] = {};
    // this is a three integer binary of the backing store which actually sets
    // the registers for the LEDS. If the backingStore changes this _must_be 
    // set with a call to ProcessBackingStore
    unsigned int backingStoreRegisterSettings[NUM_GPIOROWS_IN_LED_IMAGE];
    // processes the backingStore array into 3 integer values suitable for setting
    // the LED registers
    void ProcessBackingStore(void);
    // sets up the LED GPIO registers
    void SetAllLEDGpios(void);

  public:
    // Constructor to initialize YakIO_LEDARRAY object
    YakIO_LEDARRAY();
    void ClearImage();
    void SetBinaryImage(const unsigned char bImg[]);
    void RefreshLEDArray();
    void SetLEDState(unsigned row, unsigned col, unsigned val);
    void ToggleLEDState(unsigned row, unsigned col);
    unsigned GetLEDState(unsigned row, unsigned col);
    void SetColumn(unsigned int colNum, unsigned char zAxisMode[]);

};


#endif
