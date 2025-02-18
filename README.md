# YakIO

YakIO is an open source library which is intended to enable the creation C++ based executables for the BBC micro:bit microcontroller. The YakIO Library is entirely C++ code. It has been designed to encapsulate the functionality and abstract away as much of the problematic parts of writing C++ programs for the microbit as is possible. 

Also of interest is the provision of a simple and well documented compilation tool chain. Part of the previously mentioned encapsulation is the removal of many of the components that might "break" a compile and link operation. 

The YakIO Library is bare metal. There is no operating system, no Mbed, no DAL/CODAL or anything like that. This means you don't have the things like processes, malloc() or free() etc that you would typically get from the system libraries. Actually, you can have that sort of functionality but you have to approach it differently. Bare metal code has the nice advantage that the output binary is much smaller in size. YakIO is also very fast with the ability to toggle a pin on the microbit card edge connector at 2.31 Mega Hz.

The example programs included with the YakIO library form a sequential tutorial. These examples demonstrate how to implement your own C++ program and provide a template you can adapt. Each project demonstrates some new features - you really should review each example project in order because they are cumulative. Techniques that are discussed in a prior example might be used but not explained in subsequent examples. As with the YakIO Library source, the example code contains quite detailed comments explaining what is going on.

The YakIO Project is open source and released under the MIT License. The home page for this project can be found at [http://www.OfItselfSo.com/YakIO](http://www.OfItselfSo.com/YakIO) and that page contains other useful advice and assistance.