/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'niosv_m' in SOPC Builder design 'niosv_system'
 *
 * Generated: Mon Nov 17 16:53:55 EST 2025
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "intel_niosv_m"
#define ALT_CPU_CPU_FREQ 120000000u
#define ALT_CPU_DATA_ADDR_WIDTH 0x20
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_FREQ 120000000
#define ALT_CPU_HAS_CSR_SUPPORT 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_ICACHE_LINE_SIZE 0
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_ICACHE_SIZE 0
#define ALT_CPU_INST_ADDR_WIDTH 0x20
#define ALT_CPU_INT_MODE 0
#define ALT_CPU_MTIME_OFFSET 0x00050000
#define ALT_CPU_NAME "niosv_m"
#define ALT_CPU_NIOSV_CORE_VARIANT 1
#define ALT_CPU_NUM_GPR 32
#define ALT_CPU_RESET_ADDR 0x00000000
#define ALT_CPU_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define ALT_CPU_TIMER_DEVICE_TYPE 2


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define ABBOTTSLAKE_CPU_FREQ 120000000u
#define ABBOTTSLAKE_DATA_ADDR_WIDTH 0x20
#define ABBOTTSLAKE_DCACHE_LINE_SIZE 0
#define ABBOTTSLAKE_DCACHE_LINE_SIZE_LOG2 0
#define ABBOTTSLAKE_DCACHE_SIZE 0
#define ABBOTTSLAKE_HAS_CSR_SUPPORT 1
#define ABBOTTSLAKE_HAS_DEBUG_STUB
#define ABBOTTSLAKE_ICACHE_LINE_SIZE 0
#define ABBOTTSLAKE_ICACHE_LINE_SIZE_LOG2 0
#define ABBOTTSLAKE_ICACHE_SIZE 0
#define ABBOTTSLAKE_INST_ADDR_WIDTH 0x20
#define ABBOTTSLAKE_INT_MODE 0
#define ABBOTTSLAKE_MTIME_OFFSET 0x00050000
#define ABBOTTSLAKE_NIOSV_CORE_VARIANT 1
#define ABBOTTSLAKE_NUM_GPR 32
#define ABBOTTSLAKE_RESET_ADDR 0x00000000
#define ABBOTTSLAKE_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define ABBOTTSLAKE_TIMER_DEVICE_TYPE 2


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_I2C
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __INTEL_LW_UART
#define __INTEL_MIPI_CSI2
#define __INTEL_NIOSV_M
#define __INTEL_ONCHIP_MEMORY
#define __INTEL_VVP_DEMOSAIC
#define __INTEL_VVP_SCALER
#define __INTEL_VVP_VFW
#define __INTEL_VVP_WBC
#define __MIPI_DPHY
#define __ONE_AI_INTERFACE


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "AGILEX5"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/lw_uart"
#define ALT_STDERR_BASE 0x24040
#define ALT_STDERR_DEV lw_uart
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "intel_lw_uart"
#define ALT_STDIN "/dev/lw_uart"
#define ALT_STDIN_BASE 0x24040
#define ALT_STDIN_DEV lw_uart
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "intel_lw_uart"
#define ALT_STDOUT "/dev/lw_uart"
#define ALT_STDOUT_BASE 0x24040
#define ALT_STDOUT_DEV lw_uart
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "intel_lw_uart"
#define ALT_SYSID_BASE SYS_ID_BASE
#define ALT_SYSID_ID SYS_ID_ID
#define ALT_SYSTEM_NAME "niosv_system"
#define ALT_SYS_CLK_TICKS_PER_SEC ALT_CPU_TICKS_PER_SEC
#define ALT_TIMESTAMP_CLK_TIMER_DEVICE_TYPE ALT_CPU_TIMER_DEVICE_TYPE


/*
 * camera_i2c configuration
 *
 */

#define ALT_MODULE_CLASS_camera_i2c altera_avalon_i2c
#define CAMERA_I2C_BASE 0x24000
#define CAMERA_I2C_FIFO_DEPTH 4
#define CAMERA_I2C_FREQ 120000000
#define CAMERA_I2C_IRQ 2
#define CAMERA_I2C_IRQ_INTERRUPT_CONTROLLER_ID 0
#define CAMERA_I2C_NAME "/dev/camera_i2c"
#define CAMERA_I2C_SPAN 64
#define CAMERA_I2C_TYPE "altera_avalon_i2c"
#define CAMERA_I2C_USE_AV_ST 0


/*
 * hal2 configuration
 *
 */

#define ALT_MAX_FD 32
#define ALT_SYS_CLK NIOSV_M
#define ALT_TIMESTAMP_CLK NIOSV_M
#define INTEL_FPGA_DFL_START_ADDRESS 0xffffffffffffffff
#define INTEL_FPGA_USE_DFL_WALKER 0


/*
 * intel_niosv_m_hal_driver configuration
 *
 */

#define NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND 1000


/*
 * lw_uart configuration
 *
 */

#define ALT_MODULE_CLASS_lw_uart intel_lw_uart
#define LW_UART_BASE 0x24040
#define LW_UART_BAUD 115200
#define LW_UART_DATA_BITS 8
#define LW_UART_FIXED_BAUD 1
#define LW_UART_FREQ 120000000
#define LW_UART_IRQ 3
#define LW_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define LW_UART_NAME "/dev/lw_uart"
#define LW_UART_PARITY 'N'
#define LW_UART_READ_DEPTH 2048
#define LW_UART_SIM_TRUE_BAUD 0
#define LW_UART_SPAN 32
#define LW_UART_STOP_BITS 1
#define LW_UART_SYNC_REG_DEPTH 2
#define LW_UART_TYPE "intel_lw_uart"
#define LW_UART_USE_CTS_RTS 0
#define LW_UART_USE_EOP_REGISTER 0
#define LW_UART_WRITE_DEPTH 2048


/*
 * mipi_pio configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_pio altera_avalon_pio
#define MIPI_PIO_BASE 0x24070
#define MIPI_PIO_BIT_CLEARING_EDGE_REGISTER 0
#define MIPI_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define MIPI_PIO_CAPTURE 0
#define MIPI_PIO_DATA_WIDTH 1
#define MIPI_PIO_DO_TEST_BENCH_WIRING 0
#define MIPI_PIO_DRIVEN_SIM_VALUE 0
#define MIPI_PIO_EDGE_TYPE "NONE"
#define MIPI_PIO_FREQ 120000000
#define MIPI_PIO_HAS_IN 0
#define MIPI_PIO_HAS_OUT 1
#define MIPI_PIO_HAS_TRI 0
#define MIPI_PIO_IRQ -1
#define MIPI_PIO_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_PIO_IRQ_TYPE "NONE"
#define MIPI_PIO_NAME "/dev/mipi_pio"
#define MIPI_PIO_RESET_VALUE 1
#define MIPI_PIO_SPAN 16
#define MIPI_PIO_TYPE "altera_avalon_pio"


/*
 * mipi_system_0_mipi_csi2 configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_system_0_mipi_csi2 intel_mipi_csi2
#define MIPI_SYSTEM_0_MIPI_CSI2_BASE 0x20000
#define MIPI_SYSTEM_0_MIPI_CSI2_IRQ -1
#define MIPI_SYSTEM_0_MIPI_CSI2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_SYSTEM_0_MIPI_CSI2_NAME "/dev/mipi_system_0_mipi_csi2"
#define MIPI_SYSTEM_0_MIPI_CSI2_SPAN 4096
#define MIPI_SYSTEM_0_MIPI_CSI2_TYPE "intel_mipi_csi2"


/*
 * mipi_system_0_mipi_dphy configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_system_0_mipi_dphy mipi_dphy
#define MIPI_SYSTEM_0_MIPI_DPHY_BASE 0x21000
#define MIPI_SYSTEM_0_MIPI_DPHY_IRQ -1
#define MIPI_SYSTEM_0_MIPI_DPHY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_SYSTEM_0_MIPI_DPHY_NAME "/dev/mipi_system_0_mipi_dphy"
#define MIPI_SYSTEM_0_MIPI_DPHY_SPAN 4096
#define MIPI_SYSTEM_0_MIPI_DPHY_TYPE "mipi_dphy"


/*
 * mipi_system_0_vvp_demosaic configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_system_0_vvp_demosaic intel_vvp_demosaic
#define MIPI_SYSTEM_0_VVP_DEMOSAIC_BASE 0x22400
#define MIPI_SYSTEM_0_VVP_DEMOSAIC_IRQ -1
#define MIPI_SYSTEM_0_VVP_DEMOSAIC_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_SYSTEM_0_VVP_DEMOSAIC_NAME "/dev/mipi_system_0_vvp_demosaic"
#define MIPI_SYSTEM_0_VVP_DEMOSAIC_SPAN 512
#define MIPI_SYSTEM_0_VVP_DEMOSAIC_TYPE "intel_vvp_demosaic"


/*
 * mipi_system_0_vvp_scaler configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_system_0_vvp_scaler intel_vvp_scaler
#define MIPI_SYSTEM_0_VVP_SCALER_BASE 0x22200
#define MIPI_SYSTEM_0_VVP_SCALER_IRQ -1
#define MIPI_SYSTEM_0_VVP_SCALER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_SYSTEM_0_VVP_SCALER_NAME "/dev/mipi_system_0_vvp_scaler"
#define MIPI_SYSTEM_0_VVP_SCALER_SPAN 512
#define MIPI_SYSTEM_0_VVP_SCALER_TYPE "intel_vvp_scaler"


/*
 * mipi_system_0_vvp_vfw configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_system_0_vvp_vfw intel_vvp_vfw
#define MIPI_SYSTEM_0_VVP_VFW_BASE 0x22000
#define MIPI_SYSTEM_0_VVP_VFW_IRQ 1
#define MIPI_SYSTEM_0_VVP_VFW_IRQ_INTERRUPT_CONTROLLER_ID 0
#define MIPI_SYSTEM_0_VVP_VFW_NAME "/dev/mipi_system_0_vvp_vfw"
#define MIPI_SYSTEM_0_VVP_VFW_SPAN 512
#define MIPI_SYSTEM_0_VVP_VFW_TYPE "intel_vvp_vfw"


/*
 * mipi_system_0_vvp_wbc configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_system_0_vvp_wbc intel_vvp_wbc
#define MIPI_SYSTEM_0_VVP_WBC_BASE 0x22600
#define MIPI_SYSTEM_0_VVP_WBC_IRQ -1
#define MIPI_SYSTEM_0_VVP_WBC_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_SYSTEM_0_VVP_WBC_NAME "/dev/mipi_system_0_vvp_wbc"
#define MIPI_SYSTEM_0_VVP_WBC_SPAN 512
#define MIPI_SYSTEM_0_VVP_WBC_TYPE "intel_vvp_wbc"


/*
 * niosv_m_dm_agent configuration
 *
 */

#define ALT_MODULE_CLASS_niosv_m_dm_agent intel_niosv_m
#define NIOSV_M_DM_AGENT_BASE 0x60000
#define NIOSV_M_DM_AGENT_CPU_FREQ 120000000u
#define NIOSV_M_DM_AGENT_DATA_ADDR_WIDTH 0x20
#define NIOSV_M_DM_AGENT_DCACHE_LINE_SIZE 0
#define NIOSV_M_DM_AGENT_DCACHE_LINE_SIZE_LOG2 0
#define NIOSV_M_DM_AGENT_DCACHE_SIZE 0
#define NIOSV_M_DM_AGENT_HAS_CSR_SUPPORT 1
#define NIOSV_M_DM_AGENT_HAS_DEBUG_STUB
#define NIOSV_M_DM_AGENT_ICACHE_LINE_SIZE 0
#define NIOSV_M_DM_AGENT_ICACHE_LINE_SIZE_LOG2 0
#define NIOSV_M_DM_AGENT_ICACHE_SIZE 0
#define NIOSV_M_DM_AGENT_INST_ADDR_WIDTH 0x20
#define NIOSV_M_DM_AGENT_INT_MODE 0
#define NIOSV_M_DM_AGENT_IRQ -1
#define NIOSV_M_DM_AGENT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define NIOSV_M_DM_AGENT_MTIME_OFFSET 0x00050000
#define NIOSV_M_DM_AGENT_NAME "/dev/niosv_m_dm_agent"
#define NIOSV_M_DM_AGENT_NIOSV_CORE_VARIANT 1
#define NIOSV_M_DM_AGENT_NUM_GPR 32
#define NIOSV_M_DM_AGENT_RESET_ADDR 0x00000000
#define NIOSV_M_DM_AGENT_SPAN 65536
#define NIOSV_M_DM_AGENT_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define NIOSV_M_DM_AGENT_TIMER_DEVICE_TYPE 2
#define NIOSV_M_DM_AGENT_TYPE "intel_niosv_m"


/*
 * niosv_m_timer_sw_agent configuration
 *
 */

#define ALT_MODULE_CLASS_niosv_m_timer_sw_agent intel_niosv_m
#define NIOSV_M_TIMER_SW_AGENT_BASE 0x50000
#define NIOSV_M_TIMER_SW_AGENT_CPU_FREQ 120000000u
#define NIOSV_M_TIMER_SW_AGENT_DATA_ADDR_WIDTH 0x20
#define NIOSV_M_TIMER_SW_AGENT_DCACHE_LINE_SIZE 0
#define NIOSV_M_TIMER_SW_AGENT_DCACHE_LINE_SIZE_LOG2 0
#define NIOSV_M_TIMER_SW_AGENT_DCACHE_SIZE 0
#define NIOSV_M_TIMER_SW_AGENT_HAS_CSR_SUPPORT 1
#define NIOSV_M_TIMER_SW_AGENT_HAS_DEBUG_STUB
#define NIOSV_M_TIMER_SW_AGENT_ICACHE_LINE_SIZE 0
#define NIOSV_M_TIMER_SW_AGENT_ICACHE_LINE_SIZE_LOG2 0
#define NIOSV_M_TIMER_SW_AGENT_ICACHE_SIZE 0
#define NIOSV_M_TIMER_SW_AGENT_INST_ADDR_WIDTH 0x20
#define NIOSV_M_TIMER_SW_AGENT_INT_MODE 0
#define NIOSV_M_TIMER_SW_AGENT_IRQ -1
#define NIOSV_M_TIMER_SW_AGENT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define NIOSV_M_TIMER_SW_AGENT_MTIME_OFFSET 0x00050000
#define NIOSV_M_TIMER_SW_AGENT_NAME "/dev/niosv_m_timer_sw_agent"
#define NIOSV_M_TIMER_SW_AGENT_NIOSV_CORE_VARIANT 1
#define NIOSV_M_TIMER_SW_AGENT_NUM_GPR 32
#define NIOSV_M_TIMER_SW_AGENT_RESET_ADDR 0x00000000
#define NIOSV_M_TIMER_SW_AGENT_SPAN 64
#define NIOSV_M_TIMER_SW_AGENT_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define NIOSV_M_TIMER_SW_AGENT_TIMER_DEVICE_TYPE 2
#define NIOSV_M_TIMER_SW_AGENT_TYPE "intel_niosv_m"


/*
 * onchip_memory configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory intel_onchip_memory
#define ONCHIP_MEMORY_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY_BASE 0x0
#define ONCHIP_MEMORY_CONTENTS_INFO ""
#define ONCHIP_MEMORY_DUAL_PORT 0
#define ONCHIP_MEMORY_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_INIT_CONTENTS_FILE "ocm"
#define ONCHIP_MEMORY_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY_IRQ -1
#define ONCHIP_MEMORY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY_NAME "/dev/onchip_memory"
#define ONCHIP_MEMORY_NON_DEFAULT_INIT_FILE_ENABLED 1
#define ONCHIP_MEMORY_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY_SIZE_VALUE 65536
#define ONCHIP_MEMORY_SPAN 65536
#define ONCHIP_MEMORY_TYPE "intel_onchip_memory"
#define ONCHIP_MEMORY_WRITABLE 1


/*
 * one_ai_interface_0 configuration
 *
 */

#define ALT_MODULE_CLASS_one_ai_interface_0 one_ai_interface
#define ONE_AI_INTERFACE_0_BASE 0x70000
#define ONE_AI_INTERFACE_0_IRQ -1
#define ONE_AI_INTERFACE_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONE_AI_INTERFACE_0_NAME "/dev/one_ai_interface_0"
#define ONE_AI_INTERFACE_0_SPAN 1024
#define ONE_AI_INTERFACE_0_TYPE "one_ai_interface"


/*
 * rgb_pio configuration
 *
 */

#define ALT_MODULE_CLASS_rgb_pio altera_avalon_pio
#define RGB_PIO_BASE 0x24060
#define RGB_PIO_BIT_CLEARING_EDGE_REGISTER 0
#define RGB_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define RGB_PIO_CAPTURE 0
#define RGB_PIO_DATA_WIDTH 3
#define RGB_PIO_DO_TEST_BENCH_WIRING 0
#define RGB_PIO_DRIVEN_SIM_VALUE 0
#define RGB_PIO_EDGE_TYPE "NONE"
#define RGB_PIO_FREQ 120000000
#define RGB_PIO_HAS_IN 0
#define RGB_PIO_HAS_OUT 1
#define RGB_PIO_HAS_TRI 0
#define RGB_PIO_IRQ -1
#define RGB_PIO_IRQ_INTERRUPT_CONTROLLER_ID -1
#define RGB_PIO_IRQ_TYPE "NONE"
#define RGB_PIO_NAME "/dev/rgb_pio"
#define RGB_PIO_RESET_VALUE 7
#define RGB_PIO_SPAN 16
#define RGB_PIO_TYPE "altera_avalon_pio"


/*
 * sys_id configuration
 *
 */

#define ALT_MODULE_CLASS_sys_id altera_avalon_sysid_qsys
#define SYS_ID_BASE 0x24080
#define SYS_ID_ID 35661072
#define SYS_ID_IRQ -1
#define SYS_ID_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYS_ID_NAME "/dev/sys_id"
#define SYS_ID_SPAN 8
#define SYS_ID_TYPE "altera_avalon_sysid_qsys"

#endif /* __SYSTEM_H_ */
