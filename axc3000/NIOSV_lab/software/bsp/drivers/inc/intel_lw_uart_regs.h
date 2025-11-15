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

#ifndef __INTEL_LW_UART_REGS_H__
#define __INTEL_LW_UART_REGS_H__

#include <io.h>

/* This factor is needed for the correct offset for the register. */
#define OFFSET_FACTOR (SYSTEM_BUS_WIDTH/8)

#define IOWR_16(BASE, REGNUM, DATA) \
  IOWR_16DIRECT(BASE, (REGNUM) * (OFFSET_FACTOR), (unsigned short)(DATA))

#define IORD_16(BASE, REGNUM) \
  IORD_16DIRECT(BASE, (REGNUM) * (OFFSET_FACTOR))

#define INTEL_LW_UART_RXDATA_REG                 0
#define IOADDR_INTEL_LW_UART_RXDATA(base)        \
        __IO_CALC_ADDRESS_NATIVE(base, INTEL_LW_UART_RXDATA_REG)
#define IORD_INTEL_LW_UART_RXDATA(base)          \
        IORD_16(base, INTEL_LW_UART_RXDATA_REG) 

#define INTEL_LW_UART_TXDATA_REG                 1
#define IOADDR_INTEL_LW_UART_TXDATA(base)        \
        __IO_CALC_ADDRESS_NATIVE(base, INTEL_LW_UART_TXDATA_REG)
#define IOWR_INTEL_LW_UART_TXDATA(base, data)    \
        IOWR_16(base, INTEL_LW_UART_TXDATA_REG, data)

#define INTEL_LW_UART_STATUS_REG                 2
#define IOADDR_INTEL_LW_UART_STATUS(base)        \
        __IO_CALC_ADDRESS_NATIVE(base, INTEL_LW_UART_STATUS_REG)
#define IORD_INTEL_LW_UART_STATUS(base)          \
        IORD_16(base, INTEL_LW_UART_STATUS_REG) 
#define IOWR_INTEL_LW_UART_STATUS(base, data)    \
        IOWR_16(base, INTEL_LW_UART_STATUS_REG, data)

#define INTEL_LW_UART_STATUS_PE_MSK              (0x1)
#define INTEL_LW_UART_STATUS_PE_OFST             (0)
#define INTEL_LW_UART_STATUS_FE_MSK              (0x2)
#define INTEL_LW_UART_STATUS_FE_OFST             (1)
#define INTEL_LW_UART_STATUS_BRK_MSK             (0x4)
#define INTEL_LW_UART_STATUS_BRK_OFST            (2)
#define INTEL_LW_UART_STATUS_ROE_MSK             (0x8)
#define INTEL_LW_UART_STATUS_ROE_OFST            (3)
#define INTEL_LW_UART_STATUS_TOE_MSK             (0x10)
#define INTEL_LW_UART_STATUS_TOE_OFST            (4)
#define INTEL_LW_UART_STATUS_TMT_MSK             (0x20)
#define INTEL_LW_UART_STATUS_TMT_OFST            (5)
#define INTEL_LW_UART_STATUS_TRDY_MSK            (0x40)
#define INTEL_LW_UART_STATUS_TRDY_OFST           (6)
#define INTEL_LW_UART_STATUS_RRDY_MSK            (0x80)
#define INTEL_LW_UART_STATUS_RRDY_OFST           (7)
#define INTEL_LW_UART_STATUS_E_MSK               (0x100)
#define INTEL_LW_UART_STATUS_E_OFST              (8)
#define INTEL_LW_UART_STATUS_DCTS_MSK            (0x400)
#define INTEL_LW_UART_STATUS_DCTS_OFST           (10)
#define INTEL_LW_UART_STATUS_CTS_MSK             (0x800)
#define INTEL_LW_UART_STATUS_CTS_OFST            (11)
#define INTEL_LW_UART_STATUS_EOP_MSK             (0x1000)
#define INTEL_LW_UART_STATUS_EOP_OFST            (12)
#define INTEL_LW_UART_STATUS_RUE_MSK             (0x2000)
#define INTEL_LW_UART_STATUS_RUE_OFST            (13)
#define INTEL_LW_UART_STATUS_RFULL_MSK           (0x4000)
#define INTEL_LW_UART_STATUS_RFULL_OFST          (14)
#define INTEL_LW_UART_STATUS_RAFULL_MSK          (0x8000)
#define INTEL_LW_UART_STATUS_RAFULL_OFST         (15)

#define INTEL_LW_UART_CONTROL_REG                3
#define IOADDR_INTEL_LW_UART_CONTROL(base)       \
        __IO_CALC_ADDRESS_NATIVE(base, INTEL_LW_UART_CONTROL_REG)
#define IORD_INTEL_LW_UART_CONTROL(base)         \
        IORD_16(base, INTEL_LW_UART_CONTROL_REG)
#define IOWR_INTEL_LW_UART_CONTROL(base, data)   \
        IOWR_16(base, INTEL_LW_UART_CONTROL_REG, data)

#define INTEL_LW_UART_CONTROL_PE_MSK             (0x1)
#define INTEL_LW_UART_CONTROL_PE_OFST            (0)
#define INTEL_LW_UART_CONTROL_FE_MSK             (0x2)
#define INTEL_LW_UART_CONTROL_FE_OFST            (1)
#define INTEL_LW_UART_CONTROL_BRK_MSK            (0x4)
#define INTEL_LW_UART_CONTROL_BRK_OFST           (2)
#define INTEL_LW_UART_CONTROL_ROE_MSK            (0x8)
#define INTEL_LW_UART_CONTROL_ROE_OFST           (3)
#define INTEL_LW_UART_CONTROL_TOE_MSK            (0x10)
#define INTEL_LW_UART_CONTROL_TOE_OFST           (4)
#define INTEL_LW_UART_CONTROL_TMT_MSK            (0x20)
#define INTEL_LW_UART_CONTROL_TMT_OFST           (5)
#define INTEL_LW_UART_CONTROL_TRDY_MSK           (0x40)
#define INTEL_LW_UART_CONTROL_TRDY_OFST          (6)
#define INTEL_LW_UART_CONTROL_RRDY_MSK           (0x80)
#define INTEL_LW_UART_CONTROL_RRDY_OFST          (7)
#define INTEL_LW_UART_CONTROL_E_MSK              (0x100)
#define INTEL_LW_UART_CONTROL_E_OFST             (8)
#define INTEL_LW_UART_CONTROL_TRBK_MSK           (0x200)
#define INTEL_LW_UART_CONTROL_TRBK_OFST          (9)
#define INTEL_LW_UART_CONTROL_DCTS_MSK           (0x400)
#define INTEL_LW_UART_CONTROL_DCTS_OFST          (10)
#define INTEL_LW_UART_CONTROL_RTS_MSK            (0x800)
#define INTEL_LW_UART_CONTROL_RTS_OFST           (11)
#define INTEL_LW_UART_CONTROL_EOP_MSK            (0x1000)
#define INTEL_LW_UART_CONTROL_EOP_OFST           (12)
#define INTEL_LW_UART_CONTROL_RUE_MSK            (0x2000)
#define INTEL_LW_UART_CONTROL_RUE_OFST           (13)
#define INTEL_LW_UART_CONTROL_RFULL_MSK          (0x4000)
#define INTEL_LW_UART_CONTROL_RFULL_OFST         (14)
#define INTEL_LW_UART_CONTROL_RAFULL_MSK         (0x8000)
#define INTEL_LW_UART_CONTROL_RAFULL_OFST        (15)

#define INTEL_LW_UART_TRBK_ENABLE                (0x1)
#define INTEL_LW_UART_TRBK_DISABLE               (0)

#define INTEL_LW_UART_DIVISOR_REG                4
#define IOADDR_INTEL_LW_UART_DIVISOR(base)       \
        __IO_CALC_ADDRESS_NATIVE(base, INTEL_LW_UART_DIVISOR_REG)
#define IORD_INTEL_LW_UART_DIVISOR(base)         \
        IORD_16(base, INTEL_LW_UART_DIVISOR_REG) 
#define IOWR_INTEL_LW_UART_DIVISOR(base, data)   \
        IOWR_16(base, INTEL_LW_UART_DIVISOR_REG, data)

#define INTEL_LW_UART_EOP_REG                    5
#define IOADDR_INTEL_LW_UART_EOP(base)           \
        __IO_CALC_ADDRESS_NATIVE(base, INTEL_LW_UART_EOP_REG)
#define IORD_INTEL_LW_UART_EOP(base)             \
        IORD_16(base, INTEL_LW_UART_EOP_REG)
#define IOWR_INTEL_LW_UART_EOP(base, data)       \
        IOWR_16(base, INTEL_LW_UART_EOP_REG, data)

#define INTEL_LW_UART_EOP_MSK                    (0xFF)
#define INTEL_LW_UART_EOP_OFST                   (0)

#define INTEL_LW_UART_RXFIFO_LVL_REG             6
#define IOADDR_INTEL_LW_UART_RXFIFO_LVL(base)    \
        __IO_CALC_ADDRESS_NATIVE(base, INTEL_LW_UART_RXFIFO_LVL_REG)
#define IORD_INTEL_LW_UART_RXFIFO_LVL(base)      \
        IORD_16(base, INTEL_LW_UART_RXFIFO_LVL_REG)
        
#define INTEL_LW_UART_TXFIFO_LVL_REG             7
#define IOADDR_INTEL_LW_UART_TXFIFO_LVL(base)    \
        __IO_CALC_ADDRESS_NATIVE(base, INTEL_LW_UART_TXFIFO_LVL_REG)
#define IORD_INTEL_LW_UART_TXFIFO_LVL(base)      \
        IORD_16(base, INTEL_LW_UART_TXFIFO_LVL_REG)

#endif /* __INTEL_LW_UART_REGS_H__ */
