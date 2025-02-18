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

// This file is part of the YakIO C++ library for the BBC microbit. This code only supports V1 of the BBB microbit - the V2 version 
// contains significant differences (a different CPU!) and this code will NOT run on that platform.
//
// There should be no need for the general user to edit this code. All user edits take place within a Main() class in a separate 
// user directory. See the code in the example directories for how this works.
//
// The _StartYakIO() function below is the first thing executed when the nRF51822 CPU of the microbit begins execution. Embedded 
// programming requires initialization to be performed prior to the execution of any user code. One of the goals of the YakIO
// library is to remove the actions of these setup and configuration operations from the view of the individual user.
//
// Consequently, once the setup is done, the _StartYakIO() code will call the CreateMainObject() function. This function is in a
// .cpp file in the users code directory (usually program.cpp). The examples create an object of type Main and then call a member 
// function named MainLoop() in the new Main object. The MainLoop() function is expected to never return. MainLoop() should perform
// its actions continuously until power is lost. 
//
// The user supplies the Main object and MainLoop() code (template objects are provided for this purpose). It is compiled into an
// object file and combined with the YakIO objects at link time. Inside the Main class, the other classes available in the YakIO  
// library can be created and used as needed as can user defined classes.
//
// Please be aware that there is NO operating system here. The C++ code is running on bare metal. This means common things like 
// processes are not available. Also, there is no memory management so things such as malloc() and free() are not available. Actually,
// you can have all of the that functionality but you have to approach it differently.
//
// IMPORTANT NOTE: If you instantiate a class that uses a constructor on the heap (i.e. outside of a class) the constructor will NOT
//    be run. The identification and execution of such constructors is a pretty intricate dance on the part of the C++ libraries and
//    linker script and is just not available in bare metal programming. If you create objects as a class member variable then the 
//    constructor will run. You might wish to review the "03_Danger" sample code to see the many bad things that happen if you create 
//    classes with constructors on the heap.
// 
// Credits:
//   This startup code follows a predictable and fairly standard pattern. The bare bones of this code have been derived from 
//   Alex Taradov's open source code at:
//      https://github.com/ataradov/embedded/blob/master/examples/samd20/startup.c
//   Further insights were provided by Shawn D'Silva at:
//      https://www.shawndsilva.com/blog/embedded%20systems/writing-the-startup-code-and-linker-script-for-the-tm4c-from-scratch
//   and Mike Spivey at:
//      https://github.com/Spivoxity/bare metal-v1/blob/master/x08-heart/startup.c

#include "YakIO.h"
#include "YakIO_CLOCK.h"

extern "C" void _StartYakIO(void);
extern "C" void CreateMainObject(void); 

// These values are left unresolved at compile time and set later by the linker
// based on internal calculations in the linker script.
extern unsigned char __topOfInitializedDataSection__[];
extern unsigned char __bottomOfInitializedDataSection__[];
extern unsigned char __topOfUnInitializedDataSection__[];
extern unsigned char __bottomOfUnInitializedDataSection__[];
extern unsigned char __bottomOfCodeSection__[];
extern unsigned char __startOfStack__[];

/* startHFClock - start the High Frequency (HF) clock. This is the 16Mhz 
 *    crystal clock on the micobit
 * */
void startHFClock(void)
{
    // the HF clock seems to automatically start on the MicroBit but this is the 
    // standard way to do it so we will implement this code anyway in case it is
    // needed. Calling it does not seem to hurt anything. 
    
    // clear the HF Clock started event
    (*(unsigned volatile *) (REGISTER_CLOCK+CLOCKREG_OFFSET_HFCLKSTARTED)) = 0;
    // start the HF Clock
    (*(unsigned volatile *) (REGISTER_CLOCK+CLOCKREG_OFFSET_HFCLKSTART)) = 1;
    // wait for the clock to start
    while (1) 
    { 
        if ((*(unsigned volatile *) (REGISTER_CLOCK+CLOCKREG_OFFSET_HFCLKSTARTED)) !=0) break;
    }
}

/* nRF51822Config - configure the nRF51822 chip used in the microbit
 * */
void nRF51822Config(void)
{ 
    // the HF clock seems to automatically start on the MicroBit, but the call below 
    // is the way to do it if you need to do so. One would normally expect that, 
    // prior to this point, the CPU would be running on its internal LF clock
    
    //startHFClock();
    
    // copy initialized data out of the FLASH ROM and up into its allocated area of the RAM memory
    // we can run code from the ROM but we cannot (easily) write there. The linker expects these
    // values to be copied up into RAM by the startup routine. 
    //
    // Look up the differences between virtual memory address (VMA) and load memory address (LMA) 
    // if you want more information. https://mcyoung.xyz/2021/06/01/linker-script/ contains a good
    // summary
    unsigned int dataSize = __bottomOfInitializedDataSection__ - __topOfInitializedDataSection__;
    for (unsigned int i=0; i<dataSize; i++) __topOfInitializedDataSection__[i] = __bottomOfCodeSection__[i]; 
    
    // init our uninitialized variable space to 0. This is allocated in the RAM by the linker
    // but the linker does nothing with it.
    unsigned int bssSize = __bottomOfUnInitializedDataSection__ - __topOfUnInitializedDataSection__;
    for (unsigned int i=0; i<bssSize; i++) __topOfUnInitializedDataSection__[i] = 0;     
}

/* _StartYakIO() - this gets called from the initial address set in the vector 
 *          table below.
 * 
 *          This is where code execution starts but user code should be
 *          placed in the MainLoop() function of the Main object created 
 *          in here.
 * 
 * */
void _StartYakIO(void)
{    
    // configure the microcontroller
    nRF51822Config();
    
    CreateMainObject();    
    
    // the above call must never return. If we do, just sit in a loop
    while(1) {}
}

// READ THIS!!!!!
// READ THIS!!!!!
// READ THIS!!!!!

// TL;DR If you need an interrupt handler in your code and you define it with the exact 
// same name as the one used in the vector table at the bottom of this file, then it will be 
// called when appropriate, as if by magic, and you don't really need to concern yourself with 
// the details of how that happens.

//-----------------------------------------------------------------------------
// The above TL;DR not good enough for you? OK, here is a discussion as best as I can make it.
//
// The nRF51822 chip in the microbit is an ARM Cortex-M0+ design. This means it 
// expects to have the starting stack pointer placed at address 0x0000 in the 
// ram. This takes 4 bytes. The next 4 bytes are the address of the initial
// starting function. In this case it is our _StartYakIO() function. After that, there
// are 4 byte slots for the addresses of various system type functions. The
// functions of these slots are all hard coded. In other words, the CPU expects
// to see the address of the NMI_handler in the third 4 byte slot. The first 16 of
// the 4 byte slots are all used for this system like functionality.
//
// After the first 16 slots are filled, there are 32 4 byte slots for the addresses
// of the interrupt handler functions of the various peripherals. As before, the function 
// of these is hard coded - the slot used is dependent on a system assigned value 
// known as the IRQ_number (Interrupt Request Number) which in turn is derived
// mathematically from the location in the address space that peripheral occupies and
// that location, dear reader, is really hard coded since it is burnt into the silicon. 
//
// What this means is that (for example) the address of the function of the interrupt handler 
// for the TIMER0 peripheral (IRQ8) is always expected to be found at slot 16+8 or, 
// if you wish to do the math, it is expected to be found at address ((16+8)*4) == 96
// 
// So, we collect the addresses of all of these functions and place them in a vector 
// table. This is basically an array of addresses of the functions one after the 
// other, all in the correct order, with 0's as filler for the reserved slots. This 
// vector table essentially represents the contents of the first 192 bytes of 
// memory ((16+32)*4)starting at address 0x0000. 
// 
// You can see this table at the bottom of this file. It is named "__vectors[]"
//
//     void *__vectors[] __attribute((section(".vectors"))) .....
//
// The problem now is that the "__vectors[]" table needs to be carefully placed in RAM
// memory starting at address 0x0000. The one below could end up anywhere unless we 
// do some careful coordination with the linker to make it go in memory at 0x0000
//
// So we give it a identifier of ".vectors" - this is the purpose of the __attribute((section(".vectors")))
// text appended on the definition. When this file (YakIO.cpp) is compiled, a section in the
// .o file specially named ".vectors" will be present and the linker can find this. 
//
// The linker script contains a line 
// 
//         KEEP(*(.vectors))
// 
// right at the top of the very first section. The address at that point _is_ 0x0000 so 
// including it first, right at the start, puts the contents of the vector table (the list of 
// addresses of the appropriate functions) in memory right where we need it. 
//
// So far so good, now we have a list function addresses right where we want it and right where the 
// CPU expects it. But wait, this implies that we have to define all of those functions and 
// create bodies for them. But if we define the function in this file, then no other 
// class could use it. In other words, in order for the Timer class to handle the IRQ8 interrupt
// itself, it would have to create a IRQ_TIMER0_handler() function and then the temporary version 
// in here would have to be commented out so there wasn't a naming collision. 
//
// This isn't really very satisfactory. The ideal situation would be that if the Timer class created
// its own interrupt handler function named IRQ_TIMER0_handler() then that function would automatically 
// override the one in here. In other words it would be nice that if the linker ever saw a newer, better, 
// IRQ_TIMER0_handler() function it would know to ignore the dummy function in this class and insert 
// the address of the real IRQ_TIMER0_handler() function into the vector table instead.
//
// That is exactly what is going on. We have a function named irq_handler_dummy() which serves as a proxy 
// for all of the actual function names we don't care to actually define. If we then issue a statement like
// 
// __attribute__ ((weak, alias ("irq_handler_dummy"))) void IRQ_TIMER0_handler(void);
//
// This effectively creates a IRQ_TIMER0_handler() function that has the same address as the irq_handler_dummy 
// (which actually exists). But, since it is a weak alias, the linker will know that if it ever sees a better
// IRQ_TIMER0_handler() function it should really use that address for the vector table instead. 
//
// Below you can see the dummy handler at which we point a lot of functions we don't want to define right now
// and after that a list of weak aliased function names.
//
// Thus, the vector table gets filled with the addresses of functions (most of which will just be the address 
// of the irq_dummy_handler() function. If any of the handler functions are actually defined anywhere else, 
// then that address will be used in the vector table instead of the dummy. 
//
// NOTE that the irq_handler_dummy() function just contains an endless while loop. If it is ever called (for example
// you trigger an interrupt without defining your own handler for it) then your program will be effectively hung since
// the loop will never exit. You could replace this will something more to your liking - some people like to flash the 
// 5x5 LED array - see the Mike Spivey link above.

/* irq_handler_dummy - handle any interrupt not handled elsewhere 
 * */
extern "C" void irq_handler_dummy(void)
{
  // we do not know what to do so we just sit here forever
  while (1);
}

// now weakly define a handler for each interrupt and point it at our 
// dummy handler. This reserves the names but allows other code to override it.
// See the big long discussion above for how this works.
#define WEAK_ALIAS __attribute__ ((weak, alias ("irq_handler_dummy")))

WEAK_ALIAS void NMI_handler(void);
WEAK_ALIAS void HARD_FAULT_handler(void);
WEAK_ALIAS void SVC_handler(void);
WEAK_ALIAS void PENDSVC_handler(void);
WEAK_ALIAS void SYSTICK_handler(void);
WEAK_ALIAS void IRQ_UART0_handler(void);
WEAK_ALIAS void IRQ_TIMER0_handler(void);
WEAK_ALIAS void IRQ_TIMER1_handler(void);
WEAK_ALIAS void IRQ_TIMER2_handler(void);
WEAK_ALIAS void IRQ_CLOCK_POWER_MPU_handler_handler(void);
WEAK_ALIAS void IRQ_RADIO_handler(void);
WEAK_ALIAS void IRQ_SPI0_TWI0_handler(void);
WEAK_ALIAS void IRQ_SPI1_SPIS1_TWI1_handler(void);
WEAK_ALIAS void IRQ_GPIOTE_handler(void);
WEAK_ALIAS void IRQ_ADC_handler(void);
WEAK_ALIAS void IRQ_RTC0_handler(void);
WEAK_ALIAS void IRQ_TEMP_handler(void);
WEAK_ALIAS void IRQ_RNG_handler(void);
WEAK_ALIAS void IRQ_ECB_handler(void);
WEAK_ALIAS void IRQ_AAR_CCM_handler(void);
WEAK_ALIAS void IRQ_WDT_handler(void);
WEAK_ALIAS void IRQ_RTC1_handler(void);
WEAK_ALIAS void IRQ_QDEC_handler(void);
WEAK_ALIAS void IRQ_LPCOMP_handler(void);
WEAK_ALIAS void IRQ_SWI0_handler(void);
WEAK_ALIAS void IRQ_SWI1_handler(void);
WEAK_ALIAS void IRQ_SWI2_handler(void);
WEAK_ALIAS void IRQ_SWI3_handler(void);
WEAK_ALIAS void IRQ_SWI4_handler(void);
WEAK_ALIAS void IRQ_SWI5_handler(void);
WEAK_ALIAS void IRQ_NVMC_handler(void);
WEAK_ALIAS void IRQ_PPI_handler(void);

// Define the vector table. See the big long discussion above for how this works.
//
// NOTE: the order and names of these _REALLY_ matters. Do NOT mess with this code 
//  unless you know what you are doing and why you are doing it
void *__vectors[] __attribute((section(".vectors"))) = {
    // first 16 slots are for system things 
    __startOfStack__,
    (void *)_StartYakIO,
    (void *)NMI_handler,
    (void *)HARD_FAULT_handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (void *)SVC_handler,
    0,
    0,
    (void *)PENDSVC_handler,
    (void *)SYSTICK_handler,
    // next 32 slots are for peripheral interrupts
    (void *)IRQ_CLOCK_POWER_MPU_handler_handler,
    (void *)IRQ_RADIO_handler,
    (void *)IRQ_UART0_handler,
    (void *)IRQ_SPI0_TWI0_handler,
    (void *)IRQ_SPI1_SPIS1_TWI1_handler,
    0,
    (void *)IRQ_GPIOTE_handler,
    (void *)IRQ_ADC_handler,
    (void *)IRQ_TIMER0_handler,
    (void *)IRQ_TIMER1_handler,
    (void *)IRQ_TIMER2_handler,
    (void *)IRQ_RTC0_handler,
    (void *)IRQ_TEMP_handler,
    (void *)IRQ_RNG_handler,
    (void *)IRQ_ECB_handler,
    (void *)IRQ_AAR_CCM_handler,
    (void *)IRQ_WDT_handler,
    (void *)IRQ_RTC1_handler,
    (void *)IRQ_QDEC_handler,
    (void *)IRQ_LPCOMP_handler,
    (void *)IRQ_SWI0_handler,
    (void *)IRQ_SWI1_handler,
    (void *)IRQ_SWI2_handler,
    (void *)IRQ_SWI3_handler,
    (void *)IRQ_SWI4_handler,
    (void *)IRQ_SWI5_handler,
    0,
    0,
    0,
    0,
    (void *)IRQ_NVMC_handler,
    (void *)IRQ_PPI_handler
};

