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

#ifndef YAKIO_CALLBACK_H
#define YAKIO_CALLBACK_H

// Usually the user gets the timer interrupt in the form of a 
// callback to a function they have specified. The function 
// that gets called should be directly related (hard coded) 
// to this ID
enum CALLBACK_ID {
    CALLBACK_NONE,  
    CALLBACK_0,       // Callback0() 
    CALLBACK_1,        // Callback1()
    CALLBACK_2,        // Callback2()
    CALLBACK_3,        // Callback3()
    HEARTBEAT       // special case, Heartbeat()
};

/* YakIO_CALLBACK - a virtual class  which enables classes that 
 *   inherit from it to receive callbacks in specially named member functions. 
 * 
 *   This class is intended to operate as an Interface. 
 * */
class YakIO_CALLBACK
{ 
    private:
 
    public:
        // these are the callback functions. The child class only needs to
        // implement the ones it actually uses. Use the "override" qualifier
        // to make sure the childs implementation gets executed
        virtual void Callback0() {};
        virtual void Callback1() {};
        virtual void Callback2() {};
        virtual void Callback3() {};
        virtual void Heartbeat() {};
};

#endif
