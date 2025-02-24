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

REM Simple batch file. We just compile each YakIO library object in turn.
REM Run this script in cmd or Powershell. Set your current directory to the same
REM location as this file for this to work. Ultimately we are just producing
REM a collection of .o object files for user code to link against. We do not
REM produce a library .a file because this messes up the weak aliases
REM defined in YakIO.cpp and the code will not execute properly.

REM The arm-none-eabi-gcc.exe compiler should be on the path.

REM These are the default directories.   The output (by default) goes in .\Objects
REM do not put trailing slashes "\" on these directories
set YAKIO_INCLUDE_DIR=.\Include
set YAKIO_SOURCE_DIR=.\Source
set YAKIO_OBJECT_DIR=.\Objects
set YAKIO_COMPILE_FLAGS= -O -g -mcpu=cortex-m0 -std=c++11 -mthumb -Wall --specs=nosys.specs -fno-exceptions -fno-rtti

REM make sure our directories exist
@if not exist %YAKIO_INCLUDE_DIR%\ (
  echo "YAKIO_INCLUDE_DIR >>>%YAKIO_INCLUDE_DIR%<<< does not exist"
  exit /b 1
)
@if not exist %YAKIO_SOURCE_DIR%\ (
  echo "YAKIO_SOURCE_DIR >>>%YAKIO_SOURCE_DIR%<<< does not exist"
  exit /b 1
)
@if not exist %YAKIO_OBJECT_DIR%\ (
  echo "YAKIO_OBJECT_DIR >>>%YAKIO_OBJECT_DIR%<<< does not exist"
  exit /b 1
)

REM clean out old object files
del %YAKIO_OBJECT_DIR%\*.o
@if %errorlevel% neq 0 exit /b %errorlevel%

@echo on

arm-none-eabi-gcc -I%YAKIO_INCLUDE_DIR% %YAKIO_COMPILE_FLAGS%  -c %YAKIO_SOURCE_DIR%\YakIO.cpp -o %YAKIO_OBJECT_DIR%\YakIO.o
@if %errorlevel% neq 0 exit /b %errorlevel%
arm-none-eabi-gcc -I%YAKIO_INCLUDE_DIR% %YAKIO_COMPILE_FLAGS%  -c %YAKIO_SOURCE_DIR%\YakIO_GPIO.cpp -o %YAKIO_OBJECT_DIR%\YakIO_GPIO.o
@if %errorlevel% neq 0 exit /b %errorlevel%
arm-none-eabi-gcc -I%YAKIO_INCLUDE_DIR% %YAKIO_COMPILE_FLAGS%  -c %YAKIO_SOURCE_DIR%\YakIO_LEDARRAY.cpp -o %YAKIO_OBJECT_DIR%\YakIO_LEDARRAY.o
@if %errorlevel% neq 0 exit /b %errorlevel%
arm-none-eabi-gcc -I%YAKIO_INCLUDE_DIR% %YAKIO_COMPILE_FLAGS%  -c %YAKIO_SOURCE_DIR%\YakIO_TIMER.cpp -o %YAKIO_OBJECT_DIR%\YakIO_TIMER.o
@if %errorlevel% neq 0 exit /b %errorlevel%
arm-none-eabi-gcc -I%YAKIO_INCLUDE_DIR% %YAKIO_COMPILE_FLAGS%  -c %YAKIO_SOURCE_DIR%\YakIO_RNG.cpp -o %YAKIO_OBJECT_DIR%\YakIO_RNG.o
@if %errorlevel% neq 0 exit /b %errorlevel%
arm-none-eabi-gcc -I%YAKIO_INCLUDE_DIR% %YAKIO_COMPILE_FLAGS%  -c %YAKIO_SOURCE_DIR%\YakIO_Utils.cpp -o %YAKIO_OBJECT_DIR%\YakIO_Utils.o
@if %errorlevel% neq 0 exit /b %errorlevel%

@echo.
@echo The build of the YakIO object files was successful
