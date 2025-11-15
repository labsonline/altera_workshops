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

#ifndef __INTEL_LW_UART_H__
#define __INTEL_LW_UART_H__

#include <stddef.h>
#include <sys/termios.h>

#include "sys/alt_warning.h"

#include "os/alt_sem.h"
#include "os/alt_flag.h"
#include "alt_types.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined(ALT_USE_SMALL_DRIVERS) || defined(INTEL_LW_UART_SMALL)

/*
 ***********************************************************************
 *********************** SMALL DRIVER **********************************
 ***********************************************************************
 */

/*
 * State structure definition. Each instance of the driver uses one
 * of these structures to hold its associated state.
 */

typedef struct intel_lw_uart_state_s
{
    unsigned int        base;
} intel_lw_uart_state;

/*
 * The macro INTEL_LW_UART_STATE_INSTANCE is used by the 
 * auto-generated file alt_sys_init.c to create an instance of this 
 * device driver state.
 */

#define INTEL_LW_UART_STATE_INSTANCE(name, state)   \
    intel_lw_uart_state state =                     \
    {                                               \
        name##_BASE                                 \
    }

/*
 * The macro INTEL_LW_UART_STATE_INIT is used by the auto-generated file
 * alt_sys_init.c to initialize an instance of the device driver state.
 */

#define INTEL_LW_UART_STATE_INIT(name, state)

#else /* fast driver */

/*
 **********************************************************************
 *********************** FAST DRIVER **********************************
 **********************************************************************
 */

/*
 * INTEL_LW_UART_READ_RDY and INTEL_LW_UART_WRITE_RDY are the bitmasks 
 * that define uC/OS-II event flags that are releated to this device.
 *
 * INTEL_LW_UART_READY_RDY indicates that there is read data in the buffer 
 * ready to be processed. INTEL_LW_UART_WRITE_RDY indicates that the transmitter is
 * ready for more data.
 */

#define INTEL_LW_UART_READ_RDY  0x1
#define INTEL_LW_UART_WRITE_RDY 0x2


/*
 * This is somewhat of an ugly hack, but we need some mechanism for
 * representing the non-standard 9 bit mode provided by this UART. In this
 * case we abscond with the 5 bit mode setting. The value CS5 is defined in
 * termios.h.
 */

#define CS9 CS5

/*
 * The value INTEL_LW_UART_FB is a value set in the devices flag field to
 * indicate that the device has a fixed baud rate; i.e. if this flag is set
 * software can not control the baud rate of the device.
 */

#define INTEL_LW_UART_FB 0x1

/*
 * The value INTEL_LW_UART_FC is a value set in the device flag field to
 * indicate that the device is using flow control, i.e. the driver must 
 * throttle on transmit if the nCTS pin is low.
 */

#define INTEL_LW_UART_FC 0x2

/*
 * The value INTEL_LW_UART_EOP is a value set in the device flag field to
 * indicate that the device is using end of packet(EOP) control. 
 * The driver provides a call back funtion, when EOP interrupt is triggered.
 * The user should use intel_lw_uart_init_eop to initialize this feature.
 */

#define INTEL_LW_UART_EOP 0x4

/*
 * The intel_lw_uart_state structure is used to hold device specific data.
 * This includes the transmit and receive buffers.
 *
 * An instance of this structure is created in the auto-generated 
 * alt_sys_init.c file for each UART listed in the systems PTF file. This is
 * done using the INTEL_LW_UART_STATE_INSTANCE macro given below.
 */

typedef void (*EOP_Callback)(void);
typedef void (*EH_Callback)(void* base, alt_u32 status);

typedef struct intel_lw_uart_state_s
{
    void*                  base;            /* The base address of the device */
    alt_u32                ctrl;            /* Shadow value of the control register */
    volatile alt_u32       rx_start;        /* Start of the pending receive data */
    volatile alt_u32       rx_end;          /* End of the pending receive data */
    volatile alt_u32       tx_start;        /* Start of the pending transmit data */
    volatile alt_u32       tx_end;          /* End of the pending transmit data */
#ifdef INTEL_LW_UART_USE_IOCTL
    struct termios         termios;         /* Current device configuration */
    alt_u32                freq;            /* Current baud rate */
#endif
    alt_u32                flags;           /* Configuation flags */
    ALT_FLAG_GRP           (events)         /* Event flags used for 
                                             * foreground/background in mult-threaded
                                             * mode */
    ALT_SEM                (read_lock)      /* Semaphore used to control access to the 
                                             * read buffer in multi-threaded mode */
    ALT_SEM                (write_lock)     /* Semaphore used to control access to the
                                             * write buffer in multi-threaded mode */
    volatile alt_u8*       rx_buf;          /* The pointer to receive buffer */
    volatile alt_u8*       tx_buf;          /* The pointer to transmit buffer */
    volatile alt_u16       rx_buf_mask;     /* The mask for end of receive buffer */
    volatile alt_u16       tx_buf_mask;     /* The mask for end of transmit buffer */
    volatile EOP_Callback  eop_cb;          /* Call back function for end of package*/
    volatile EH_Callback   eh_cb;           /* Call back function for exception condition*/
} intel_lw_uart_state;

/*
 * Conditionally define the data structures used to process ioctl requests.
 * The following macros are defined for use in creating a device instance:
 *
 * INTEL_LW_UART_TERMIOS - Initialize the termios structure used to
 *                              describe the UART configuration.
 * INTEL_LW_UART_FREQ    - Initialize the 'freq' field of the device
 *                              structure, if the field exists.
 * INTEL_LW_UART_IOCTL   - Initialize the 'ioctl' field of the device
 *                              callback structure, if ioctls are enabled.
 */

#ifdef INTEL_LW_UART_USE_IOCTL

#define INTEL_LW_UART_TERMIOS(stop_bits,                      \
                              no_parity,                      \
                              odd_parity,                     \
                              data_bits,                      \
                              ctsrts,                         \
                              baud)                           \
{                                                             \
    0,                                                        \
    0,                                                        \
    ((stop_bits == 2) ? CSTOPB: 0)      |                     \
      ((no_parity) ? 0: PARENB)         |                     \
      ((odd_parity) ? PAODD: 0)         |                     \
      ((data_bits == 7) ? CS7: (data_bits == 9) ? CS9: CS8) | \
      ((ctsrts) ? CRTSCTS : 0),                               \
    0,                                                        \
    0,                                                        \
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                  \
    baud,                                                     \
    baud                                                      \
},
#define INTEL_LW_UART_FREQ(name) name##_FREQ,

#else /* !INTEL_LW_UART_USE_IOCTL */

#define INTEL_LW_UART_TERMIOS(stop_bits,  \
                              parity,     \
                              odd_parity, \
                              data_bits,  \
                              ctsrts,     \
                              baud)
#define INTEL_LW_UART_FREQ(name)

#endif /* INTEL_LW_UART_USE_IOCTL */

/*
 * The macro INTEL_LW_UART_INSTANCE is used by the auto-generated file
 * alt_sys_init.c to create an instance of this device driver state.
 */

#define INTEL_LW_UART_STATE_INSTANCE(name, state)         \
    intel_lw_uart_state state =                           \
    {                                                     \
        (void*) name##_BASE,                              \
        0,                                                \
        0,                                                \
        0,                                                \
        0,                                                \
        0,                                                \
        INTEL_LW_UART_TERMIOS(name##_STOP_BITS,           \
                                  (name##_PARITY == 'N'), \
                                  (name##_PARITY == 'O'), \
                                  name##_DATA_BITS,       \
                                  name##_USE_CTS_RTS,     \
                                  name##_BAUD)            \
        INTEL_LW_UART_FREQ(name)                          \
        (name##_FIXED_BAUD ? INTEL_LW_UART_FB : 0) |      \
        (name##_USE_CTS_RTS ? INTEL_LW_UART_FC : 0)|      \
        (name##_USE_EOP_REGISTER ? INTEL_LW_UART_EOP : 0) \
    }

/*
 * intel_lw_uart_init() is called by the auto-generated function 
 * alt_sys_init() for each UART in the system. This is done using the 
 * INTEL_LW_UART_INIT macro given below.
 *
 * This function is responsible for performing all the run time initilisation
 * for a device instance, i.e. registering the interrupt handler, and 
 * regestering the device with the system.
 */
extern void intel_lw_uart_init(intel_lw_uart_state* sp,
                               alt_u32 irq_controller_id, alt_u32 irq);

/*
 * intel_lw_uart_init_eop() can be called by the user application to
 * configure the end of packet(eop) feature.
 */
extern alt_8 intel_lw_uart_init_eop (const int file, const EOP_Callback eop_cb_in, const alt_u8 eop);

/* 
 * intel_lw_uart_init_eh() can be called by the user application to
 * set user call back function that handle exceptions.
 * The call back function has status and base address of device 
 * as input to identify the source of exception.
 */
extern alt_8 intel_lw_uart_init_eh (const int file, const EH_Callback eh_cb_in);

/* 
 * intel_lw_uart_set_trbk() can be called by the user application to
 * transmit a break character over the TXD output. The TXD signal is forced to 0 when
 * the TRBK bit is set to 1. The TRBK bit overrides any logic level that the
 * transmitter logic would otherwise drive on the TXD output. The TRBK bit
 * interferes with any transmission in process. The user must set the TRBK bit back to 0
 * after an appropriate break period elapses.
 */
extern alt_8 intel_lw_uart_set_trbk (const int file, const alt_u8 enable);

/*
 * The macro INTEL_LW_UART_STATE_INIT is used by the auto-generated file
 * alt_sys_init.c to initialize an instance of the device driver state.
 *
 * This macro performs a sanity check to ensure that the interrupt has been
 * connected for this device. If not, then an apropriate error message is 
 * generated at build time.
 */

#define INTEL_LW_UART_STATE_INIT(name, state)                                  \
    if (name##_IRQ == ALT_IRQ_NOT_CONNECTED)                                   \
    {                                                                          \
        ALT_LINK_ERROR ("Error: Interrupt not connected for " #name ". "       \
                        "You have selected the interrupt driven version of "   \
                        "the Intel LW UART driver, but the interrupt is "      \
                        "not connected for this device. You can select a "     \
                        "polled mode driver by checking the 'small driver' "   \
                        "option in the HAL configuration window, or by "       \
                        "using the -DINTEL_LW_UART_SMALL preprocessor "        \
                        "flag.");                                              \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        intel_lw_uart_init(&state, name##_IRQ_INTERRUPT_CONTROLLER_ID,         \
        name##_IRQ);                                                           \
    }


/*
 * The macro INTEL_LW_UART_BUFF_INIT is used by the auto-generated file
 * alt_sys_init.c to allocate static memory for transmit and receive buffer
 * of the device driver.
 * The size of the buffer is equavilent to the fifo size of the IP, 
 * that is set in the platform desginer(qsys).
 *
 * This macro also initialize the value of mask to determine the end of 
 * transmit and receive buffer.
 */

#define INTEL_LW_UART_BUFF_INIT(name, dev)                                    \
        dev.state.rx_buf = dev##_rx_buf;                                      \
        dev.state.tx_buf = dev##_tx_buf;                                      \
        dev.state.rx_buf_mask = name##_READ_DEPTH-1;                          \
        dev.state.tx_buf_mask = name##_WRITE_DEPTH-1

#endif /* small driver */

/*
 * Include in case non-direct version of driver required.
 */
#include "intel_lw_uart_fd.h"

/*
 * Map alt_sys_init macros to direct or non-direct versions.
 */
#ifdef ALT_USE_DIRECT_DRIVERS

#define INTEL_LW_UART_INSTANCE(name, state) \
    INTEL_LW_UART_STATE_INSTANCE(name, state)
#define INTEL_LW_UART_INIT(name, state) \
    INTEL_LW_UART_STATE_INIT(name, state)

#else /* !ALT_USE_DIRECT_DRIVERS */

#define INTEL_LW_UART_INSTANCE(name, dev) \
    INTEL_LW_UART_DEV_INSTANCE(name, dev)
#define INTEL_LW_UART_INIT(name, dev) \
    INTEL_LW_UART_DEV_INIT(name, dev)

#endif /* ALT_USE_DIRECT_DRIVERS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INTEL_LW_UART_H__ */
