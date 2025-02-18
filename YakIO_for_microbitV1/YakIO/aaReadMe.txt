The YakIO Library 

YakIO is an open source library and example compilation toolchain which 
is intended to enable the creation C++ based executables for the BBC 
micro:bit microcontroller.

The YakIO library and example code is released under the MIT license. As
is stated everywhere in the source code, there is no warranty that the 
software is bug free or that the software is suitable for any purpose. 

You use the YakIO library and example code entirely at your own risk! 

The home page for the YakIO library can be found at:
   http://www.OfItselfSo.com/YakIO

The YakIO Library is entirely C++ code. It has been designed to encapsulate
the peripherals and abstract away as much of the problematic parts of 
writing C++ code for the microbit as is possible. 

This branch of the YakIO Library is only for the BBC microbit V1. It will 
NOT work on the V2 version. The V2 uses a different CPU and there are 
significant differences.

Also of considerable use is the provision of a simple and well documented
compilation tool chain. Part of the previously mentioned encapsulation is  
the removal of many of the components that might "break" a compile and  
link operation. For example, there is no makefile. Make is tricky and 
problematic to install on Windows (particularly for the inexperienced) 
and it is just not necessary to get started. Instead, easily readable  
batch files have been provided to enable single command compilation and 
and link. True, they always recompile everything, but this operation is 
so fast it really makes no practical difference. 

Please be aware that when you use YakIO you are programming bare metal. 
There is no operating system, no Mbed, no DAL/CODAL so you don't have 
the things like processes, malloc() or free() etc that you would typically 
get from the system libraries. Actually, you can have that sort of 
functionality but you have to approach it differently. Bare metal code has
the nice advantage that the output binary is much smaller in size. 

Please be aware that the YakIO Examples form a kind of tutorial. Each 
project demonstrates some new features. You really should review each
example project in order because they are cumulative. Techniques that 
are discussed in a prior example might be used but not explained in 
subsequent examples. 

Each example directory has an aaReadMe.txt file which explains what the 
example code does. You really should read this file. Each is specific to 
the example code in that directory - they are not all the same.
   
By itself, the YakIO Library cannot do anything. You need to create your own
code and link against it. The example folders in this directory provide 
well commented demonstrations of how to do this.

The YakIO Library ships with pre-compiled object files. There is no 
.a library file as you might expect. The creation of a .a library file and 
linking against it corrupts the startup code for the microbit and nothing
runs. Despite repeated attempts, no workaround for this issue has, as yet, 
been found. You have to directly link against the .o object files. Fortunately, 
this is easily automated and all of the example code demonstrate how to
do this.

If you wish to recomple the YakIO Library you can easily do so by opening 
a command prompt to the top of the YakIO directory (where this file is located)
and running the CompileYakIO.bat file you find there.

Other things you need to know: 

  1) The assumption in the CompileYakIO.bat file is that it is being run 
     on a Windows 10 or 11 system. However, seeing as how it is cross 
     compiling (generating code for one type of CPU on another) this code
     will work fine if compiled on Linux or Apple platforms with possibly 
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
             | 01_Blinky
             | 02_BetterBlinky
             | 03_Danger
             
     This is how everything is structured when downloaded from the GitHub repo.
   
  5) See the acQuickStart.txt file for a step-by-step set of instructions 
     regarding how to compile up your first bare metal C++ program using YakIO.
     

