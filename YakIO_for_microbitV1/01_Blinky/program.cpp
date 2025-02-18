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

// The YakIO library is designed to abstract away most of the complications involved in getting a C++ program to compile and run 
// on the BBC microbit.

// This is the first code in the user directory that is called by the YakIO library. There are quite a few other things that have 
// happened before this point but it is not necessary to know about that in order to use the YakIO library. By all means have a 
// look if you wish. The YakIO.cpp file over in the YakIO source is the place to start - it has been extensively commented.

// This file is largely boiler plate. The function name CreateMainObject() is fixed - the YakIO startup routines expect that. After
// that it is up to you what you do in here. You don't have to use the YakIO classes if you don't want to - you could write your 
// own bare metal code. 

// Having said that, the YakIO classes are available if you wish. The way to use them is to create a class, instantiate it here and 
// then call a function in that class to kick things off. This function should never return - your code should cycle repeatedly in
// that loop. 

// You can see this being done below. The Main class is defined in the users Main.h file and the code for the MainLoop() member 
// function is defined in the users Main.cpp file. The Main class is instantiated and the MainLoop function is called.

// WARNING!!!
// WARNING!!!
// WARNING!!!

// Whatever you do, do NOT instantiate a class on the heap if that class has a constructor - even a default one. Constructors will
// NOT be run under those circumstances. Instantiating a class, in another class at runtime as part of code execution is perfectly OK, 
// the constructors will be run as expected. 
//
// Review the "03_Danger" sample code to see the bad things that happen if you create classes with constructors on the heap.



/* CreateMainObject - instantiate the softwares primary object (a class named Main() by default) and call its main loop function 
 *    to perform the programs operations
 * 
 *    Note: this is kind of the same way C# kicks everything off.
 * */
extern "C" void CreateMainObject(void)
{        
    // create the Main Class, the user provides this
    Main mainObj {};
    
    // run the main loop. The code should never return from 
    // this call. Cycle in here forever! You, the user, 
    // add your code inside the MainLoop() function
    mainObj.MainLoop();
    
    // the above call must never return. If we do, just sit in a loop forever
    while(1) {}
}

