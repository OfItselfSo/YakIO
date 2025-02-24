The 07_Random Example 

YakIO is an open source library and example compilation toolchain which 
is intended to enable the creation C++ programs for the BBC micro:bit
microcontroller.

The YakIO library and example code is released under the MIT license. As
is stated everywhere in the source code, there is no warranty that the 
software is bug free or that the software is suitable for any purpose. 

You use the YakIO library and example code entirely at your own risk! 

Please be aware that the YakIO Examples form a kind of tutorial. Each 
project demonstrates some new features. You really should review each
example project because they are cumulative. Techniques that are discussed
in a prior example might not be commented on in subsequent examples.

This folder contains the source code for the 07_Random C++ program 
which is designed to access the random number perpipheral on the 
Nordic nRF51822 CPU. It uses this value to set the 5x5 LED array to a 
rough 0-24 approximation of the random number value. 

Other specific things demonstrated in this example code which you might 
wish to look out for:

  1) The inheritance of the Main() class from the YakIO_CALLBACK interface
     class which enables it to receive callbacks.
  2) The instantiation of YakIO library objects as class member variables
     so that the code executing in the callback functions has access to
     them. 
  2) The use of polling to obtain the new random number value.
  4) Alternatively, the use of an interrupt driven callback function to 
     obtain a new random number value as soon as it is ready.

The home page for the YakIO library can be found at:
   http://www.OfItselfSo.com/YakIO
   
Things you need to know: 

  1) The assumption in this example is that it is being run on a Windows 
     10 or 11 system. However, seeing as how it is cross compiling 
     (generating code for one type of CPU on another) this code will 
     work fine if compiled on Linux or Apple platforms with possibly 
     only minor tweaks required to the compilation tool chain.
     
  2) The arm-none-eabi-gcc compiler and other tools are absolutely necessary.
     They are free! The one used for development was the Windows installer
     
        gcc-arm-none-eabi-4_9-2015q2-20150609-win32.exe 
        
     available from the GNU Arm Embedded Toolchain website
     
        https://launchpad.net/gcc-arm-embedded/+download
        
     There are later versions, but this was not noticed until fairly late
     in the development process so the decision was made to stay with 
     the one known to work. 
     
  3) The arm-none-eabi-gcc.exe compiler and arm-none-eabi-objcopy.exe 
     converter should be on the path. Either that or a full path will 
     have to be specified when compiling. If you get it right, the following 
     command should always work from the Windows command prompt or powershell:
     
     > arm-none-eabi-gcc.exe --version
     
        arm-none-eabi-gcc.exe (GNU Tools for ARM Embedded Processors) 4.9.3 20150529 (release) [ARM/embedded-4_9-branch revision 224288]
        Copyright (C) 2014 Free Software Foundation, Inc.

  4) The batch scripts that build the example code assume that the user code 
     directory is at the same level as the YakIO library. In other words
         SomeDir
           |
           YakIO_for_microbitV1
             |
             | YakIO
             |   | Include
             |   | Objects              
             |   | Source              
             |
             | 07_Random
     This is how it is structured when downloaded from the GitHub repo.
     
  5) The YakIO Objects directory should contain a full complement of .o files
     There should be one for every .cpp file in the Source directory. If those
     files are not there, then create them by opening a command prompt to the 
     to YakIO directory and running the CompileYakIO.bat file you find there.
     
  6) The Main.h and Main.cpp are the only files of interest to the user in this
     example. In particular, the program.cpp file is boiler plate and there 
     is usually no need to edit it. 
    
  7) Open the Main.h and Main.cpp files and understand the contents. For
     experienced C++ programmers, this code will seem trivial but the 
     techniques used in there to work with YakIO objects will be used
     in subsequent example programs without much discussion so it pays to 
     have a working understanding of what is going on. 
   
  8) Also have a look at the CompileProgram.bat script to see what it does

  9) When ready, run the CompileProgram.bat script. It should complete without
     errors. You execute this file by opening a cmd or powershell prompt  
     to the top of the 07_Random directory and running the 
     CompileProgram.bat script.
   
 10) The successful run of the CompileProgram.bat script will have left a 
     Main.hex file in the directory. This is the program for the microbit. 
     Just plug the microbit into a USB port on the PC - it will appear as
     a drive in Windows Explorer. Then drag and drop the Main.hex file onto 
     the microbit. It should automatically load and run and, after a bit of
     manipulation of the LED on the 5 x 5 array you should see the software
     settle down and the LEDs should light up indicating the range of the 
     latest random number scaled down to a range of 0-24. This image will
     change very 2 seconds.
     
 11) If you look at the size of the Main.hex file you will see that it is 
     very small. Actually, the size is half of what you see since the Intel 
     Hex format it is encoded in effectively doubles the size. This small
     size is a consequence of the fact that there is no operating system.
     
     You are now programming bare metal in C++! Good luck.
