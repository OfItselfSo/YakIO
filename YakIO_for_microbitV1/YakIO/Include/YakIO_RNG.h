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

#ifndef YAKIO_RNG_H
#define YAKIO_RNG_H
#include "YakIO.h"
#include "YakIO_CALLBACK.h"
#include "YakIO_NVIC.h"
#include "YakIO_Utils.h"

// RNG REGISTER SPECIFIC SECTION
#define RNGREG_OFFSET_START 0x000           // Task starting the random number generator
#define RNGREG_OFFSET_STOP 0x004            // Task stopping the random number generator
#define RNGREG_OFFSET_VALRDY 0x100          // Event being generated for every new random number written to the VALUE register
#define RNGREG_OFFSET_SHORTS 0x200          // Shortcut register
#define RNGREG_OFFSET_ITEN 0x300            // Enable or disable interrupt
#define RNGREG_OFFSET_ITENSET 0x304         // Enable interrupt
#define RNGREG_OFFSET_ITENCLR 0x308         // Disable interrupt
#define RNGREG_OFFSET_CONFIG 0x504          // Configuration register
#define RNGREG_OFFSET_VALUE 0x508           // Output random number

// note the value here is carefully set to the value we have to
// stuff in the register to set the bit mode properly
enum RNG_DERCEN {
    RNG_DERCEN_DIS=0,     // Digital error correction disabled
    RNG_DERCEN_ENA=1,     // Digital error correction enabled
};

#define RNG_SHORT_VALRDY_STOP_BIT 0x01      // bit we set/clear
#define RNG_INTEN_BIT 0x01                  // bit we set/clear
#define RNG_INTEN_CLR_BIT 0x01                  // bit we set/clear

/* YakIO_RNG - a class to represent and encapsulate the random number
 *     peripheral
 * */
class YakIO_RNG
{
  private:
      unsigned int isInitialized =0;
      YakIO_CALLBACK *callbackInterfacePtr =0;
      enum CALLBACK_ID callbackID = CALLBACK_NONE;
      void SetINTEN(void);
      void ClearINTEN(void);
      void ResetAllShorts(void);
      void ClearValueReady(void);

  public:
      // Constructor to initialize YakIO_RNG object
      YakIO_RNG();
      void SetCallback(enum CALLBACK_ID callbackIDIn, YakIO_CALLBACK *callbackInterfacePtrIn);
      void CallCallback();
      void ClearAllCallbacks(void);
      void EnableRngIRQ(void);
      void DisableRngIRQ(void);
      void RngStart(void);
      void RngStop(void);
      void RngShutdown(void);
      void SetDerCen(enum RNG_DERCEN derCenValue);
      unsigned int GetRngValue(void);
      enum RNG_DERCEN GetDerCen(void);

};


#endif

