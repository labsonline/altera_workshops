# Copyright (C) 2025  Altera Corporation. All rights reserved.
# Your use of Altera Corporation's design tools, logic functions 
# and other software and tools, and any partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Altera Program License 
# Subscription Agreement, the Altera Quartus Prime License Agreement,
# the Altera IP License Agreement, or other applicable license
# agreement, including, without limitation, that your use is for
# the sole purpose of programming logic devices manufactured by
# Altera and sold by Altera or its authorized distributors.  Please
# refer to the Altera Software License Subscription Agreements 
# on the Quartus Prime software download page.

# Quartus Prime: Generate Tcl File for Project
# File: AXC3000_MIPI.tcl
# Generated on: Mon Nov 17 19:48:15 2025

# Load Quartus Prime Tcl Project package
package require ::quartus::project

set need_to_close_project 0
set make_assignments 1

# Check that the right project is open
if {[is_project_open]} {
	if {[string compare $quartus(project) "AXC3000_MIPI"]} {
		puts "Project AXC3000_MIPI is not open"
		set make_assignments 0
	}
} else {
	# Only open if not already open
	if {[project_exists AXC3000_MIPI]} {
		project_open -revision AXC3000_MIPI AXC3000_MIPI
	} else {
		project_new -revision AXC3000_MIPI AXC3000_MIPI
	}
	set need_to_close_project 1
}

# Make assignments
if {$make_assignments} {
	set_global_assignment -name DEVICE A5EB013BB23BE4SCS
	set_global_assignment -name TOP_LEVEL_ENTITY AXC3000_MIPI
	set_global_assignment -name SDC_FILE ../sources/AXC3000_MIPI.sdc
	set_global_assignment -name VERILOG_FILE ../sources/AXC3000_MIPI.v
	set_global_assignment -name QSYS_FILE niosv_system.qsys
	set_global_assignment -name IP_FILE ip/niosv_system/reset_release.ip
	set_global_assignment -name IP_FILE ip/niosv_system/niosv_m.ip
	set_global_assignment -name IP_FILE ip/niosv_system/onchip_memory.ip
	set_global_assignment -name IP_FILE ip/niosv_system/jtag_master.ip
	set_global_assignment -name IP_FILE ip/niosv_system/sys_id.ip
	set_global_assignment -name IP_FILE ip/niosv_system/reset_bridge.ip
	set_global_assignment -name IP_FILE ip/niosv_system/mipi_pio.ip
	set_global_assignment -name IP_FILE ip/niosv_system/rgb_pio.ip
	set_global_assignment -name QSYS_FILE mipi_system.qsys
	set_global_assignment -name IP_FILE ip/mipi_system/vvp_scaler.ip
	set_global_assignment -name IP_FILE ip/mipi_system/vid_mm_bridge.ip
	set_global_assignment -name IP_FILE ip/mipi_system/mipi_system_reset_in.ip
	set_global_assignment -name IP_FILE ip/mipi_system/mipi_dphy.ip
	set_global_assignment -name IP_FILE ip/mipi_system/vvp_demosaic.ip
	set_global_assignment -name IP_FILE ip/mipi_system/vvp_vfw.ip
	set_global_assignment -name IP_FILE ip/mipi_system/vvp_wbc.ip
	set_global_assignment -name IP_FILE ip/mipi_system/mipi_csi2.ip
	set_global_assignment -name IP_FILE ip/mipi_system/mipi_system_clock_in.ip
	set_global_assignment -name IP_FILE ip/niosv_system/onchip_vid_buf.ip
	set_global_assignment -name IP_FILE ip/niosv_system/iopll.ip
	set_global_assignment -name IP_FILE ip/niosv_system/camera_i2c.ip
	set_global_assignment -name IP_FILE ip/niosv_system/sysclk_bridge_o.ip
	set_global_assignment -name IP_FILE ip/niosv_system/lw_uart.ip
	set_global_assignment -name IP_FILE ip/niosv_system/image_source_0.ip
	set_global_assignment -name IP_FILE ip/niosv_system/one_ai_interface_0.ip
	set_global_assignment -name MIN_CORE_JUNCTION_TEMP 0
	set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
	set_global_assignment -name FAMILY "Agilex 5"
	set_global_assignment -name ENABLE_ED_CRC_CHECK OFF
	set_global_assignment -name MINIMUM_SEU_INTERVAL 1313422124
	set_global_assignment -name ENABLE_CRAM_INTEGRITY_CHECK ON
	set_global_assignment -name USE_CONF_DONE SDM_IO16
	set_global_assignment -name USE_INIT_DONE SDM_IO0
	set_global_assignment -name POWER_APPLY_THERMAL_MARGIN ADDITIONAL
	set_global_assignment -name PWRMGT_VOLTAGE_OUTPUT_FORMAT "LINEAR FORMAT"
	set_global_assignment -name PWRMGT_LINEAR_FORMAT_N "-12"
	set_global_assignment -name LAST_QUARTUS_VERSION "25.3.0 Pro Edition"
	set_global_assignment -name BOARD default
	set_global_assignment -name IP_FILE ip/niosv_system/niosv_system_clock_in.ip
	set_instance_assignment -name WEAK_PULL_UP_RESISTOR ON -to USER_BTN -entity AXC3000_MIPI
	set_location_assignment PIN_C8 -to USER_BTN
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to USER_BTN -entity AXC3000_MIPI
	set_location_assignment PIN_BR19 -to UART_RXD
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to UART_RXD -entity AXC3000_MIPI
	set_location_assignment PIN_CJ1 -to UART_TXD
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to UART_TXD -entity AXC3000_MIPI
	set_location_assignment PIN_DF35 -to RLED
	set_instance_assignment -name IO_STANDARD "1.1-V" -to RLED -entity AXC3000_MIPI
	set_location_assignment PIN_DJ32 -to GLED
	set_instance_assignment -name IO_STANDARD "1.1-V" -to GLED -entity AXC3000_MIPI
	set_location_assignment PIN_DN22 -to BLED
	set_instance_assignment -name IO_STANDARD "1.1-V" -to BLED -entity AXC3000_MIPI
	set_location_assignment PIN_DP23 -to LED1
	set_instance_assignment -name IO_STANDARD "1.1-V" -to LED1 -entity AXC3000_MIPI
	set_location_assignment PIN_DP38 -to MIPI_D1P
	set_instance_assignment -name IO_STANDARD DPHY -to MIPI_D1P -entity AXC3000_MIPI
	set_location_assignment PIN_DP36 -to MIPI_D0P
	set_instance_assignment -name IO_STANDARD DPHY -to MIPI_D0P -entity AXC3000_MIPI
	set_location_assignment PIN_DP33 -to MIPI_CLKP
	set_instance_assignment -name IO_STANDARD DPHY -to MIPI_CLKP -entity AXC3000_MIPI
	set_location_assignment PIN_DN38 -to MIPI_D1N
	set_instance_assignment -name IO_STANDARD DPHY -to MIPI_D1N -entity AXC3000_MIPI
	set_location_assignment PIN_DN33 -to MIPI_D0N
	set_instance_assignment -name IO_STANDARD DPHY -to MIPI_D0N -entity AXC3000_MIPI
	set_location_assignment PIN_DN30 -to MIPI_CLKN
	set_instance_assignment -name IO_STANDARD DPHY -to MIPI_CLKN -entity AXC3000_MIPI
	set_location_assignment PIN_DD35 -to MIPI_RZQ
	set_instance_assignment -name IO_STANDARD "1.1-V" -to MIPI_RZQ -entity AXC3000_MIPI
	set_location_assignment PIN_BF23 -to MIPI_REFCLK
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to MIPI_REFCLK -entity AXC3000_MIPI
	set_location_assignment PIN_BP1 -to CAMERA_SDA
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to CAMERA_SDA -entity AXC3000_MIPI
	set_location_assignment PIN_BP2 -to CAMERA_SCL
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to CAMERA_SCL -entity AXC3000_MIPI
	set_location_assignment PIN_BR8 -to CAMERA_EN
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to CAMERA_EN -entity AXC3000_MIPI
	set_location_assignment PIN_V16 -to CLK_50M_C
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to CLK_50M_C -entity AXC3000_MIPI

	# Including default assignments
	set_global_assignment -name FLOW_ENABLE_DESIGN_ASSISTANT ON -family "Agilex 5"
	set_global_assignment -name TIMING_ANALYZER_MULTICORNER_ANALYSIS ON -family "Agilex 5"
	set_global_assignment -name TDC_CCPP_TRADEOFF_TOLERANCE 0 -family "Agilex 5"
	set_global_assignment -name TIMING_ANALYZER_DO_CCPP_REMOVAL ON -family "Agilex 5"
	set_global_assignment -name AUTO_OPEN_DRAIN_PINS OFF -family "Agilex 5"
	set_global_assignment -name PHYSICAL_SHIFT_REGISTER_INFERENCE ON -family "Agilex 5"
	set_global_assignment -name MAXIMUM_SYNCHRONIZER_LENGTH_PROTECTED 3 -family "Agilex 5"
	set_global_assignment -name SYNTH_RESOURCE_AWARE_INFERENCE_FOR_BLOCK_RAM ON -family "Agilex 5"
	set_global_assignment -name ADVANCED_PHYSICAL_SYNTHESIS_REGISTER_PACKING ON -family "Agilex 5"
	set_global_assignment -name PHYSICAL_SYNTHESIS ON -family "Agilex 5"
	set_global_assignment -name POST_ROUTE_PHYSICAL_SYNTHESIS OFF -family "Agilex 5"
	set_global_assignment -name STRATIXV_CONFIGURATION_SCHEME "ACTIVE SERIAL X4" -family "Agilex 5"
	set_global_assignment -name PRESERVE_UNUSED_XCVR_CHANNEL ON -family "Agilex 5"
	set_global_assignment -name OPTIMIZE_HOLD_TIMING "ALL PATHS" -family "Agilex 5"
	set_global_assignment -name OPTIMIZE_MULTI_CORNER_TIMING ON -family "Agilex 5"
	set_global_assignment -name ENABLE_PHYSICAL_DSP_MERGING ON -family "Agilex 5"
	set_global_assignment -name AUTO_DELAY_CHAINS ON -family "Agilex 5"
	set_global_assignment -name ALLOW_SEU_FAULT_INJECTION OFF -family "Agilex 5"
	set_global_assignment -name FITTER_RESYNTHESIS ON -family "Agilex 5"
	set_global_assignment -name FITTER_EARLY_RETIMING ON -family "Agilex 5"
	set_global_assignment -name FLOW_ENABLE_HYPER_RETIMER_FAST_FORWARD OFF -family "Agilex 5"
	set_global_assignment -name HYPER_RETIMER_FAST_FORWARD_ON_HIERARCHY ON -family "Agilex 5"
	set_global_assignment -name GENERATE_PR_RBF_FILE ON -family "Agilex 5"
	set_global_assignment -name HPS_INITIALIZATION "AFTER INIT_DONE" -family "Agilex 5"
	set_global_assignment -name POWER_USE_DEVICE_CHARACTERISTICS MAXIMUM -family "Agilex 5"
	set_global_assignment -name ACTIVE_SERIAL_CLOCK AS_FREQ_100MHZ -family "Agilex 3"
	set_global_assignment -name ACTIVE_SERIAL_CLOCK AS_FREQ_100MHZ -family "Agilex 5"
	set_global_assignment -name DEVICE_INITIALIZATION_CLOCK INIT_INTOSC -family "Agilex 5"
	set_global_assignment -name PROGRAMMING_BITSTREAM_ENCRYPTION_KEY_SELECT "Battery Backup RAM" -family "Agilex 5"
	set_global_assignment -name EDA_IBIS_MUTUAL_COUPLING ON -section_id eda_board_design_signal_integrity -family "Agilex 5"
	set_global_assignment -name EDA_IBIS_SPECIFICATION_VERSION 5P0 -section_id eda_board_design_signal_integrity -family "Agilex 5"

	# Commit assignments
	export_assignments

	# Close project
	if {$need_to_close_project} {
		project_close
	}
}
