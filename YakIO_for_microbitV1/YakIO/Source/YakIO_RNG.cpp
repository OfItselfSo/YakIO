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

#include "YakIO.h"
#include "YakIO_RNG.h"

// the IRQ_RNG_handler is a non-member function. It has no idea of
// what class it should work on. The pointer below is set in the
// constructor of the RNG Object.
//
// This allows the RNG IRQ function to find the proper rng object.
//
// This value is exclusively local in scope and should not be messed with by
// external operations, or by anything really.
//
// NOTE: also see the discussion of the IRQ_RNG_handler in the comments on
// that function for additional information

YakIO_RNG *rng_ptr = NULL;


    /* constructor
     *
     * */
    YakIO_RNG::YakIO_RNG()
    {
        // set this so we know we have run through the constructor. Creating
        // objects on the heap will NOT run the constructor
        isInitialized =1;

        // remember our 'this' pointer
        rng_ptr = this;
    }

    /* ResetAllShorts - reset all relevant bits in the SHORTS register
     *
     * */
    void YakIO_RNG::ResetAllShorts(void)
    {
        // the bits we need to clear are the bitwise NOT of this
        unsigned int bitVal = RNG_SHORT_VALRDY_STOP_BIT; // we only have one meaningful bit

        // set the value in the appropriate register
        (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_SHORTS)) = (~bitVal);
    }

    /* SetINTEN - sets the RNG_INTEN_BIT in the INTENSET register
     *    So as to enable an interrupt every time our random number is ready
     *
     * */
    void YakIO_RNG::SetINTEN(void)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // set the value in the appropriate register
        (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_ITENSET)) |= RNG_INTEN_BIT;
    }

    /* ClearINTEN - Clears the RNG_INTEN_BIT in the INTENCLR register
     *    So as to disable an interrupt called every time our random number is ready
     *
     * */
    void YakIO_RNG::ClearINTEN(void)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // we use a CLR register so we can just set these bits directly
        (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_ITENCLR)) |= RNG_INTEN_BIT;
    }

    /* SetCallback - sets the callback object and function within that object.
     *     The callback object must inherit from YakIO_CALLBACK
     *
     *   NOTE: Does not start the RNG object. This is a separate call
     *
     * inputs:
     *    callbackInterfacePtrIn - the "this" pointer of the object to receive
     *       the callback
     *    callbackIDIn - the callback id to use. Essentially this identifies the function name within the
     *       callback interface object
     *
     * */
    void YakIO_RNG::SetCallback(enum CALLBACK_ID callbackIDIn, YakIO_CALLBACK *callbackInterfacePtrIn)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // set the callback interface pointer, this is the "this" pointer
        // of the objects that wants to receive the callback
        callbackInterfacePtr = callbackInterfacePtrIn;
        // set the callbackID
        callbackID = callbackIDIn;
        // enable an interrupt every time our count matches the counter
        SetINTEN();
        // we can trigger interrupts all we want but they will not call anything unless
        // we also enable the RNGs interrupt in the NVIC
        EnableRngIRQ();
    }

    /* CallCallback - calls the callback function set on this object
     *
     * */
    void YakIO_RNG::CallCallback()
    {
        if(isInitialized==0) return;
        // we have to have this
        if(callbackInterfacePtr==NULL) return;

        // figure out what callback function to call and call it
        if(callbackID == CALLBACK_0) callbackInterfacePtr->Callback0();
        else if(callbackID == CALLBACK_1) callbackInterfacePtr->Callback1();
        else if(callbackID == CALLBACK_2) callbackInterfacePtr->Callback1();
        else if(callbackID == CALLBACK_3) callbackInterfacePtr->Callback1();
        // we never permit callbacks to the heartbeat in here. That is a special
        // case TIMER thing
        //else if(callbackID == HEARTBEAT) callbackInterfacePtr->Heartbeat();
    }

    /* ClearAllCallbacks - clear all callbacks
     *
     * */
    void YakIO_RNG::ClearAllCallbacks(void)
    {
        // run through our list of callbacks
        // we only have one at the moment so this will do it
        callbackInterfacePtr=NULL;
        callbackID=CALLBACK_NONE;
    }

    /* EnableRngIRQ - enable the RNGs IRQ in the NVIC
     *
     * */
    void YakIO_RNG::EnableRngIRQ(void)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // enable the proper interrupt
        EnableIRQ(IRQ_RNG);
    }

    /* DisableRngIRQ - disable the RNGs IRQ in the NVIC
     *
     * */
    void YakIO_RNG::DisableRngIRQ(void)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // disable the proper interrupt
        DisableIRQ(IRQ_RNG);
    }

    /* RngStart - starts the random number generator peripheral.
     *
     * Note: The RNG peripheral should be fully configured before starting. You
     *    should not change the parameters while is is running. Bad
     *    things will happen
     *
     * */
    void YakIO_RNG::RngStart(void)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // clear this now
        ClearValueReady();

        // set the value in the appropriate register
        (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_START)) = 1;
    }

    /* RngStop - stops the random number generator peripheral.
     *
     * Note: The RNG peripheral should be stopped before configuring. You
     *    should not change the parameters while is is running. Bad
     *    things will happen
     *
     * */
    void YakIO_RNG::RngStop(void)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // set the value in the appropriate register
        (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_STOP)) = 1;
    }

    /* RngShutdown - shuts down the random nunber generator peripheral]
     *     and clears all interrupts and callbacks.
     * */
    void YakIO_RNG::RngShutdown(void)
    {
        // make sure the rng is stopped
        RngStop();
        // clear all callbacks
        ClearAllCallbacks();
        // disable the IRQ
        DisableRngIRQ();
    }

    /* SetDerCen - sets the digital error correction value. This
     *    removes any bias toward '1' or '0'.
     *
     * inputs:
     *         derCenValue - a RNG_DERCEN value indicating the mode
     * */
    void YakIO_RNG::SetDerCen(enum RNG_DERCEN derCenValue)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // set the value in the appropriate register
        (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_CONFIG)) = derCenValue;
    }

    /* GetDerCen - sets the digital error correction value
     *
     * returns
     *        returns a RNG_DERCEN value indicating the DirCen
     * */
    enum RNG_DERCEN YakIO_RNG::GetDerCen(void)
    {
        // we must be initialized
        if(isInitialized==0) return RNG_DERCEN_DIS;

        // get the value from the appropriate register
        unsigned int derCenValue = (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_CONFIG));
        if(derCenValue==RNG_DERCEN_ENA) return RNG_DERCEN_ENA;
        else return RNG_DERCEN_DIS;
    }

    /* GetRngValue - gets the random number value. This is a true random generated from
     *               thermal noise.
     *
     *      Note: This spins until VALRDY is nonzero. If you do not do this and if you
     *            call it fast enough you would get many, many, duplicate values in
     *            a row as the same value is read over and over again. This code also
     *            clears VALRDY. This means you can sit in here for an indeterminate
     *            amount of time until a value is ready.
     *
     *      Note: The random number is 8 bit. So only values between 0 and 255
     *           are returned.
     *
     * returns
     *        returns the current random number value
     * */
    unsigned int YakIO_RNG::GetRngValue(void)
    {
        // we must be initialized
        if(isInitialized==0) return 0;

        // spin until a value is ready
        while ( (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_VALRDY)) == 0) {}

        // get the value from the appropriate register
        unsigned int rngValue = (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_VALUE));

        // clear this now or we will get duplicates if the user calls it fast enough
        (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_VALRDY)) = 0x00;

        return rngValue;
    }

    /* ClearValueReady - clears the value ready flag so that we
     *    can generate another
     *
     * */
    void YakIO_RNG::ClearValueReady(void)
    {
        // we must be initialized
        if(isInitialized==0) return;

        // set the value in the appropriate register
        (*(unsigned volatile *) (REGISTER_RNG+RNGREG_OFFSET_VALRDY)) = 0x00;
     }

    /* IRQ_RNG_handler
     *
     * Note: the address of this function is set in the flash by the linker.
     *       If this function exists then this functions address will be used,
     *       if it does not exist then the interrupt will be directed to a
     *       default handler which spins forever. The flash address is not
     *       runtime settable.
     *
     *       The name really matters here. See the discussion in YakIO.cpp
     *
     *   Do NOT define this anywhere else. This class needs it here.
     * */
    void IRQ_RNG_handler(void)
    {
        if(rng_ptr==NULL) return;
        // we have a pointer, call the callback
        rng_ptr->CallCallback();
    }
