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

#include "YakIO_LEDARRAY.h"

    /* constructor
     * */
    YakIO_LEDARRAY::YakIO_LEDARRAY()
    {        
        // ensure the backingStore is initialized
        ClearImage();
        // set the gpios that drive the LED appropriately
        SetAllLEDGpios();
    }
    
    /* SetAllLEDGpios - Set all relevant GPIO registers to states 
     *     appropriate to driving the LEDs
     * 
     * */
    void YakIO_LEDARRAY::SetAllLEDGpios(void)
    {
        // we use the SET register so we can just set these bits directly
        unsigned int bitVal = LEDROW1 | LEDROW2 | LEDROW3;
                               
        // set the value in the appropriate register
        (*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_DIRSET)) = bitVal;
    }
    
    /* ClearImage -- immediately set image to blank 
     * 
     * */
    void YakIO_LEDARRAY::ClearImage()
    {
        // set it all to 0
        for (unsigned int i=0; i<sizeof(backingStore); i++) backingStore[i] = 0; 
        // move it into the register words
        ProcessBackingStore();
        // call this three times to really get the image cleared down
        RefreshLEDArray();
        RefreshLEDArray();
        RefreshLEDArray();    
    }
    
    /* GetLEDState - gets the state of an individual led
     * 
     * inputs:
     *   row - the 1 based row must be 1<=row<=5
     *   col - the 1 based col must be 1<=col<=5
     * 
     * returns:
     *   the state 1 or 0 of the LED at the (row,col)
     * */
    unsigned int YakIO_LEDARRAY::GetLEDState(unsigned row, unsigned col)
    {
        // rows and cols are 1 based here
        if (row <= 0) return -1;
        if (col <= 0) return -1;
        if (row > 5) return -1;
        if (col > 5) return -1;
        // get the value
        return backingStore[((row-1)*5)+(col-1)];
    }
    
    /* SetLEDState - sets the state of an individual led
     * 
     * inputs:
     *   row - the 1 based row must be 1<=row<=5
     *   col - the 1 based col must be 1<=col<=5
     *   val - the value to set must be 0 or 1
     * 
     * */
    void YakIO_LEDARRAY::SetLEDState(unsigned row, unsigned col, unsigned val)
    {
        // rows and cols are 1 based here
        if (row <= 0) return;
        if (col <= 0) return;
        if (row > 5) return;
        if (col > 5) return;
        if (val > 1) return;
        // set the value
        backingStore[((row-1)*5)+(col-1)]=val;
        // move it into the register words
        ProcessBackingStore();
    }

    /* ToggleLEDState - toggles the state of an individual LED
     * 
     * inputs:
     *   row - the 1 based row must be 1<=row<=5
     *   col - the 1 based col must be 1<=col<=5
     * 
     * */
    void YakIO_LEDARRAY::ToggleLEDState(unsigned row, unsigned col)
    {
        // rows and cols are 1 based here
        if (row <= 0) return;
        if (col <= 0) return;
        if (row > 5) return;
        if (col > 5) return;
        // set the value
        if(backingStore[((row-1)*5)+(col-1)] == 0) backingStore[((row-1)*5)+(col-1)]=1;
        else backingStore[((row-1)*5)+(col-1)] = 0;
        // move it into the register words
        ProcessBackingStore();
    }
    
    /* SetBinaryImage - accepts a 25 char array and sets our backing store
     *       to those values. Does not automatically display the backing store
     * 
     * inputs:
     *   bImage  - a 25 byte unsigned char with 1's and 0's representing
     *                  the on and off state of the 25 LEDS
     * */
    void YakIO_LEDARRAY::SetBinaryImage(const unsigned char bImage[])
    {
        // copy the data into place
        for (unsigned int i=0; i<sizeof(backingStore); i++) backingStore[i] =  bImage[i]; 
        // move it into the register words
        ProcessBackingStore();
    }
    
    /* RefreshLEDArray - shows one row from whatever 5x5 image we have 
     *      set in the backingStore
     * 
     * */
    void YakIO_LEDARRAY::RefreshLEDArray()
    {
        // note we only do one LED row per call. This means it takes three
        // calls here to display one 5x5 led image. This makes it easy to 
        // get a consistent brightness on each row
        
        // we take care to just operate on the bits we are interested in, set them to 0 
        // and leave the others there is a special CLR register where writing a 1 to 
        // the appropriate positon will clear the bit
        (*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_OUTCLR)) = GPIO_LED_MASK;
        // now if the bits we are interested in are to be 1 we set them using the special SET register
        (*(unsigned volatile *) (REGISTER_GPIO+GPIOREG_OFFSET_OUTSET)) =  backingStoreRegisterSettings[currentRow];
        currentRow++;
        if (currentRow >= NUM_GPIOROWS_IN_LED_IMAGE) currentRow=0;
    }
    
    /* ProcessBackingStore - converts the backingStore into 3 integer words suitable
     * for setting in the LED registers.
     * 
     * Must be called each time the backingStore is changed or the backingStore changes will have no
     * effect
     *  
     * */
    void YakIO_LEDARRAY::ProcessBackingStore(void)
    {        
        // the first row
        backingStoreRegisterSettings[0]= LEDROW1  ^ (backingStore[0]<<C1_SHL)  ^ (backingStore[2]<<C2_SHL)  ^ (backingStore[4]<<C3_SHL)  ^ (backingStore[11]<<C9_SHL)
                     ^ (backingStore[15]<<C8_SHL) ^ (backingStore[16]<<C7_SHL) ^ (backingStore[17]<<C6_SHL) ^ (backingStore[18]<<C5_SHL) ^ (backingStore[19]<<C4_SHL);                  
        // the second row
        backingStoreRegisterSettings[1]= LEDROW2  ^ (backingStore[1]<<C4_SHL)  ^ (backingStore[3]<<C5_SHL)  ^ (backingStore[10]<<C2_SHL) ^ (backingStore[12]<<C3_SHL)
                     ^ (backingStore[14]<<C1_SHL) ^ (backingStore[21]<<C7_SHL) ^ (backingStore[23]<<C6_SHL);
        // the third row
        backingStoreRegisterSettings[2]= LEDROW3  ^ (backingStore[5]<<C4_SHL)  ^ (backingStore[6]<<C5_SHL)  ^ (backingStore[7]<<C6_SHL)  ^ (backingStore[8]<<C7_SHL)
                     ^ (backingStore[9]<<C8_SHL)  ^ (backingStore[13]<<C9_SHL) ^ (backingStore[20]<<C3_SHL) ^ (backingStore[22]<<C1_SHL) ^ (backingStore[24]<<C2_SHL);    
    }

    /* SetColumn - sets the state of column on the display
     * 
     * inputs:
     *   col - the 1 based col must be 1<=col<=5
     *   colArray - an array of 5 values all (1 or 0) for (on or off)
     * 
     * */
    void YakIO_LEDARRAY::SetColumn(unsigned int colNum, unsigned char colArray[])
    {
        if (colNum <= 0) return;
        if (colNum > 5) return;
        for(int i=0; i<5; i++)
        {
            if(colArray[i]==0) backingStore[(i*5)+(colNum-1)] = 0;
            else backingStore[(i*5)+(colNum-1)] = 1;
        }
        // move it into the register words
        ProcessBackingStore();
    }

