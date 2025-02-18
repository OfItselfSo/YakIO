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

#ifndef YAKIO_NVIC_H
#define YAKIO_NVIC_H

// the offsets into the NVIC register. This information is not in the 
// the nrf51822 reference guide. You have to go to the cortex-m0+ 
// documentation from ARM for the register mappings. The defines below
// are derived from:
// https://developer.arm.com/documentation/ddi0413/d/nested-vectored-interrupt-controller/nvic-programmer-s-model/nvic-register-map
#define NVICREG_OFFSET_ISER       0x100 // Interrupt Set-Enable Register
#define NVICREG_OFFSET_ICER       0x180 // Interrupt Clear-Enable Register
#define NVICREG_OFFSET_ISPR       0x200 // Interrupt Set-Pending Register
#define NVICREG_OFFSET_ICPR       0x280 // Interrupt Clear-Pending Register
#define NVICREG_OFFSET_PRI0       0x400 // Priority 0 Register
#define NVICREG_OFFSET_PRI1       0x404 // Priority 1 Register
#define NVICREG_OFFSET_PRI2       0x408 // Priority 2 Register
#define NVICREG_OFFSET_PRI3       0x40C // Priority 3 Register
#define NVICREG_OFFSET_PRI4       0x410 // Priority 4 Register
#define NVICREG_OFFSET_PRI5       0x414 // Priority 5 Register
#define NVICREG_OFFSET_PRI6       0x418 // Priority 6 Register
#define NVICREG_OFFSET_PRI7       0x41C // Priority 7 Register

// The IRQ number of a peripheral is derived from its base address. The following discussion is taken
// from the page: https://docs.nordicsemi.com/bundle/ps_nrf5340/page/peripheral_interface.html
//
// Every peripheral is assigned a fixed block of 0x1000 bytes of address space, which is equal to 1024 x 32 bit registers.
// There is a direct relationship between peripheral ID and base address. For example, a peripheral with base address 
// 0x40000000 is assigned ID=0, a peripheral with base address 0x40001000 is assigned ID=1, and a peripheral with base 
// address 0x4001F000 is assigned ID=31.
//
// Peripherals may share the same ID, which may impose one or more of the following limitations:
//
//    Shared registers or common resources
//    Limited availability due to mutually exclusive operation; only one peripheral in use at a time
//    Enforced peripheral behavior when switching between peripherals (disable the first peripheral before enabling the second)

#define IRQ_CLOCK   0x00 // CLOCK Clock control
#define IRQ_POWER   0x00 // POWER Power Control
#define IRQ_MPU     0x00 // MPU Memory Protection Unit
#define IRQ_RADIO   0x01 // RADIO 2.4 GHz radio
#define IRQ_UART0   0x02 // UART Universal Asynchronous Receiver/Transmitter
#define IRQ_SPI0    0x03 // SPI SPI master 0
#define IRQ_TWI0    0x03 // TWI Two-wire interface master 0
#define IRQ_SPI1    0x04 // SPI SPI master 1
#define IRQ_SPIS1   0x04 // SPIS SPI slave 1
#define IRQ_TWI1    0x04 // TWI Two-wire interface master 1
#define IRQ_GPIOTE  0x06 // GPIOTE GPIO tasks and events
#define IRQ_ADC     0x07 // ADC Analog to digital converter
#define IRQ_TIMER0  0x08 // TIMER Timer 0
#define IRQ_TIMER1  0x09 // TIMER Timer 1
#define IRQ_TIMER2  0x0A // TIMER Timer 2
#define IRQ_RTC0    0x0B // RTC Real time counter 0
#define IRQ_TEMP    0x0C // TEMP Temperature Sensor
#define IRQ_RNG     0x0D // RNG Random Number Generator
#define IRQ_ECB     0x0E // ECB AES ECB Mode Encryption
#define IRQ_AAR     0x0F // AAR Accelerated Address Resolver
#define IRQ_CCM     0x0F // CCM AES CCM Mode Encryption
#define IRQ_WDT     0x10 // WDT Watchdog Timer
#define IRQ_RTC1    0x11 // RTC Real time counter 1
#define IRQ_QDEC    0x12 // QDEC Quadrature decoder
#define IRQ_LPCOMP  0x13 // LPCOMP Low power comparator
#define IRQ_SWI0    0x14 // SWI Software interrupt 0
#define IRQ_SWI1    0x15 // SWI Software interrupt 1
#define IRQ_SWI2    0x16 // SWI Software interrupt 2
#define IRQ_SWI3    0x17 // SWI Software interrupt 3
#define IRQ_SWI4    0x18 // SWI Software interrupt 4
#define IRQ_SWI5    0x19 // SWI Software interrupt 5
#define IRQ_NVMC    0x1E // NVMC Non Volatile Memory Controller
#define IRQ_PPI     0x1F // PPI PPI controller

#endif
