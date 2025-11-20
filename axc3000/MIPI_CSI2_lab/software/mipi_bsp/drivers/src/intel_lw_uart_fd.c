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

#include "alt_types.h"
#include "sys/alt_dev.h"
#include "intel_lw_uart.h"

extern int intel_lw_uart_read(intel_lw_uart_state* sp,
  char* buffer, int space, int flags);
extern int intel_lw_uart_write(intel_lw_uart_state* sp,
  const char* ptr, int count, int flags);
extern int intel_lw_uart_ioctl(intel_lw_uart_state* sp,
  int req, void* arg);
extern int intel_lw_uart_close(intel_lw_uart_state* sp, int flags);

/* ----------------------------------------------------------------------- */
/* --------------------- WRAPPERS FOR ALT FD SUPPORT --------------------- */
/*
 *
 */

/* intel_lw_uart_read_fd
 *
 * Public function: intel_lw_uart_read_fd() is a wrapper function to redirect system call
 * to driver function intel_lw_uart_read(). 
 * Should be called indirectly by using system function fread or read.
 *
 *** Arguments:
 * - *fd                      : the UART device file descriptor.
 * - ptr                      : destination address.
 * - len                      : maximum length of the data to read.
 *
 *** Returns:
 *    Number of bytes read    -> successful.
 *   -EWOULDBLOCK             -> the read operation would have blocked, 
 *                               but the file descriptor was placed in non-blocking mode.
 *   -ETIME                   -> read timeout.(invalid for small driver)
 */
 
int 
intel_lw_uart_read_fd(alt_fd* fd, char* buffer, int len)
{
    intel_lw_uart_dev* dev = (intel_lw_uart_dev*) fd->dev; 

    return intel_lw_uart_read(&dev->state, buffer, len,
                              fd->fd_flags);
}

/* intel_lw_uart_write_fd
 *
 * Public function: intel_lw_uart_write_fd() is a wrapper function to redirect system call
 * to driver function intel_lw_uart_write(). 
 * Should be called indirectly by using system function fwrite, write or fprintf.
 *
 *** Arguments:
 * - *fd    : the UART device file descriptor.
 * - ptr    : source address.
 * - len    : length of the data to write.
 *
 *
 *** Returns:
 *   Number of bytes written -> successful.
 *  -EWOULDBLOCK             -> the write operation would have blocked, 
 *                              but the file descriptor was placed in non-blocking mode.
 *  -ETIME                   -> write timeout.(invalid for small driver)
 */
 
int 
intel_lw_uart_write_fd(alt_fd* fd, const char* buffer, int len)
{
    intel_lw_uart_dev* dev = (intel_lw_uart_dev*) fd->dev; 

    return intel_lw_uart_write(&dev->state, buffer, len,
                               fd->fd_flags);
}

#if !defined(ALT_USE_SMALL_DRIVERS) && !defined(INTEL_LW_UART_SMALL)

/*
 * Fast driver
 */

/*
 * To reduce the code footprint of this driver, the ioctl() function is not
 * included by default. If you wish to use the ioctl features provided 
 * below, you can do so by adding the option : -DINTEL_LW_UART_USE_IOCTL
 * to CPPFLAGS in the Makefile (or through the Eclipse IDE).
 */

#ifdef INTEL_LW_UART_USE_IOCTL

/* intel_lw_uart_ioctl_fd
 *
 * Public function: iWrapper function to redirect system call to driver function intel_lw_uart_ioctl. 
 * Should be called indirectly by using system function ioctl.
 *
 *** Arguments:
 * - *fd     : the UART device file descriptor.
 * - reg     : IOCTL request: TIOCMGET, TIOCMSET, TIOCEXCL, TIOCNXCL.
 * - *arg    : input termios structure.
 *
 *** Returns:
 *  0        -> success.
 * -ENOTTY   -> unsupported request.
 * -EIO      -> unsupported termios setting.
 */
 
int 
intel_lw_uart_ioctl_fd(alt_fd* fd, int req, void* arg)
{
    intel_lw_uart_dev* dev = (intel_lw_uart_dev*) fd->dev;

    return intel_lw_uart_ioctl(&dev->state, req, arg);
}

#endif /* INTEL_LW_UART_USE_IOCTL */


/* intel_lw_uart_close_fd
 *
 * Public function: intel_lw_uart_close_fd() is a wrapper function to redirect system call
 * to driver function intel_lw_uart_close(). 
 * Should be called indirectly by using system function fclose or close.
 *
 *** Arguments:
 * - *fd                    : the UART device file descriptor.
 *
 *** Returns:
 *   0                      -> success.
 *  -EWOULDBLOCK            -> the close operation would have blocked, 
 *                             but the file descriptor was placed in non-blocking mode.
 */
 
int 
intel_lw_uart_close_fd(alt_fd* fd)
{
    intel_lw_uart_dev* dev = (intel_lw_uart_dev*) fd->dev; 

    return intel_lw_uart_close(&dev->state, fd->fd_flags);
}

#endif /* fast driver */
