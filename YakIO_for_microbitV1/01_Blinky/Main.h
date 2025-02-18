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

#ifndef MAIN_H
#define MAIN_H

#include "YakIO.h"

/* Main - your program starts with a call to MainLoop() and all 
 *        global objects should be owned by this class
 * 
 *        WARNING: Do NOT declare class variables on the heap (ie outside of a class)! 
 *        The constructor will NOT be run when the object is created and member variables
 *        will NOT be initialized.
 * 
 *        Instantiate all classes inside some other class. If a class is instantiated
 *        at runtime (as opposed to compile time) then the constructor will run.
 * 
 *        You might wish to review the "03_Danger" sample code to see the bad 
 *        things that happen if you create classes with constructors on the heap.
 *       
 * */
class Main // you can inherit from other classes if you wish. See the 02_BetterBlinky example
{ 
    private:
        // there are no variables declared in here in this example 
        // but you can do so if you wish.
    public:
        // this needs to be public because the CreateMainObject() function in program.cpp 
        // calls it. See that code to better understand what is going on here.
        void MainLoop(void);
 
};

#endif
