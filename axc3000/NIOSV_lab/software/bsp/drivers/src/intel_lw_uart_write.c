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

#include "sys/alt_dev.h"
#include "sys/alt_irq.h"
#include "sys/ioctl.h"
#include "sys/alt_errno.h"

#include "intel_lw_uart_regs.h"
#include "intel_lw_uart.h"

#ifdef ALT_SINGLE_THREADED
  #if INTEL_LW_UART_TX_TIMEOUT_CYCLE > 0
#include "priv/alt_busy_sleep.h"
  #endif
#endif

#if defined(ALT_USE_SMALL_DRIVERS) || defined(INTEL_LW_UART_SMALL)

/* ----------------------------------------------------------- */
/* ------------------------ SMALL DRIVER --------------------- */
/* ----------------------------------------------------------- */

/* intel_lw_uart_write
 *
 * Private function: intel_lw_uart_write() is called by the system write() function in 
 * order to write a block of data to the UART.
 *
 * Permission checks are made before the call to intel_lw_uart_write(), so
 * we know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * This function will block on the devices transmit register, until all 
 * characters have been transmitted. This is unless the device is being 
 * accessed in non-blocking mode. In this case this function will return as 
 * soon as the device reports that it is not ready to transmit.
 *
 * Since this is the small footprint version of the UART driver, the value of 
 * CTS is ignored.
 *
 *** Arguments:
 * - *sp        : pointer to the LW UART device state.
 * - *ptr       : source address.
 * - len        : length of the data to write.
 * - flags      : for indicating blocking/non-blocking access for single/multi-threaded.
 *
 *** Returns:
 *   Number of bytes written -> success.
 *   -EWOULDBLOCK            -> the write operation would have blocked, 
 *                              but the file descriptor was placed in non-blocking mode.
 */

int 
intel_lw_uart_write(intel_lw_uart_state* sp, const char* ptr, int len,
  int flags)
{
    int block;
    unsigned int status;
    int count;

    block = !(flags & O_NONBLOCK);
    count = len;

    do
    {
        status = IORD_INTEL_LW_UART_STATUS(sp->base);
       
        if (status & INTEL_LW_UART_STATUS_TRDY_MSK)
        {
            IOWR_INTEL_LW_UART_TXDATA(sp->base, *ptr++);
            count--;
        }
    }
    while (block && count);

    if (count)
    {
        ALT_ERRNO = EWOULDBLOCK;
    }

    return (len - count);
}

#else /* Using the "fast" version of the driver */

/* ----------------------------------------------------------- */
/* ------------------------- FAST DRIVER --------------------- */
/* ----------------------------------------------------------- */

/* intel_lw_uart_write_enable_interrupt
 *
 * Private function: intel_lw_uart_write_enable_interrupt() is called by 
 * the intel_lw_uart_write() function in order to enable necessary interrupt
 * for write operation.
 *
 *** Arguments:
 * - *sp        : pointer to the LW UART device state.
 *
 *** Returns: 
 *   None
 */

void 
intel_lw_uart_write_enable_interrupt(intel_lw_uart_state* sp)
{
    alt_irq_context context;

    context = alt_irq_disable_all ();
    sp->ctrl |= INTEL_LW_UART_CONTROL_TRDY_MSK;

    if(sp->flags & INTEL_LW_UART_FC)
    {
         sp->ctrl |= (INTEL_LW_UART_CONTROL_DCTS_MSK |
                      INTEL_LW_UART_CONTROL_RTS_MSK);
    
         /* disable transmit empty interrupt, so that RTS will not be deasserted before new data is processed.*/
         sp->ctrl &= ~INTEL_LW_UART_CONTROL_TMT_MSK;
    }

    IOWR_INTEL_LW_UART_CONTROL(sp->base, sp->ctrl);
    alt_irq_enable_all (context);
}

/* intel_lw_uart_write
 *
 * Private function: intel_lw_uart_write() is called by the system write() function in order
 * to write a block of data to the UART.
 *
 * Permission checks are made before the call to intel_lw_uart_write(), so
 * we know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * This function does not communicate with the device directly. Instead data is
 * transfered to a circular buffer. The interrupt handler is then responsible
 * for copying data from this buffer into the device.
 *
 *** Arguments:
 * - *sp        : pointer to the LW UART device state.
 * - *ptr       : source address.
 * - len        : length of the data to write.
 * - flags      : for indicating blocking/non-blocking access for single/multi-threaded.
 *
 *** Returns:
 *   Number of bytes written -> success.
 *   -EWOULDBLOCK            -> the write operation would have blocked, 
 *                              but the file descriptor was placed in non-blocking mode.
 *   -ETIME                 -> write timeout.
 */

int
intel_lw_uart_write(intel_lw_uart_state* sp, const char* ptr, int len,
  int flags)
{
    int             no_block;
    alt_u32         next;
    int             count = len;
    alt_u8          write_would_block = 0;
#ifdef ALT_SINGLE_THREADED
    #if INTEL_LW_UART_TX_TIMEOUT_CYCLE > 0
    int             write_timeout_cnt = 0;
    alt_u8          write_would_timeout = 0;
    #endif
#endif
    /* 
     * Construct a flag to indicate whether the device is being accessed in
     * blocking or non-blocking mode.
     */

    no_block = (flags & O_NONBLOCK);

    /*
     * When running in a multi threaded environment, obtain the "write_lock"
     * semaphore. This ensures that writing to the device is thread-safe.
     */

    ALT_SEM_PEND (sp->write_lock, 0);

    /*
     * Loop transferring data from the input buffer to the transmit circular
     * buffer. The loop is terminated once all the data has been transferred,
     * or, (if in non-blocking mode) the buffer becomes full.
     */

    while (count)
    {
        /* Determine the next slot in the buffer to access */

        next = (sp->tx_end + 1) & sp->tx_buf_mask;

        /* block waiting for space if necessary */

        if (next == sp->tx_start)
        {
            if (no_block)
            {
                /* Set errno to indicate why this function returned early */
     
                ALT_ERRNO = EWOULDBLOCK;
                write_would_block = 1;
                break;
            }
            else
            {
                /* Block waiting for space in the circular buffer */

                /* First, ensure transmit interrupts are enabled to avoid deadlock */
                intel_lw_uart_write_enable_interrupt(sp);

                /* wait for space to come free */

                do
                {
                    /*
                     * When running in a multi-threaded mode, we pend on the write event 
                     * flag set in the interrupt service routine. This avoids wasting CPU
                     * cycles waiting in this thread, when we could be doing something
                     * more profitable elsewhere.
                     */

                    ALT_FLAG_PEND (sp->events, 
                                   INTEL_LW_UART_WRITE_RDY,
                                   ALT_FLAG_WAIT_SET_ANY_WITH_CONSUME,
                                   ALT_FLAG_WAIT_MAX_TIMEOUT);

#ifdef ALT_SINGLE_THREADED
    #if INTEL_LW_UART_TX_TIMEOUT_CYCLE > 0
                    /* break the dead lock if buffer is still full after timeout*/
                    if( INTEL_LW_UART_RX_TIMEOUT_CYCLE < write_timeout_cnt )
                    {
                       ALT_ERRNO = ETIME;
                       write_would_timeout = 1;
                       break;
                    } 
                    else 
                    {
                       alt_busy_sleep(INTEL_LW_UART_TX_TIMEOUT_US);
                       write_timeout_cnt++;
                    }
    #endif
#endif
                }
                while ((next == sp->tx_start));
            }
        }

#ifdef ALT_SINGLE_THREADED
    #if INTEL_LW_UART_TX_TIMEOUT_CYCLE > 0
        if(1 == write_would_timeout)
        {
            /*break the loop upon timeout, and stop sending the rest.*/
            break;
        }
        else
    #endif
#endif
        {
            count--;

            /* Add the next character to the transmit buffer */

            sp->tx_buf[sp->tx_end] = *ptr++;
            sp->tx_end = next;
        }
    }

    /*
     * Now that access to the circular buffer is complete, release the write
     * semaphore so that other threads can access the buffer.
     */

    ALT_SEM_POST (sp->write_lock);

    /* 
     * Ensure that interrupts are enabled, so that the circular buffer can 
     * drain.
     */
    intel_lw_uart_write_enable_interrupt(sp);

    /* return the number of bytes written */
    if(write_would_block) 
    {
      return -EWOULDBLOCK;
    }
#ifdef ALT_SINGLE_THREADED
    #if INTEL_LW_UART_TX_TIMEOUT_CYCLE > 0
    else if (write_would_timeout)
    {
        return -ETIME;
    }
    #endif
#endif
    else 
    {
        return (len - count);
    }
}

#endif /* fast driver */
