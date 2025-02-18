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

#include "LEDAddress.h"    
    
    /* Constructor - does nothing
     * 
     * */
    LEDAddress::LEDAddress()
    {
    }
    
    /* Constructor - initializes the object
     * 
     * inputs:
     *    rowValIn - the row value
     *    colValIn - the column value
     * */
    LEDAddress::LEDAddress(unsigned int rowValIn, unsigned int colValIn)
    {
        // set the member variables
        rowVal = rowValIn;
        colVal = colValIn;        
    }
    
    /* GetRowVal(void) - get the row value
     * 
     * returns
     *   the row value
     * */
    unsigned int LEDAddress::GetRowVal(void)
    {
        return rowVal;
    }

    /* GetColVal(void) - get the col value
     * 
     * returns
     *   the col value
     * */
    unsigned int LEDAddress::GetColVal(void)
    {
        return colVal;
    }
