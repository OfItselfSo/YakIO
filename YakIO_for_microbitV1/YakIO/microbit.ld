/*
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
*/
    
/* This file links together all of the .o and .a files output by the compiler and makes them into a .elf file which is kind of a 
 * cross platform common standard file format for executable files. The .elf file will still need to be converted to a Intel Hex
 * format (.hex) file before the BBC microbit can use it.
 *
 * Be aware that there are variables calculated in here which are then used in the code of the YakIO library in order to perform 
 * various startup actions such as initializing variable regions and setting the initial stack pointer. These are merged in at 
 * link time by the linker.
 * 
 * You need this linker file in order to get your code joined up with the YakIO library code, but there should be no need for a user 
 * of the YakIO library to edit this file. Unless, of course, you want to do a bit of experimenting. In that case probably the best 
 * and most readable beginner link script tutorial available can be found at:
 * 
 * https://mcyoung.xyz/2021/06/01/linker-script/
 *
 * Credit: 
 *    There are a number of example linker scripts out there. The bare bones of this one is based on Alex Taradov's open source code
 *    at: https://github.com/ataradov/embedded/blob/master/examples/samd20/samd20j18.ld
 * 
 */

/* define the basic memory structure. This is specific to the nRF51822 chip on the BBC microbit V1 */
MEMORY
{
  flash (rx) : ORIGIN = 0x00000000, LENGTH = 0x40000   /* 256k */
  ram  (rwx) : ORIGIN = 0x20000000, LENGTH = 0x4000    /* 16k */
}

/* now we find all of the various code sections in each .o and .a file and carefully place them in the output 
 * .elf file. The names .text, .data, .bss etc are standard POSIX names for specific types of executable code. From the 
 * mcyoung.xyz link discussed above they are:
 *
 *   .text, where your code lives. It’s usually a loadable, readonly, executable section.
 *   .data contains the initial values of global variables. It’s loadable.
 *   .rodata contains constants. It’s loadable and readonly.
 *   .bss is an empty allocatable section. C specifies that uninitialized globals default to zero; this is a convenient 
 *        way for avoiding storing a huge block of zeros in the executable!
 *
 * Note that the order of these sections and the lines within each section matters here and generally follows the commonly 
 * used and predictable pattern. The KEEP(*(.vectors)) line of the .text section is particularly important. If you wish to 
 * know more about that, read the comments in the YakIO.cpp file of the YakIO library.
 *
 * */
SECTIONS
{
    .text : ALIGN(4)
    {
        __topOfCodeSection__ = .;
        KEEP(*(.vectors))
        *(.text*)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        __bottomOfCodeSection__ = .;
    } > flash

    .data : ALIGN(4) 
    {
        __topOfInitializedDataSection__ = .;
        _data = .;
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        __bottomOfInitializedDataSection__ = .;
    } > ram AT > flash
    

    .bss : ALIGN(4)
    {
        __topOfUnInitializedDataSection__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bottomOfUnInitializedDataSection__ = .;
        /* add a bit of space at the bottom of the data section */
        FILL(0xff)
        /* record the maximum address of the ram */
        . = ALIGN(4);
        __bottomOfRAM__ = .;
    } > ram


    /DISCARD/ : {
        /* Special section to remove code we do not use. If we remove all code
         * from libc.a and libm.a we get rid of the spurious "memset() undefined"
         * style link errors which occur even if we do not use memset. As you 
         * might imagine, it took a while to figure this out.
         * */
        libc.a ( * )
        libm.a ( * )
    }

   /* Now we need to set a variable so our software knows where the start of the 
    * stack is. We have stuffed a bunch of variables into the RAM already in the 
    * .data and .bss sections. We set our stack start right after that. We can 
    * easily calculate our stack start from the start of the RAM and the length
    * of the RAM section. By the time we get here, the linker knows both of these values 
    *
    * Note: This __startOfStack__ variable is very important. It will be injected
    * into the vector table (__vectors[]) over in the YakIO.cpp code. This is the 
    * value placed at address 0x0000000 in the RAM and the CPU absolutely expects to find 
    * it there
    * */
    __startOfStack__ = ORIGIN(ram) + LENGTH(ram); 
    
   /* Note: There are various methods (ASSERTS) discussed online that can check if you 
    * have a minimum stack size available. This is more useful for C code rather than for
    * C++ code which tends to use the stack much more. In general, you will have available
    * for your stack everything remaining in the 16K of RAM that is not consumed by data. If
    * you wish to insert a check to generate a link time error you can do something like
    * the following. It ensures you have space for at least a 4K stack . You do have to know
    * you need 4K and not 8K etc though The value you use is up to you.
    *
    * __StackLimit__ = __startOfStack__ - 4k;
    * ASSERT(__StackLimit__ >= __bottomOfRAM__, "Region RAM overflowed with stack")
    *
    * Credit: https://community.nxp.com/t5/Kinetis-Microcontrollers/How-can-I-increase-the-stack-size-in-my-linker-file/td-p/325487
    */    
    
}

