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

#ifndef YAKIO_H
#define YAKIO_H

#define YAKIO_MAJOR_VERSION 0
#define YAKIO_MINOR_VERSION 90

#define NULL 0
#define BYTES_IN_REGISTER 4         // we are a 32 bit system

// register defines, straight out of page 17 in the
// nrf51822 reference guide
#define REGISTER_CLOCK   0x40000000 // CLOCK Clock control
#define REGISTER_POWER   0x40000000 // POWER Power Control
#define REGISTER_MPU     0x40000000 // MPU Memory Protection Unit
#define REGISTER_RADIO   0x40001000 // RADIO 2.4 GHz radio
#define REGISTER_UART0   0x40002000 // UART Universal Asynchronous Receiver/Transmitter
#define REGISTER_SPI0    0x40003000 // SPI SPI master 0
#define REGISTER_TWI0    0x40003000 // TWI Two-wire interface master 0
#define REGISTER_SPI1    0x40004000 // SPI SPI master 1
#define REGISTER_SPIS1   0x40004000 // SPIS SPI slave 1
#define REGISTER_TWI1    0x40004000 // TWI Two-wire interface master 1
#define REGISTER_GPIOTE  0x40006000 // GPIOTE GPIO tasks and events
#define REGISTER_ADC     0x40007000 // ADC Analog to digital converter
#define REGISTER_TIMER0  0x40008000 // TIMER Timer 0
#define REGISTER_TIMER1  0x40009000 // TIMER Timer 1
#define REGISTER_TIMER2  0x4000A000 // TIMER Timer 2
#define REGISTER_RTC0    0x4000B000 // RTC Real time counter 0
#define REGISTER_TEMP    0x4000C000 // TEMP Temperature Sensor
#define REGISTER_RNG     0x4000D000 // RNG Random Number Generator
#define REGISTER_ECB     0x4000E000 // ECB AES ECB Mode Encryption
#define REGISTER_AAR     0x4000F000 // AAR Accelerated Address Resolver
#define REGISTER_CCM     0x4000F000 // CCM AES CCM Mode Encryption
#define REGISTER_WDT     0x40010000 // WDT Watchdog Timer
#define REGISTER_RTC1    0x40011000 // RTC Real time counter 1
#define REGISTER_QDEC    0x40012000 // QDEC Quadrature decoder
#define REGISTER_LPCOMP  0x40013000 // LPCOMP Low power comparator
#define REGISTER_SWI0    0x40014000 // SWI Software interrupt 0
#define REGISTER_SWI1    0x40015000 // SWI Software interrupt 1
#define REGISTER_SWI2    0x40016000 // SWI Software interrupt 2
#define REGISTER_SWI3    0x40017000 // SWI Software interrupt 3
#define REGISTER_SWI4    0x40018000 // SWI Software interrupt 4
#define REGISTER_SWI5    0x40019000 // SWI Software interrupt 5
#define REGISTER_NVMC    0x4001E000 // NVMC Non Volatile Memory Controller
#define REGISTER_PPI     0x4001F000 // PPI PPI controller
#define REGISTER_FICR    0x10000000 // FICR Factory Information Configuration
#define REGISTER_UICR    0x10001000 // UICR User Information Configuration
#define REGISTER_RTC2    0x40024000 // RTC Real time counter 2.
#define REGISTER_GPIO    0x50000000 // GPIO General purpose input and output
// the NVIC is not discussed much in the nrf51822 reference guide. You have to go
// to the cortex-m0+ documentation from ARM for the register mappings
#define REGISTER_NVIC    0xE000E000 // NVIC Nested Vectored Interrupt Controller base address




#endif
