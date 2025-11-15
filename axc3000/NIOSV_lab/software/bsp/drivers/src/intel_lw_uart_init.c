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
#include "priv/alt_file.h"

#include "intel_lw_uart.h"
#include "intel_lw_uart_regs.h"

#if !defined(ALT_USE_SMALL_DRIVERS) && !defined(INTEL_LW_UART_SMALL)

/* ----------------------------------------------------------- */
/* ------------------------- FAST DRIVER --------------------- */
/* ----------------------------------------------------------- */

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void intel_lw_uart_irq(void* context);
#else
static void intel_lw_uart_irq(void* context, alt_u32 id);
#endif 

static void intel_lw_uart_rxirq(intel_lw_uart_state* sp,
  alt_u32 status);
static void intel_lw_uart_txirq(intel_lw_uart_state* sp,
  alt_u32 status);
static void intel_lw_uart_dctsirq(intel_lw_uart_state* sp,
  alt_u32 status);
static void intel_lw_uart_tmtirq(intel_lw_uart_state* sp,
  alt_u32 status);


/* intel_lw_uart_init
 *
 * Private function: intel_lw_uart_init() is called by the auto-generated function 
 * alt_sys_init() in order to initialize a particular instance of this device.
 * It is responsible for configuring the device and associated software 
 * constructs.
 *
 *** Arguments:
 * - *sp               : pointer to the LW UART device state.
 * - irq_controller_id : interrupt controller id.
 * - irq               : interrupt number.
 *
 *** Returns:
 *   None
 */

void 
intel_lw_uart_init(intel_lw_uart_state* sp, 
  alt_u32 irq_controller_id,  alt_u32 irq)
{
    void* base = sp->base;
    int error;

    /* 
     * Initialize the read and write flags and the semaphores used to 
     * protect access to the circular buffers when running in a multi-threaded
     * environment.
     */
    error = ALT_FLAG_CREATE (&sp->events, 0)    || 
            ALT_SEM_CREATE (&sp->read_lock, 1)  ||
            ALT_SEM_CREATE (&sp->write_lock, 1);

    /*initialize all call back function with null*/
    sp->eop_cb = NULL;
    sp->eh_cb = NULL;

    if (!error)
    {
        /* enable interrupts at the device */
        sp->ctrl = INTEL_LW_UART_CONTROL_RRDY_MSK;

        if(sp->flags & INTEL_LW_UART_FC)
        {
            sp->ctrl |= INTEL_LW_UART_CONTROL_DCTS_MSK;
        }

        IOWR_INTEL_LW_UART_CONTROL(base, sp->ctrl);

        /* register the interrupt handler */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
        alt_ic_isr_register(irq_controller_id, irq, intel_lw_uart_irq, sp, 
        0x0);
#else
        alt_irq_register (irq, sp, intel_lw_uart_irq);
#endif
    }
}


/* intel_lw_uart_irq
 *
 * Private function: intel_lw_uart_irq() is the interrupt handler registered at 
 * configuration time for processing UART interrupts. It vectors 
 * interrupt requests to either intel_lw_uart_rxirq() (for incoming 
 * data), or intel_lw_uart_txirq() (for outgoing data).
 *
 *** Arguments:
 * - *context : pointer to the LW UART device state.
 * - id       : interrupt controller id. (only if ALT_ENHANCED_INTERRUPT_API_PRESENT is undefined.)
 *
 *** Returns:
 *   None
 */

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void intel_lw_uart_irq(void* context)
#else
static void intel_lw_uart_irq(void* context, alt_u32 id)
#endif
{
    alt_u32 status;

    intel_lw_uart_state* sp = (intel_lw_uart_state*) context;
    void* base               = sp->base;

    /*
     * Read the status register in order to determine the cause of the
     * interrupt.
     */

    status = IORD_INTEL_LW_UART_STATUS(base);

    /* process a read irq */
    if (status & INTEL_LW_UART_STATUS_RRDY_MSK)
    {
        intel_lw_uart_rxirq(sp, status);
    }

    /* process a write irq */
    if (status & (INTEL_LW_UART_STATUS_TRDY_MSK | 
                  INTEL_LW_UART_STATUS_DCTS_MSK))
    {
        intel_lw_uart_txirq(sp, status);
    }

    /* update status, in case tx or rx irq is processed earlier. */
    status = IORD_INTEL_LW_UART_STATUS(base);

    /*process dcts irq if flow control is enabled.*/
    if(sp->flags & INTEL_LW_UART_FC)
    {
        if (status & INTEL_LW_UART_STATUS_DCTS_MSK)
        {
            intel_lw_uart_dctsirq(sp, status);
        }

        /*only service the routine, if data is being transmitted.*/
        if( (status & INTEL_LW_UART_STATUS_TMT_MSK) &&
            (sp->ctrl & INTEL_LW_UART_CONTROL_TMT_MSK) )
        {
            intel_lw_uart_tmtirq(sp, status);
        }
    }

    /*eop service*/
    if(sp->flags & INTEL_LW_UART_EOP)
    {
        if (status & INTEL_LW_UART_STATUS_EOP_MSK)
        {
            if(NULL != sp->eop_cb)
            {
                sp->eop_cb();
            }
        }
    }

    /*exception handling call back*/
    if (status & INTEL_LW_UART_STATUS_E_MSK)
    {
        if(NULL != sp->eh_cb)
        {
            sp->eh_cb(base, status);
        }
    }

    /* Clear any error flags set at the device */
    IOWR_INTEL_LW_UART_STATUS(base, 0);

    /* Dummy read to ensure IRQ is negated before ISR returns 
     * Comment out as this is already the end of ISR.
     */
    //IORD_INTEL_LW_UART_STATUS(base);
}


/* intel_lw_uart_rxirq
 *
 * Private function: intel_lw_uart_rxirq() is called by intel_lw_uart_irq() to 
 * process a receive interrupt. It transfers the incoming character into 
 * the receive circular buffer, and sets the apropriate flags to indicate 
 * that there is data ready to be processed.
 *
 *** Arguments:
 * - *sp         : pointer to the LW UART device state.
 * - status      : individual bits that indicate conditions inside the LW UART core.
 *
 *** Returns:
 *   None
 */

static void 
intel_lw_uart_rxirq(intel_lw_uart_state* sp, alt_u32 status)
{
    alt_u32 next;

    /* If there was an error, discard the data */

    if (status & (INTEL_LW_UART_STATUS_PE_MSK | 
                  INTEL_LW_UART_STATUS_FE_MSK))
    {
        return;
    }

    /*
     * In a multi-threaded environment, set the read event flag to indicate
     * that there is data ready. This is only done if the circular buffer was
     * previously empty.
     */

    if (sp->rx_end == sp->rx_start)
    {
        ALT_FLAG_POST (sp->events, INTEL_LW_UART_READ_RDY, ALT_FLAG_SET);
    }

    /*
     * If the cicular buffer was full, disable interrupts. Interrupts will be
     * re-enabled when data is removed from the buffer.
     */

    do
    {
        /* Determine which slot to use next in the circular buffer */
        next = (sp->rx_end + 1) & sp->rx_buf_mask;

        if (next == sp->rx_start)
        {
            /*circular buffer is full*/
            sp->ctrl &= ~INTEL_LW_UART_CONTROL_RRDY_MSK;
            break;
        }
        else
        {
            /* Transfer data from the device to the circular buffer */
            sp->rx_buf[sp->rx_end] = IORD_INTEL_LW_UART_RXDATA(sp->base);

            if ( status & (INTEL_LW_UART_STATUS_BRK_MSK|INTEL_LW_UART_STATUS_PE_MSK|INTEL_LW_UART_STATUS_FE_MSK) )
            {
                /* discard corrupted data and wait for next rx interrupt */
                break;
            }
            else
            {
                /* save valid data in the circular buffer */
                sp->rx_end = next;
            }
        }

        /*update status to check if rx fifo is empty.*/
        status = IORD_INTEL_LW_UART_STATUS(sp->base);
    }
    while(status&INTEL_LW_UART_STATUS_RRDY_MSK);

    /* todo : this is a workaround.
     * this solves the problem, when tmt irq deassert RTS, but other UART is still transmitting.
     * RTS is asserted here to avoid blocking of other UART transmit.
     * Proper solution: should check for rx shift not empty in tmt irq(not yet implemented in ip) .
     */
   
    /* check if other UART still request to send.*/
    if(status & INTEL_LW_UART_STATUS_CTS_MSK)
    {
        /* if fifo is not full, set RTS to enable send.*/
        if( !(status & (INTEL_LW_UART_CONTROL_RAFULL_MSK|INTEL_LW_UART_CONTROL_RFULL_MSK)) )
        { 
            sp->ctrl |= INTEL_LW_UART_CONTROL_RTS_MSK;
        }
    }

    IOWR_INTEL_LW_UART_CONTROL(sp->base, sp->ctrl); 
}


/* intel_lw_uart_txirq
 *
 * Private function: intel_lw_uart_txirq() is called by intel_lw_uart_irq() to 
 * process a transmit interrupt. It transfers data from the transmit 
 * buffer to the device, and sets the apropriate flags to indicate that 
 * there is data ready to be processed.
 *
 *** Arguments:
 * - *sp         : pointer to the LW UART device state.
 * - status      : individual bits that indicate conditions inside the LW UART core.
 *
 *** Returns:
 *   None
 */

static void 
intel_lw_uart_txirq(intel_lw_uart_state* sp, alt_u32 status)
{
    alt_u8 data_sent = 0;

    /* Transfer data if there is some ready to be transfered */

    while ((sp->tx_start != sp->tx_end) && !(sp->ctrl & INTEL_LW_UART_CONTROL_TRBK_MSK))
    {
        /*
         * There is a race condition here. "status" may indicate that 
         * CTS is low, but it actually went high before DCTS was cleared on 
         * the last write to the status register. To avoid this resulting in
         * deadlock, it's necessary to re-check the status register here
         * before throttling.
         */
        status = IORD_INTEL_LW_UART_STATUS(sp->base); 

        /* 
         * stop sending if tx fifo is full, 
         * interrupt will trigger again after data is shifted out.
         */
        if( !(status & INTEL_LW_UART_STATUS_TRDY_MSK) )
        {
            break;
        }

        /* 
         * If the device is using flow control (i.e. RTS/CTS), then the
         * transmitter is required to throttle if CTS is high.
         */

        if (!(sp->flags & INTEL_LW_UART_FC) ||
            (status & INTEL_LW_UART_STATUS_CTS_MSK))
        {

            /*
             * In a multi-threaded environment, set the write event flag to indicate
             * that there is space in the circular buffer. This is only done if the
             * buffer was previously empty.
             */

            if (sp->tx_start == ((sp->tx_end + 1) & sp->tx_buf_mask))
            {
                ALT_FLAG_POST (sp->events, 
                               INTEL_LW_UART_WRITE_RDY,
                               ALT_FLAG_SET);
            }

            /* Write the data to the device */
            IOWR_INTEL_LW_UART_TXDATA(sp->base, sp->tx_buf[sp->tx_start]);

            sp->tx_start = ((sp->tx_start) + 1) & sp->tx_buf_mask;

            /*
             * In case the tranmit interrupt had previously been disabled by 
             * detecting a low value on CTS, it is reenabled here.
             */ 

            sp->ctrl |= INTEL_LW_UART_CONTROL_TRDY_MSK;

            data_sent = 1;
        }
        else
        {
            /*
             * CTS is low and we are using flow control, so disable the transmit
             * interrupt while we wait for CTS to go high again. This will be 
             * detected using the DCTS interrupt.
             */

            if (!(status & INTEL_LW_UART_STATUS_CTS_MSK))
            {
                sp->ctrl &= ~INTEL_LW_UART_CONTROL_TRDY_MSK;
            }
            /* break to avoid dead lock.*/
            break;
        }
    }

    if(sp->ctrl & INTEL_LW_UART_CONTROL_TRBK_MSK)
    {
        /*flush the driver tx buffer*/
        sp->tx_start = sp->tx_end;
        
        /*set data sent to deassert RTS*/
        data_sent = 1;
    }

    /*
     * If the circular buffer is empty, disable the interrupt. This will be
     * re-enabled when new data is placed in the buffer.
     * Transmit empty interrupt will be enabled to deassert RTS signal.
     */

    if (sp->tx_start == sp->tx_end)
    {
        if(sp->flags & INTEL_LW_UART_FC) 
        {
            if (1 == data_sent)
            {
                sp->ctrl |= INTEL_LW_UART_CONTROL_TMT_MSK;
            }
        }

        sp->ctrl &= ~INTEL_LW_UART_CONTROL_TRDY_MSK;
    }

    IOWR_INTEL_LW_UART_CONTROL(sp->base, sp->ctrl);
}


/* intel_lw_uart_dctsirq
 *
 * Private function: intel_lw_uart_dctsirq() is called by intel_lw_uart_irq() to 
 * process a change in cts signal interrupt. It will assert or deassert RTS according to
 * the CTS signal, that is comming from the other UART.
 * Before deasserting RTS, this function will also check, if there are still 
 * data to be transmited, so that the transmit will not be interrupted. 
 *
 *** Arguments:
 * - *sp         : pointer to the LW UART device state.
 * - status      : individual bits that indicate conditions inside the LW UART core.
 *
 *** Returns:
 *   None
 */

static void 
intel_lw_uart_dctsirq(intel_lw_uart_state* sp, alt_u32 status)
{
    if(status & INTEL_LW_UART_STATUS_CTS_MSK)
    {
        /*only assert RTS signal, if RTS from other UART is asserted.*/
        sp->ctrl |= (INTEL_LW_UART_CONTROL_RTS_MSK|INTEL_LW_UART_CONTROL_TRDY_MSK);
    }
    else
    {
        /*
         * If RTS from other UART is deasserted,
         * only deassert RTS, if there are not more data to be sent.
         */
        if( (sp->tx_start == sp->tx_end) && 
            (status & INTEL_LW_UART_STATUS_TMT_MSK) )
        {
            sp->ctrl &= ~INTEL_LW_UART_CONTROL_RTS_MSK;
        }
    }

    IOWR_INTEL_LW_UART_CONTROL(sp->base, sp->ctrl); 
}


/* intel_lw_uart_tmtirq
 *
 * Private function: intel_lw_uart_tmtirq() is called by intel_lw_uart_irq() to 
 * process a transmit empty interrupt.
 * This is triggered after tx fifo and transmit shift is empty
 * The RTS signal will be deasserted and interrupt will also be disabled.
 *
 *** Arguments:
 * - *sp         : pointer to the LW UART device state.
 * - status      : individual bits that indicate conditions inside the LW UART core.
 *
 *** Returns:
 *   None
 */

static void 
intel_lw_uart_tmtirq(intel_lw_uart_state* sp, alt_u32 status)
{
    /* turn off interrupt to avoid dead lock. */
    sp->ctrl &= ~INTEL_LW_UART_CONTROL_TMT_MSK;

    /* only turn off RTS if currently is not receiving or sending. */

    /* todo : check if rx shift is empty, after it is implemented. */
    //if( !(status & INTEL_LW_UART_STATUS_RMT_MSK) )
    {
        sp->ctrl &= ~INTEL_LW_UART_CONTROL_RTS_MSK;
    }

    IOWR_INTEL_LW_UART_CONTROL(sp->base, sp->ctrl);
}


/* intel_lw_uart_close
 *
 * Private function: routine is implemented to drain the UART transmit buffer
 * when not in "small" mode. This routine will wait for transimt data to be
 * emptied unless the driver flags have been set to non-blocking mode. 
 * This routine should be called indirectly (i.e. though the C library 
 * close() routine) so that the file descriptor associated with the relevant 
 * stream (i.e. stdout) can be closed as well. This routine does not manage 
 * file descriptors.
 * 
 * The close routine is not implemented for the small driver; instead it will
 * map to null. This is because the small driver simply waits while characters
 * are transmitted; there is no interrupt-serviced buffer to empty 
 *
 *** Arguments:
 * - *sp         : pointer to the LW UART device state.
 * - flags      : for indicating blocking/non-blocking access for single/multi-threaded.
 *
 *** Returns:
 *   0                      -> success.
 *  -EWOULDBLOCK            -> the close operation would have blocked, 
 *                             but the file descriptor was placed in non-blocking mode.
 */

int
intel_lw_uart_close(intel_lw_uart_state* sp, int flags)
{
    /* 
     * Wait for all transmit data to be emptied by the UART ISR.
     */
    while (sp->tx_start != sp->tx_end)
    {
        if (flags & O_NONBLOCK)
        {
            ALT_ERRNO = EWOULDBLOCK;
            return -EWOULDBLOCK; 
        }
    }

    return 0;
}

/* intel_lw_uart_tmtirq
 *
 * Private function: intel_lw_uart_tmtirq() can be used to 
 * access file descriptor structure alt_fd*.
 * The function input (const int file) is the file descriptor (fd) 
 * It is assumed that the user gets the correct fd by using open().
 * If fopen is used, please use fileno to get the fd.
 *
 * Return alt_fd* type, so that caller function can check fd_flags for valid access.
 *
 *** Arguments:
 * - file    : the LW UART file descriptor.
 *
 *** Returns:
 *   *fd -> success and return pointer to file descriptor structure
 *   Null    -> device is not found.
 */

static alt_fd*
intel_lw_uart_find_dev (const int file)
{
    alt_fd*  fd;
    
    fd = (file < 0) ? NULL : &alt_fd_list[file];

    return fd;
}


/* intel_lw_uart_init_eop
 *
 * Public function: intel_lw_uart_init_eop() can be used to 
 * enable EOP interrupt and initialize the call back function.
 * User should use this in their application init, so that it will not intefer with the driver during run time.
 *
 *** Arguments:
 * - file       : the LW UART file descriptor.
 * - eop_cb_in  : user defined function, that is called in EOP interrupt.
 * - eop        : character to trigger eop interrupt.
 *
 *** Returns:
 *  0           -> success.
 * -1           -> invalid file descriptor or call back function is null or EOP option disabled.
 * -EACCES      -> file descriptor is read only.
 */

alt_8
intel_lw_uart_init_eop (const int file, const EOP_Callback eop_cb_in, const alt_u8 eop)
{
    alt_8 rval = 0;
    const alt_fd*  const fd = intel_lw_uart_find_dev(file);
    intel_lw_uart_dev* dev = NULL;

    if(NULL == fd)
    {
        return -1;
    }
    else
    {
        if( O_RDONLY == (fd->fd_flags & O_ACCMODE) ) 
        {
            ALT_ERRNO = EACCES;
            return -EACCES;
        }
        else
        {
            dev = (intel_lw_uart_dev*) fd->dev; 
        }
    }

    if(!(dev->state.flags & INTEL_LW_UART_EOP) ||
        (NULL == eop_cb_in)
      )
    {
        rval = -1;
    }
    else
    {
        /*set callback function*/
        dev->state.eop_cb = eop_cb_in;

        /*set eop character*/
        IOWR_INTEL_LW_UART_EOP(dev->state.base, eop);

        /*enable interrupt*/
        dev->state.ctrl |= INTEL_LW_UART_CONTROL_EOP_MSK;	
        IOWR_INTEL_LW_UART_CONTROL(dev->state.base, dev->state.ctrl);
    }

    return rval;
}


/* intel_lw_uart_init_eh
 *
 * Public function: intel_lw_uart_init_eh() an be used to 
 * enable E interrupt and set the call back function for exception handling.
 * The call back function has status and base address of device as input to identify the source of exception.
 * User should use this in their application init, so that it will not intefer with the driver during run time.
 *
 *** Arguments:
 * - file       : the LW UART file descriptor.
 * - eh_cb_in  : user defined function, that is called in EH interrupt.
 *
 *** Returns:
 *  0           -> success.
 * -1           -> invalid file descriptor or call back function is null.
 * -EACCES      -> file descriptor is read only.
 */

alt_8
intel_lw_uart_init_eh (const int file, const EH_Callback eh_cb_in)
{
    alt_8 rval = 0;
    const alt_fd*  const fd = intel_lw_uart_find_dev(file);
    intel_lw_uart_dev* dev = NULL;

    if(NULL == fd)
    {
        return -1;
    }
    else
    {
        if( O_RDONLY == (fd->fd_flags & O_ACCMODE) ) 
        {
            return -EACCES;
        }
        else
        {
            dev = (intel_lw_uart_dev*) fd->dev; 
        }
    }

    if(NULL == eh_cb_in)
    {
        rval = -1;
    }
    else
    {
        /*set callback function*/
        dev->state.eh_cb = eh_cb_in;

        /*enable interrupt*/
        dev->state.ctrl |= INTEL_LW_UART_CONTROL_E_MSK;	
        IOWR_INTEL_LW_UART_CONTROL(dev->state.base, dev->state.ctrl);
    }

    return rval;
}

/* intel_lw_uart_set_trbk
 *
 * Public function: intel_lw_uart_set_trbk() can be called by the user application to
 * transmit a break character over the TXD output. The TXD signal is forced to 0 when
 * the TRBK bit is set to 1. The TRBK bit overrides any logic level that the
 * transmitter logic would otherwise drive on the TXD output. The TRBK bit
 * interferes with any transmission in process. The user must set the TRBK bit back to 0
 * after an appropriate break period elapses.
 *
 *** Arguments:
 * - file       : the LW UART file descriptor.
 * - enable     : set INTEL_LW_UART_TRBK_ENABLE or INTEL_LW_UART_TRBK_DISABLE.
 *
 *** Returns:
 *  0           -> success.
 * -1           -> invalid file descriptor or invalid enable.
 * -EACCES      -> file descriptor is read only.
 */
 
alt_8
intel_lw_uart_set_trbk (const int file, const alt_u8 enable)
{
    alt_8 rval = 0;
    const alt_fd*  const fd = intel_lw_uart_find_dev(file);
    intel_lw_uart_dev* dev = NULL;
    
    if(NULL == fd)
    {
        return -1;
    }
    else
    {
        if( O_RDONLY == (fd->fd_flags & O_ACCMODE) ) 
        {
            return -EACCES;
        }
        else
        {
            dev = (intel_lw_uart_dev*) fd->dev; 
        }
    }
    
    if(INTEL_LW_UART_TRBK_ENABLE == enable)
    {
        dev->state.ctrl |= INTEL_LW_UART_CONTROL_TRBK_MSK;
        
        /* flush the driver tx buffer*/
        dev->state.tx_start = dev->state.tx_end;
    }
    else if (INTEL_LW_UART_TRBK_DISABLE == enable)
    {
        dev->state.ctrl &= ~INTEL_LW_UART_CONTROL_TRBK_MSK;
    }
    else
    {
        /* do nothing if user give invalid enable.*/
        rval = -1;
    }
    
    if(0 == rval)
    {
        IOWR_INTEL_LW_UART_CONTROL(dev->state.base, dev->state.ctrl);
    }
    
    return rval;
};

#endif /* fast driver */
