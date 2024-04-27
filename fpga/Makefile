# FPGA settings
FPGA_PART = xcvc1902-vsva2197-2MP-e-S
FPGA_BOARD = xilinx.com:vck190:part0:3.2
FPGA_TOP  = vck190-platform
FPGA_ARCH = versal

# Files for synthesis
SYN_FILES  = src/rtl/jesd_clock.v
SYN_FILES += src/rtl/rx_mapper.v
SYN_FILES += src/rtl/tx_mapper.v
SYN_FILES += src/rtl/loop_ctrl.v
SYN_FILES += src/rtl/adc_to_axistream.v
SYN_FILES += src/rtl/inOutAct.v
SYN_FILES += src/rtl/RVTDNNTop.v

# XDC files
XDC_FILES = src/xdc/top.xdc

# IP
IP_TCL_FILES = src/ip/ip.tcl

# BD
BD_TCL_FILES = src/bd/design_1.tcl

# Defines
DEFS = VERSAL
DEFS_DICT := ARCH="VERSAL"

include ../scripts/vivado.mk

SW_DIR := src/sw
C_SRC_FILE := $(wildcard $(SW_DIR)/*.c)
C_HDR_FILE := $(wildcard $(SW_DIR)/*.h)

include ../scripts/vitis.mk

program: $(FPGA_TOP).bit
	echo "open_hw_manager" > program.tcl
	echo "connect_hw_server" >> program.tcl
	echo "open_hw_target" >> program.tcl
	echo "current_hw_device [lindex [get_hw_devices] 0]" >> program.tcl
	echo "refresh_hw_device -update_hw_probes false [current_hw_device]" >> program.tcl
	echo "set_property PROGRAM.FILE {$(FPGA_TOP).bit} [current_hw_device]" >> program.tcl
	echo "program_hw_devices [current_hw_device]" >> program.tcl
	echo "exit" >> program.tcl
	vivado -nojournal -nolog -mode batch -source program.tcl
