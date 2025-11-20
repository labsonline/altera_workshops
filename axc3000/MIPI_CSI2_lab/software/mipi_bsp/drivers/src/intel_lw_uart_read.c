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

#include "sys/alt_irq.h"
#include "sys/ioctl.h"
#include "sys/alt_errno.h"

#include "intel_lw_uart.h"
#include "intel_lw_uart_regs.h"

#ifdef ALT_SINGLE_THREADED
    #if INTEL_LW_UART_RX_TIMEOUT_CYCLE > 0
#include "priv/alt_busy_sleep.h"
    #endif
#endif

#if defined(ALT_USE_SMALL_DRIVERS) || defined(INTEL_LW_UART_SMALL)

/* ----------------------------------------------------------- */
/* ----------------------- SMALL DRIVER ---------------------- */
/* ----------------------------------------------------------- */

/* intel_lw_uart_read
 *
 * Private function: intel_lw_uart_read() is called by the system read() function in 
 * order to read a block of data from the UART.
 *
 * Permission checks are made before the call to intel_lw_uart_read(), so
 * we know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * This implementation polls the device waiting for characters. At most it can
 * only return one character, regardless of how many are requested. If the 
 * device is being accessed in non-blocking mode then it is possible for this
 * function to return without reading any characters. In this case errno is
 * set to EWOULDBLOCK.
 *
 *** Arguments:
 * - *sp        : pointer to the LW UART device state.
 * - *ptr       : destination address.
 * - len        : maximum length of the data to read.
 * - flags      : for indicating blocking/non-blocking access for single/multi-threaded.
 *
 *** Returns:
 *   Number of bytes read    -> success.
 *   -EWOULDBLOCK            -> the read operation would have blocked, 
 *                              but the file descriptor was placed in non-blocking mode.
 */

int 
intel_lw_uart_read(intel_lw_uart_state* sp, char* ptr, int len,
  int flags)
{
    int block;
    unsigned int status;

    block = !(flags & O_NONBLOCK);

    do
    {
        status = IORD_INTEL_LW_UART_STATUS(sp->base);

        /* clear any error flags */

        IOWR_INTEL_LW_UART_STATUS(sp->base, 0);

        if (status & INTEL_LW_UART_CONTROL_RRDY_MSK)
        {
            ptr[0] = IORD_INTEL_LW_UART_RXDATA(sp->base);
        
            if (!(status & (INTEL_LW_UART_STATUS_PE_MSK | 
            INTEL_LW_UART_STATUS_FE_MSK)))
            {
                return 1;
            }
        }
    }
    while (block);

    ALT_ERRNO = EWOULDBLOCK;

    return 0;
}

#else

/* ----------------------------------------------------------- */
/* ----------------------- FAST DRIVER ----------------------- */
/* ----------------------------------------------------------- */

/* intel_lw_uart_read_enable_interrupt
 *
 * Private function: intel_lw_uart_read_enable_interrupt() is called by 
 * the intel_lw_uart_read() function in order to enable necessary interrupt
 * for read operation.
 *
 *** Arguments:
 * - *sp        : pointer to the LW UART device state.
 *
 *** Returns: 
 *   None
 */

void 
intel_lw_uart_read_enable_interrupt(intel_lw_uart_state* sp)
{
    alt_irq_context context;

    context = alt_irq_disable_all ();
    sp->ctrl |= INTEL_LW_UART_CONTROL_RRDY_MSK;
    IOWR_INTEL_LW_UART_CONTROL(sp->base, sp->ctrl);
    alt_irq_enable_all (context);
}

/* intel_lw_uart_read
 *
 * Private function: intel_lw_uart_read() is called by the system read() function in 
 * order to read a block of data from the UART.
 *
 * Permission checks are made before the call to intel_lw_uart_read(), so
 * we know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * This function does not communicate with the device directly. Instead data is
 * transfered from a circular buffer. The interrupt handler is then responsible
 * for copying data from the device into this buffer.
 *
 *** Arguments:
 * - *sp        : pointer to the LW UART device state.
 * - *ptr       : destination address.
 * - len        : maximum length of the data to read.
 * - flags      : for indicating blocking/non-blocking access for single/multi-threaded.
 *
 *** Returns:
 *   Number of bytes read    -> success.
 *   -EWOULDBLOCK            -> the read operation would have blocked, 
 *                              but the file descriptor was placed in non-blocking mode.
 *   -ETIME                  -> read timeout.
 */

int 
intel_lw_uart_read(intel_lw_uart_state* sp, char* ptr, int len,
  int flags)
{
    int             no_block;
    alt_u8          read_would_block = 0;
    int             count = 0;
#ifdef ALT_SINGLE_THREADED
    #if INTEL_LW_UART_RX_TIMEOUT_CYCLE > 0
    int             read_timeout_cnt = 0;
    alt_u8          read_would_timeout = 0;
    #endif
#endif

    /* only read if the requested len is >0! */
    if(0 < len) 
    {
        /* 
        * Construct a flag to indicate whether the device is being accessed in
        * blocking or non-blocking mode.
        */

        no_block = (flags & O_NONBLOCK);

        /*
         * When running in a multi threaded environment, obtain the "read_lock"
         * semaphore. This ensures that reading from the device is thread-safe.
         */

        ALT_SEM_PEND (sp->read_lock, 0);

        /*
         * Loop, copying data from the circular buffer to the destination address
         * supplied in "ptr". This loop is terminated when the required number of
         * bytes have been read. If the circular buffer is empty, and no data has
         * been read, then the loop will block (when in blocking mode).
         *
         * If the circular buffer is empty, and some data has already been 
         * transferred, or the device is being accessed in non-blocking mode, then
         * the loop terminates without necessarily reading all the requested data.
         */
        do
        {
            /*
             * Read the required amount of data, until the circular buffer runs
             * empty
             */

            while ((count < len) && (sp->rx_start != sp->rx_end))
            {
                count++;
                *ptr++ = sp->rx_buf[sp->rx_start];

                sp->rx_start = (sp->rx_start+1) & sp->rx_buf_mask;
            }

            /*
             * If no data has been transferred, the circular buffer is empty, and
             * this is not a non-blocking access, block waiting for data to arrive.
             */

            if (!count && (sp->rx_start == sp->rx_end))
            {
                if (no_block)
                {
                    /* Set errno to indicate the reason we're not returning any data */

                    ALT_ERRNO = EWOULDBLOCK;
                    read_would_block = 1;
                    break;
                }
                else
                {
                    /* Block waiting for some data to arrive */

                    /* First, ensure read interrupts are enabled to avoid deadlock */
                    intel_lw_uart_read_enable_interrupt(sp);

                    /*
                     * When running in a multi-threaded mode, we pend on the read event 
                     * flag set in the interrupt service routine. This avoids wasting CPU
                     * cycles waiting in this thread, when we could be doing something more 
                     * profitable elsewhere.
                     */

                    ALT_FLAG_PEND (sp->events, 
                                INTEL_LW_UART_READ_RDY,
                                ALT_FLAG_WAIT_SET_ANY_WITH_CONSUME,
                                ALT_FLAG_WAIT_MAX_TIMEOUT);

#ifdef ALT_SINGLE_THREADED
    #if INTEL_LW_UART_RX_TIMEOUT_CYCLE > 0
                    /* break the dead lock if no data is received after n cycles.*/
                    if( INTEL_LW_UART_RX_TIMEOUT_CYCLE < read_timeout_cnt )
                    {
                        ALT_ERRNO = ETIME;
                        read_would_timeout = 1;
                        break;
                    }
                    else 
                    {
                        alt_busy_sleep(INTEL_LW_UART_RX_TIMEOUT_US);
                        read_timeout_cnt++;
                    }
    #endif
#endif
                }
            }
        }
        while (!count && len);
    }
    /*
     * Now that access to the circular buffer is complete, release the read
     * semaphore so that other threads can access the buffer.
     */

    ALT_SEM_POST (sp->read_lock);

    /*
     * Ensure that interrupts are enabled, so that the circular buffer can
     * re-fill.
     */

    intel_lw_uart_read_enable_interrupt(sp);

    /* Return the number of bytes read */
    if(read_would_block) 
    {
        return -EWOULDBLOCK;
    }
#ifdef ALT_SINGLE_THREADED
    #if INTEL_LW_UART_RX_TIMEOUT_CYCLE > 0
    else if (read_would_timeout)
    {
        return -ETIME;
    }
    #endif
#endif  
    else
    {
        return count;
    }
}

#endif /* fast driver */
