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

#ifndef YAKIO_CLOCK_H
#define YAKIO_CLOCK_H

// CLOCK REGISTER SPECIFIC SECTION
#define CLOCKREG_OFFSET_HFCLKSTART      0x000 // Start HFCLK crystal oscillator
#define CLOCKREG_OFFSET_HFCLKSTOP       0x004 // Stop HFCLK crystal oscillator
#define CLOCKREG_OFFSET_LFCLKSTART      0x008 // Start LFCLK source
#define CLOCKREG_OFFSET_LFCLKSTOP       0x00C // Stop LFCLK source
#define CLOCKREG_OFFSET_CAL             0x010 // Start calibration of LFCLK RC oscillator
#define CLOCKREG_OFFSET_CTSTART         0x014 // Start calibration timer
#define CLOCKREG_OFFSET_CTSTOP          0x018 // Stop calibration timer
// Events
#define CLOCKREG_OFFSET_HFCLKSTARTED    0x100 // HFCLK oscillator started
#define CLOCKREG_OFFSET_LFCLKSTARTED    0x104 // LFCLK started
#define CLOCKREG_OFFSET_DONE            0x10C // Calibration of LFCLK RC oscillator complete event
#define CLOCKREG_OFFSET_CTTO            0x110 // Calibration timer timeout
// Registers
#define CLOCKREG_OFFSET_INTENSET        0x304 // Enable interrupt
#define CLOCKREG_OFFSET_INTENCLR        0x308 // Disable interrupt
#define CLOCKREG_OFFSET_HFCLKRUN        0x408 // Status indicating that HFCLKSTART task has been triggered
#define CLOCKREG_OFFSET_HFCLKSTAT       0x40C // Which HFCLK source is running
#define CLOCKREG_OFFSET_LFCLKRUN        0x414 // Status indicating that LFCLKSTART task has been triggered
#define CLOCKREG_OFFSET_LFCLKSTAT       0x418 // Which LFCLK source is running
#define CLOCKREG_OFFSET_LFCLKSRCCOPY    0x41C // Copy of LFCLKSRC register, set when LFCLKSTART task was triggered
#define CLOCKREG_OFFSET_LFCLKSRC        0x518 // Clock source for the LFCLK
#define CLOCKREG_OFFSET_CTIV            0x538 // Calibration timer interval
#define CLOCKREG_OFFSET_XTALFREQ        0x550 // Crystal frequency

#endif
