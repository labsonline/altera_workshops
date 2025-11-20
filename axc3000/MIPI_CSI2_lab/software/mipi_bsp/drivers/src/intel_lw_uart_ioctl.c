/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2022 Intel Corporation, San Jose, California, USA.            *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
*                                                                             *
******************************************************************************/

#include <fcntl.h>
#include <string.h>

#include "sys/alt_irq.h"
#include "sys/ioctl.h"
#include "sys/alt_errno.h"

#include "intel_lw_uart_regs.h"
#include "intel_lw_uart.h"


#if !defined(ALT_USE_SMALL_DRIVERS) && !defined(INTEL_LW_UART_SMALL)

/* ----------------------------------------------------------- */
/* ------------------------- FAST DRIVER --------------------- */
/* ----------------------------------------------------------- */

/*
 * To reduce the code footprint of this driver, the ioctl() function is not
 * included by default. If you wish to use the ioctl features provided 
 * below, you can do so by adding the option : -DINTEL_LW_UART_USE_IOCTL
 * to CPPFLAGS in the Makefile (or through the Eclipse IDE).
 */

#ifdef INTEL_LW_UART_USE_IOCTL

static int intel_lw_uart_tiocmget(intel_lw_uart_state* sp,
  struct termios* term);
static int intel_lw_uart_tiocmset(intel_lw_uart_state* sp,
  struct termios* term);

/* intel_lw_uart_ioctl
 *
 * Private function: intel_lw_uart_ioctl() is called by the system ioctl() 
 * function to handle ioctl requests for the UART. 
 * The only ioctl requests supported are TIOCMGET and TIOCMSET.
 *
 * TIOCMGET returns a termios structure that describes the current device
 * configuration.
 *
 * TIOCMSET sets the device (if possible) to match the requested configuration.
 * The requested configuration is described using a termios structure passed
 * through the input argument "arg".
 *
 *** Arguments:
 * - *sp     : pointer to the LW UART device state.
 * - reg     : IOCTL request: TIOCMGET, TIOCMSET, TIOCEXCL, TIOCNXCL.
 * - *arg    : input termios.
 *
 *** Returns: 
 *  0        -> success.
 * -ENOTTY   -> unsupported request.
 * -EIO      -> unsupported termios setting.
 * -1        -> input termios(arg) is null pointer.
 */

int 
intel_lw_uart_ioctl(intel_lw_uart_state* sp, int req, void* arg)
{
    int rc = -ENOTTY;

    switch (req)
    {
    case TIOCMGET:
        rc = intel_lw_uart_tiocmget(sp, (struct termios*) arg);
        break;
    case TIOCMSET:
        rc = intel_lw_uart_tiocmset(sp, (struct termios*) arg);
        break;
    default:
        break;
    }
    return rc;
}

/* intel_lw_uart_tiocmget
 *
 * Private function: intel_lw_uart_tiocmget() is used by intel_lw_uart_ioctl() to fill
 * in the input termios structure with the current device configuration. 
 *
 * See termios.h for further details on the contents of the termios structure.
 *
 *** Arguments:
 * - *sp        : pointer to the LW UART device state.
 * - *term      : output termios.
 *** Returns: 
 *   0          -> success.
 *  -1          -> output term is null pointer.
 */

static int 
intel_lw_uart_tiocmget(intel_lw_uart_state* sp,
  struct termios* term)
{
    if(NULL == term)
    {
        return -1;
    }

    memcpy (term, &sp->termios, sizeof (struct termios));
    return 0;
}

/* intel_lw_uart_tiocmset
 *
 * Private function: intel_lw_uart_tiocmset() is used by intel_lw_uart_ioctl() to
 * intel_lw_uart_tiocmset() is used by intel_lw_uart_ioctl() to 
 * configure the device according to the settings in the input termios 
 * structure. In practice the only configuration that can be changed is the
 * baud rate, and then only if the hardware is configured to have a writable
 * baud register.
 *
 *** Arguments:
 * *sp        : pointer to the LW UART device state.
 * *term      : input termios.
 *** Returns: 
 *   0        -> success.
 * - 1        -> input term is null pointer.
 * - EIO      -> unsupported termios setting.
 */

static int 
intel_lw_uart_tiocmset(intel_lw_uart_state* sp,
  struct termios* term)
{
    speed_t speed;
    if(NULL == term) 
    {
        return -1;
    }

    speed = sp->termios.c_ispeed;

    /* Update the settings if the hardware supports it */

    if (!(sp->flags & INTEL_LW_UART_FB))
    {
        sp->termios.c_ispeed = sp->termios.c_ospeed = term->c_ispeed;
    }
    /* 
     * If the request was for an unsupported setting, return an error.
     */

    if (memcmp(term, &sp->termios, sizeof (struct termios)))
    {
        sp->termios.c_ispeed = sp->termios.c_ospeed = speed;
        return -EIO;
    }

    /* 
     * If the request speed is same as the current speed, return 0.
     * Before this, code will also check for invalid parameter.
     */
    if(term->c_ispeed == speed)
    {
        return 0;
    }

    /* 
     * If the request is more than clock frequency/4, return an error. 
     * Higher frequency will cause framing error.
     * User should consider cable length, when increasing baudrate!
     */
    if( (sp->freq>>2) < term->c_ispeed)
    {
        sp->termios.c_ispeed = sp->termios.c_ospeed = speed;
        return -EIO;
    }

    /*
     * Otherwise, update the hardware.
     */
    
    IOWR_INTEL_LW_UART_DIVISOR(sp->base, 
        ((sp->freq/sp->termios.c_ispeed) - 1));

    return 0;
}

#endif /* INTEL_LW_UART_USE_IOCTL */

#endif /* fast driver */
