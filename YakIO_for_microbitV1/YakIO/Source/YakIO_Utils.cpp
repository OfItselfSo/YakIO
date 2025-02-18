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

#include "YakIO_Utils.h"

/* EnableIRQ - enables an IRQ in the NVIC
 * 
 * inputs:
 *         irqNum - the irq number to enable, cannot be <0 or > 31
 * */
void EnableIRQ(int irqNum)
{
    if(irqNum <0) return;
    if(irqNum>31) return;
    // the NVIC ISER is a set register so we can just set these bits directly
    (*(unsigned volatile *) (REGISTER_NVIC+NVICREG_OFFSET_ISER)) = (0x01<<irqNum);            
}

/* DisableIRQ - disables an IRQ in the NVIC
 * 
 * inputs:
 *         irqNum - the irq number to disable, cannot be <0 or > 31
 * */
void DisableIRQ(int irqNum)
{
    if(irqNum <0) return;
    if(irqNum>31) return;
    // the NVIC ICER is a CLR register so we can just set these bits directly to clear them
    (*(unsigned volatile *) (REGISTER_NVIC+NVICREG_OFFSET_ICER)) = (0x01<<irqNum);            
}

/* ClearPendingIRQ - clears a pending IRQ in the NVIC
 * 
 * inputs:
 *         irqNum - the pending irq number to clear, cannot be <0 or > 31
 * */
void ClearPendingIRQ(int irqNum)
{
    if(irqNum <0) return;
    if(irqNum>31) return;
    // the NVIC ICPR is a CLR register so we can just set these bits directly to clear the IRQ
    (*(unsigned volatile *) (REGISTER_NVIC+NVICREG_OFFSET_ICPR)) = (0x01<<irqNum);            
}
