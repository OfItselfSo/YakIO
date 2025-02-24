@echo off

REM +------------------------------------------------------------------------------------------------------------------------------+
REM ¦                                                   TERMS OF USE: MIT License                                                  ¦
REM +------------------------------------------------------------------------------------------------------------------------------¦
REM ¦Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation    ¦
REM ¦files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,    ¦
REM ¦modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software¦
REM ¦is furnished to do so, subject to the following conditions:                                                                   ¦
REM ¦                                                                                                                              ¦
REM ¦The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.¦
REM ¦                                                                                                                              ¦
REM ¦THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE          ¦
REM ¦WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR         ¦
REM ¦COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,   ¦
REM ¦ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                         ¦
REM +------------------------------------------------------------------------------------------------------------------------------+

REM This is a simple batch file to create an output .hex file suitable for uploading to the 
REM BBC microbit microcontroller. 

REM Please read the aaReadMe.txt file in this directory. It is much more than simple boiler
REM plate text and will tell you what this example file does and why it does it. The 
REM examples should be reviewed in order - they are designed to form a kind of YakIO library
REM tutorial.

REM Run this script in cmd or Powershell. Set your current directory to the same 
REM location as this file and also place your .h and .cpp code in with it. 
 
REM This script assumes that the necessary YakIO objects can be found at the path 
REM
REM     ..\YakIO\Objects 
REM
REM and the include files in 
REM
REM     ..\YakIO\Include
REM
REM In other words, the folder containing this file is should be in the same folder as the 
REM top of the YakIO library. 

REM Ultimately, what we are doing is compiling all .cpp files in the current directory
REM Then we link against the YakIO library objects (.o files). These must exist. If 
REM they do not, then go and compile those up first. This script will not do that for you.

REM Note that we do not have a Make file here. Installing Make on Windows is tricky and 
REM this script is much simpler. We always recompile all .cpp files here even if they do
REM not need it. The compile process is so fast it really makes very little difference.

REM Once the user .o objects and the YakIO .o objects are linked, we will have an .elf file
REM This needs to be converted to Intel Hex format. Once that is done, a .hex file will be 
REM present in this directory. You can drag and drop that file onto the BBC microbit in  
REM Windows Explorer to flash and run the program

REM The arm-none-eabi-gcc.exe compiler and arm-none-eabi-objcopy.exe converter should be on the path.

REM These are the default locations for the YakIO include files and object files. 
REM Do not put trailing slashes "\" on these directory paths
set YAKIO_TOP_DIR=..\YakIO
set YAKIO_INCLUDE_DIR=..\YakIO\Include
set YAKIO_OBJECT_DIR=..\YakIO\Objects

REM These are the compile and link flags. They have been carefully selected (admittedly, mostly
REM by trial and error) and they all seem to be necessary
set YAKIO_COMPILE_FLAGS= -O -g -mcpu=cortex-m0 -std=c++11 -mthumb -Wall --specs=nosys.specs -fno-exceptions -fno-rtti
set YAKIO_LINK_FLAGS= -mcpu=cortex-m0 -mthumb -O -g -Wall -ffreestanding -fno-builtin -nostdlib

REM make sure our directories exist
@if not exist %YAKIO_TOP_DIR%\ (
  echo "YAKIO_TOP_DIR >>>%YAKIO_TOP_DIR%<<< does not exist"
  exit /b 1
) 
@if not exist %YAKIO_INCLUDE_DIR%\ (
  echo "YAKIO_INCLUDE_DIR >>>%YAKIO_INCLUDE_DIR%<<< does not exist"
  exit /b 1
) 
@if not exist %YAKIO_OBJECT_DIR%\ (
  echo "YAKIO_OBJECT_DIR >>>%YAKIO_OBJECT_DIR%<<< does not exist"
  exit /b 1
) 

REM clean out old object files
del .\*.o
@if %errorlevel% neq 0 exit /b %errorlevel%
REM clean out old elf files
del .\*.elf
@if %errorlevel% neq 0 exit /b %errorlevel%
REM clean out old hex files
del .\*.hex
@if %errorlevel% neq 0 exit /b %errorlevel%

@echo on

@REM compile all local cpp files
arm-none-eabi-gcc -I%YAKIO_INCLUDE_DIR% %YAKIO_COMPILE_FLAGS% -c .\*.cpp
@if %errorlevel% neq 0 exit /b %errorlevel%

@REM link all local .o and YakIO .o object files along with the libgcc library
arm-none-eabi-gcc *.o %YAKIO_OBJECT_DIR%\*.o %YAKIO_TOP_DIR%\libgcc.a %YAKIO_LINK_FLAGS% -T %YAKIO_TOP_DIR%\microbit.ld -o Main.elf  
@if %errorlevel% neq 0 exit /b %errorlevel%

@REM convert to Intel Hex format. The microbit can only load this
arm-none-eabi-objcopy -O ihex Main.elf Main.hex
@if %errorlevel% neq 0 exit /b %errorlevel%

@echo.
@echo The build of the output .hex file was successful
