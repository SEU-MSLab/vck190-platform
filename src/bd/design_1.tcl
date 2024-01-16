
################################################################
# This is a generated script based on design: design_1
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2023.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_gid_msg -ssname BD::TCL -id 2041 -severity "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source design_1_script.tcl


# The design that will be created by this Tcl script contains the following 
# module references:
# jesd_clock, jesd_clock, rx_mapper, tx_mapper, inOutAct, RVTDNNTop, loop_ctrl, adc_to_axistream

# Please add the sources of those modules before sourcing this Tcl script.

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xcvc1902-vsva2197-2MP-e-S
   set_property BOARD_PART xilinx.com:vck190:part0:2.2 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name design_1

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_gid_msg -ssname BD::TCL -id 2001 -severity "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_gid_msg -ssname BD::TCL -id 2002 -severity "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_gid_msg -ssname BD::TCL -id 2003 -severity "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_gid_msg -ssname BD::TCL -id 2004 -severity "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_gid_msg -ssname BD::TCL -id 2005 -severity "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_gid_msg -ssname BD::TCL -id 2006 -severity "ERROR" $errMsg}
   return $nRet
}

set bCheckIPsPassed 1
##################################################################
# CHECK IPs
##################################################################
set bCheckIPs 1
if { $bCheckIPs == 1 } {
   set list_check_ips "\ 
xilinx.com:ip:proc_sys_reset:5.0\
xilinx.com:ip:axi_bram_ctrl:4.1\
xilinx.com:ip:axi_dma:7.1\
xilinx.com:ip:axi_gpio:2.0\
xilinx.com:ip:axis_ila:1.2\
xilinx.com:ip:axis_vio:1.0\
xilinx.com:ip:jesd204c:4.2\
xilinx.com:ip:smartconnect:1.0\
xilinx.com:ip:util_ds_buf:2.2\
xilinx.com:ip:versal_cips:3.3\
xilinx.com:ip:xlconstant:1.1\
xilinx.com:ip:axi_noc:1.0\
xilinx.com:ip:util_reduced_logic:2.0\
xilinx.com:ip:xlconcat:2.1\
xilinx.com:ip:gt_quad_base:1.1\
xilinx.com:ip:xlslice:1.0\
xilinx.com:ip:axis_data_fifo:2.0\
"

   set list_ips_missing ""
   common::send_gid_msg -ssname BD::TCL -id 2011 -severity "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2012 -severity "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

}

##################################################################
# CHECK Modules
##################################################################
set bCheckModules 1
if { $bCheckModules == 1 } {
   set list_check_mods "\ 
jesd_clock\
jesd_clock\
rx_mapper\
tx_mapper\
inOutAct\
RVTDNNTop\
loop_ctrl\
adc_to_axistream\
"

   set list_mods_missing ""
   common::send_gid_msg -ssname BD::TCL -id 2020 -severity "INFO" "Checking if the following modules exist in the project's sources: $list_check_mods ."

   foreach mod_vlnv $list_check_mods {
      if { [can_resolve_reference $mod_vlnv] == 0 } {
         lappend list_mods_missing $mod_vlnv
      }
   }

   if { $list_mods_missing ne "" } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2021 -severity "ERROR" "The following module(s) are not found in the project: $list_mods_missing" }
      common::send_gid_msg -ssname BD::TCL -id 2022 -severity "INFO" "Please add source files for the missing module(s) above."
      set bCheckIPsPassed 0
   }
}

if { $bCheckIPsPassed != 1 } {
  common::send_gid_msg -ssname BD::TCL -id 2023 -severity "WARNING" "Will not continue with creation of design due to the error(s) above."
  return 3
}

##################################################################
# DESIGN PROCs
##################################################################


# Hierarchical cell: BRAM_FIFO
proc create_hier_cell_BRAM_FIFO { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_BRAM_FIFO() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins

  # Create pins
  create_bd_pin -dir I -type rst s_axis_aresetn
  create_bd_pin -dir I -type clk s_axis_aclk
  create_bd_pin -dir I s_axis_tvalid
  create_bd_pin -dir I -type clk m_axis_aclk
  create_bd_pin -dir O -from 0 -to 0 -type data BRAM_enable
  create_bd_pin -dir O -from 15 -to 0 -type data BRAM_data
  create_bd_pin -dir O -from 9 -to 0 BRAM_addr
  create_bd_pin -dir I -from 15 -to 0 In_addr
  create_bd_pin -dir I -from 31 -to 0 In_data
  create_bd_pin -dir I -from 15 -to 0 -type data rddata
  create_bd_pin -dir I -from 0 -to 0 -type data load

  # Create instance: xlslice_0, and set properties
  set xlslice_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_0 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {31} \
    CONFIG.DIN_TO {16} \
    CONFIG.DIN_WIDTH {48} \
    CONFIG.DOUT_WIDTH {16} \
  ] $xlslice_0


  # Create instance: xlconcat_0, and set properties
  set xlconcat_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_0 ]
  set_property -dict [list \
    CONFIG.IN0_WIDTH {16} \
    CONFIG.IN1_WIDTH {32} \
  ] $xlconcat_0


  # Create instance: xlslice_1, and set properties
  set xlslice_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_1 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {11} \
    CONFIG.DIN_TO {2} \
    CONFIG.DIN_WIDTH {48} \
    CONFIG.DOUT_WIDTH {10} \
  ] $xlslice_1


  # Create instance: axis_data_fifo_0, and set properties
  set axis_data_fifo_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_0 ]
  set_property -dict [list \
    CONFIG.IS_ACLK_ASYNC {1} \
    CONFIG.TDATA_NUM_BYTES {6} \
  ] $axis_data_fifo_0


  # Create instance: BRAMfifo, and set properties
  set BRAMfifo [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_ila:1.2 BRAMfifo ]
  set_property -dict [list \
    CONFIG.C_NUM_OF_PROBES {5} \
    CONFIG.C_PROBE0_WIDTH {10} \
    CONFIG.C_PROBE1_WIDTH {16} \
    CONFIG.C_PROBE3_WIDTH {16} \
  ] $BRAMfifo


  # Create port connections
  connect_bd_net -net PRVTDNN_bram_ctrl_bram_addr_a [get_bd_pins In_addr] [get_bd_pins xlconcat_0/In0]
  connect_bd_net -net PRVTDNN_bram_ctrl_bram_en_a [get_bd_pins s_axis_tvalid] [get_bd_pins axis_data_fifo_0/s_axis_tvalid]
  connect_bd_net -net PRVTDNN_bram_ctrl_bram_wrdata_a [get_bd_pins In_data] [get_bd_pins xlconcat_0/In1]
  connect_bd_net -net axis_data_fifo_0_m_axis_tdata [get_bd_pins axis_data_fifo_0/m_axis_tdata] [get_bd_pins xlslice_0/Din] [get_bd_pins xlslice_1/Din]
  connect_bd_net -net axis_data_fifo_0_m_axis_tvalid [get_bd_pins axis_data_fifo_0/m_axis_tvalid] [get_bd_pins BRAM_enable] [get_bd_pins BRAMfifo/probe2]
  connect_bd_net -net jesd_clock_0_coreclk [get_bd_pins m_axis_aclk] [get_bd_pins axis_data_fifo_0/m_axis_aclk] [get_bd_pins BRAMfifo/clk]
  connect_bd_net -net probe3_1 [get_bd_pins rddata] [get_bd_pins BRAMfifo/probe3]
  connect_bd_net -net probe4_1 [get_bd_pins load] [get_bd_pins BRAMfifo/probe4]
  connect_bd_net -net rst_versal_cips_0_333M_peripheral_aresetn [get_bd_pins s_axis_aresetn] [get_bd_pins axis_data_fifo_0/s_axis_aresetn]
  connect_bd_net -net versal_cips_0_pl0_ref_clk [get_bd_pins s_axis_aclk] [get_bd_pins axis_data_fifo_0/s_axis_aclk]
  connect_bd_net -net xlconcat_0_dout [get_bd_pins xlconcat_0/dout] [get_bd_pins axis_data_fifo_0/s_axis_tdata]
  connect_bd_net -net xlslice_0_Dout [get_bd_pins xlslice_0/Dout] [get_bd_pins BRAM_data] [get_bd_pins BRAMfifo/probe1]
  connect_bd_net -net xlslice_1_Dout [get_bd_pins xlslice_1/Dout] [get_bd_pins BRAM_addr] [get_bd_pins BRAMfifo/probe0]

  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: GT_PHY
proc create_hier_cell_GT_PHY { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_GT_PHY() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_tx_interface_rtl:1.0 TX0_GT_IP_Interface

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_tx_interface_rtl:1.0 TX1_GT_IP_Interface

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_tx_interface_rtl:1.0 TX2_GT_IP_Interface

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_tx_interface_rtl:1.0 TX3_GT_IP_Interface

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AXI_LITE

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 GT_Serial

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_rx_interface_rtl:1.0 RX0_GT_IP_Interface

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_rx_interface_rtl:1.0 RX1_GT_IP_Interface

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_rx_interface_rtl:1.0 RX2_GT_IP_Interface

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_rx_interface_rtl:1.0 RX3_GT_IP_Interface

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AXI_LITE1

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 GT_Serial_2

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_rx_interface_rtl:1.0 RX0_GT_IP_Interface1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_rx_interface_rtl:1.0 RX1_GT_IP_Interface1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_rx_interface_rtl:1.0 RX2_GT_IP_Interface1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_rx_interface_rtl:1.0 RX3_GT_IP_Interface1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AXI_LITE2

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 GT_Serial_0

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_tx_interface_rtl:1.0 TX0_GT_IP_Interface1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_tx_interface_rtl:1.0 TX1_GT_IP_Interface1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_tx_interface_rtl:1.0 TX2_GT_IP_Interface1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:gt_tx_interface_rtl:1.0 TX3_GT_IP_Interface1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 AXI_LITE3

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 GT_Serial_1


  # Create pins
  create_bd_pin -dir I -type gt_usrclk ch1_txusrclk
  create_bd_pin -dir I -from 2 -to 0 ch1_loopback
  create_bd_pin -dir I -type clk GT_REFCLK0
  create_bd_pin -dir I -type clk s_axi_lite_clk
  create_bd_pin -dir I -from 2 -to 0 ch1_loopback1
  create_bd_pin -dir O rplllock
  create_bd_pin -dir O gt_pd
  create_bd_pin -dir I -from 0 -to 0 In9
  create_bd_pin -dir I -type clk GT_REFCLK1
  create_bd_pin -dir I ch2_txusrclk

  # Create instance: AND_rplllock_logic, and set properties
  set AND_rplllock_logic [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_reduced_logic:2.0 AND_rplllock_logic ]
  set_property CONFIG.C_SIZE {8} $AND_rplllock_logic


  # Create instance: xlconcat_0, and set properties
  set xlconcat_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_0 ]
  set_property CONFIG.NUM_PORTS {8} $xlconcat_0


  # Create instance: xlconcat_2, and set properties
  set xlconcat_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_2 ]
  set_property CONFIG.NUM_PORTS {4} $xlconcat_2


  # Create instance: AND_gt_pg, and set properties
  set AND_gt_pg [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_reduced_logic:2.0 AND_gt_pg ]
  set_property CONFIG.C_SIZE {4} $AND_gt_pg


  # Create instance: xlconcat_1, and set properties
  set xlconcat_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_1 ]
  set_property -dict [list \
    CONFIG.IN0_WIDTH {1} \
    CONFIG.NUM_PORTS {16} \
  ] $xlconcat_1


  # Create instance: gt_quad_RX1, and set properties
  set gt_quad_RX1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:gt_quad_base:1.1 gt_quad_RX1 ]
  set_property -dict [list \
    CONFIG.PORTS_INFO_DICT {LANE_SEL_DICT {PROT0 RX0 PROT1 RX1 PROT2 RX2 PROT3 RX3 unconnected {TX0 TX1 TX2 TX3}} GT_TYPE GTY REG_CONF_INTF AXI_LITE BOARD_PARAMETER { }} \
    CONFIG.PROT0_GT_DIRECTION {SIMPLEX_RX} \
    CONFIG.PROT0_PRESET {GTY-JESD204_8B10B} \
    CONFIG.PROT1_NO_OF_LANES {1} \
    CONFIG.PROT2_NO_OF_LANES {1} \
    CONFIG.PROT3_NO_OF_LANES {1} \
    CONFIG.PROT_OUTCLK_VALUES {CH0_RXOUTCLK 245.76 CH0_TXOUTCLK 390.625 CH1_RXOUTCLK 245.76 CH1_TXOUTCLK 390.625 CH2_RXOUTCLK 245.76 CH2_TXOUTCLK 390.625 CH3_RXOUTCLK 245.76 CH3_TXOUTCLK 390.625} \
    CONFIG.REFCLK_STRING {HSCLK0_RPLLGTREFCLK0 refclk_PROT0_R0_PROT1_R0_245.76_MHz_unique1 HSCLK1_RPLLGTREFCLK0 refclk_PROT2_R0_PROT3_R0_245.76_MHz_unique1} \
    CONFIG.REG_CONF_INTF {AXI_LITE} \
  ] $gt_quad_RX1


  # Create instance: gt_quad_TX1, and set properties
  set gt_quad_TX1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:gt_quad_base:1.1 gt_quad_TX1 ]
  set_property -dict [list \
    CONFIG.PORTS_INFO_DICT {LANE_SEL_DICT {unconnected {RX0 RX1 RX2 RX3} PROT0 TX0 PROT1 TX1 PROT2 TX2 PROT3 TX3} GT_TYPE GTY REG_CONF_INTF AXI_LITE BOARD_PARAMETER { }} \
    CONFIG.PROT1_NO_OF_LANES {1} \
    CONFIG.PROT2_NO_OF_LANES {1} \
    CONFIG.PROT3_NO_OF_LANES {1} \
    CONFIG.PROT_OUTCLK_VALUES {CH0_RXOUTCLK 390.625 CH0_TXOUTCLK 245.76 CH1_RXOUTCLK 390.625 CH1_TXOUTCLK 245.76 CH2_RXOUTCLK 390.625 CH2_TXOUTCLK 245.76 CH3_RXOUTCLK 390.625 CH3_TXOUTCLK 245.76} \
    CONFIG.REFCLK_STRING {HSCLK0_RPLLGTREFCLK0 refclk_PROT0_R0_PROT1_R0_245.76_MHz_unique1 HSCLK1_RPLLGTREFCLK0 refclk_PROT2_R0_PROT3_R0_245.76_MHz_unique1} \
    CONFIG.REG_CONF_INTF {AXI_LITE} \
  ] $gt_quad_TX1


  # Create instance: gt_quad_TX0, and set properties
  set gt_quad_TX0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:gt_quad_base:1.1 gt_quad_TX0 ]
  set_property -dict [list \
    CONFIG.PORTS_INFO_DICT {LANE_SEL_DICT {unconnected {RX0 RX1 RX2 RX3} PROT0 TX0 PROT1 TX1 PROT2 TX2 PROT3 TX3} GT_TYPE GTY REG_CONF_INTF AXI_LITE BOARD_PARAMETER { }} \
    CONFIG.PROT0_GT_DIRECTION {SIMPLEX_TX} \
    CONFIG.PROT0_PRESET {GTY-JESD204_8B10B} \
    CONFIG.PROT1_NO_OF_LANES {1} \
    CONFIG.PROT2_NO_OF_LANES {1} \
    CONFIG.PROT3_NO_OF_LANES {1} \
    CONFIG.PROT_OUTCLK_VALUES {CH0_RXOUTCLK 390.625 CH0_TXOUTCLK 245.76 CH1_RXOUTCLK 390.625 CH1_TXOUTCLK 245.76 CH2_RXOUTCLK 390.625 CH2_TXOUTCLK 245.76 CH3_RXOUTCLK 390.625 CH3_TXOUTCLK 245.76} \
    CONFIG.REFCLK_STRING {HSCLK0_RPLLGTREFCLK0 refclk_PROT0_R0_PROT1_R0_245.76_MHz_unique1 HSCLK1_RPLLGTREFCLK0 refclk_PROT2_R0_PROT3_R0_245.76_MHz_unique1} \
    CONFIG.REG_CONF_INTF {AXI_LITE} \
  ] $gt_quad_TX0


  # Create instance: gt_quad_RX0, and set properties
  set gt_quad_RX0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:gt_quad_base:1.1 gt_quad_RX0 ]
  set_property -dict [list \
    CONFIG.PORTS_INFO_DICT {LANE_SEL_DICT {PROT0 RX0 PROT1 RX1 PROT2 RX2 PROT3 RX3 unconnected {TX0 TX1 TX2 TX3}} GT_TYPE GTY REG_CONF_INTF AXI_LITE BOARD_PARAMETER { }} \
    CONFIG.PROT0_GT_DIRECTION {SIMPLEX_RX} \
    CONFIG.PROT0_PRESET {GTY-JESD204_8B10B} \
    CONFIG.PROT1_NO_OF_LANES {1} \
    CONFIG.PROT2_NO_OF_LANES {1} \
    CONFIG.PROT3_NO_OF_LANES {1} \
    CONFIG.PROT_OUTCLK_VALUES {CH0_RXOUTCLK 245.76 CH0_TXOUTCLK 390.625 CH1_RXOUTCLK 245.76 CH1_TXOUTCLK 390.625 CH2_RXOUTCLK 245.76 CH2_TXOUTCLK 390.625 CH3_RXOUTCLK 245.76 CH3_TXOUTCLK 390.625} \
    CONFIG.REFCLK_STRING {HSCLK0_RPLLGTREFCLK0 refclk_PROT0_R0_PROT1_R0_245.76_MHz_unique1 HSCLK1_RPLLGTREFCLK0 refclk_PROT2_R0_PROT3_R0_245.76_MHz_unique1} \
    CONFIG.REG_CONF_INTF {AXI_LITE} \
  ] $gt_quad_RX0


  # Create interface connections
  connect_bd_intf_net -intf_net AXI_LITE3_1 [get_bd_intf_pins AXI_LITE3] [get_bd_intf_pins gt_quad_TX1/AXI_LITE]
  connect_bd_intf_net -intf_net TX0_GT_IP_Interface1_1 [get_bd_intf_pins TX0_GT_IP_Interface1] [get_bd_intf_pins gt_quad_TX1/TX0_GT_IP_Interface]
  connect_bd_intf_net -intf_net TX1_GT_IP_Interface1_1 [get_bd_intf_pins TX1_GT_IP_Interface1] [get_bd_intf_pins gt_quad_TX1/TX1_GT_IP_Interface]
  connect_bd_intf_net -intf_net TX2_GT_IP_Interface1_1 [get_bd_intf_pins TX2_GT_IP_Interface1] [get_bd_intf_pins gt_quad_TX1/TX2_GT_IP_Interface]
  connect_bd_intf_net -intf_net TX3_GT_IP_Interface1_1 [get_bd_intf_pins TX3_GT_IP_Interface1] [get_bd_intf_pins gt_quad_TX1/TX3_GT_IP_Interface]
  connect_bd_intf_net -intf_net gt_quad_RX1_GT_NORTHIN_SOUTHOUT [get_bd_intf_pins gt_quad_RX0/GT_NORTHOUT_SOUTHIN] [get_bd_intf_pins gt_quad_RX1/GT_NORTHIN_SOUTHOUT]
  connect_bd_intf_net -intf_net gt_quad_TX1_GT_Serial [get_bd_intf_pins GT_Serial] [get_bd_intf_pins gt_quad_TX1/GT_Serial]
  connect_bd_intf_net -intf_net gt_quad_base_0_GT_NORTHIN_SOUTHOUT [get_bd_intf_pins gt_quad_TX1/GT_NORTHIN_SOUTHOUT] [get_bd_intf_pins gt_quad_TX0/GT_NORTHOUT_SOUTHIN]
  connect_bd_intf_net -intf_net gt_quad_base_0_GT_Serial [get_bd_intf_pins GT_Serial_2] [get_bd_intf_pins gt_quad_RX1/GT_Serial]
  connect_bd_intf_net -intf_net gt_quad_base_1_GT_Serial [get_bd_intf_pins GT_Serial_1] [get_bd_intf_pins gt_quad_TX0/GT_Serial]
  connect_bd_intf_net -intf_net gt_quad_base_2_GT_Serial [get_bd_intf_pins GT_Serial_0] [get_bd_intf_pins gt_quad_RX0/GT_Serial]
  connect_bd_intf_net -intf_net jesd204c_0_GT_TX0 [get_bd_intf_pins TX0_GT_IP_Interface] [get_bd_intf_pins gt_quad_TX0/TX0_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_0_GT_TX1 [get_bd_intf_pins TX1_GT_IP_Interface] [get_bd_intf_pins gt_quad_TX0/TX1_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_0_GT_TX2 [get_bd_intf_pins TX2_GT_IP_Interface] [get_bd_intf_pins gt_quad_TX0/TX2_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_0_GT_TX3 [get_bd_intf_pins TX3_GT_IP_Interface] [get_bd_intf_pins gt_quad_TX0/TX3_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX0 [get_bd_intf_pins RX0_GT_IP_Interface1] [get_bd_intf_pins gt_quad_RX0/RX0_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX1 [get_bd_intf_pins RX1_GT_IP_Interface1] [get_bd_intf_pins gt_quad_RX0/RX1_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX2 [get_bd_intf_pins RX2_GT_IP_Interface1] [get_bd_intf_pins gt_quad_RX0/RX2_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX3 [get_bd_intf_pins RX3_GT_IP_Interface1] [get_bd_intf_pins gt_quad_RX0/RX3_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX4 [get_bd_intf_pins RX0_GT_IP_Interface] [get_bd_intf_pins gt_quad_RX1/RX0_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX5 [get_bd_intf_pins RX1_GT_IP_Interface] [get_bd_intf_pins gt_quad_RX1/RX1_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX6 [get_bd_intf_pins RX2_GT_IP_Interface] [get_bd_intf_pins gt_quad_RX1/RX2_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX7 [get_bd_intf_pins RX3_GT_IP_Interface] [get_bd_intf_pins gt_quad_RX1/RX3_GT_IP_Interface]
  connect_bd_intf_net -intf_net smartconnect_0_M03_AXI [get_bd_intf_pins AXI_LITE] [get_bd_intf_pins gt_quad_TX0/AXI_LITE]
  connect_bd_intf_net -intf_net smartconnect_0_M14_AXI [get_bd_intf_pins AXI_LITE2] [get_bd_intf_pins gt_quad_RX0/AXI_LITE]
  connect_bd_intf_net -intf_net smartconnect_0_M15_AXI [get_bd_intf_pins AXI_LITE1] [get_bd_intf_pins gt_quad_RX1/AXI_LITE]

  # Create port connections
  connect_bd_net -net AND_gt_pg_Res [get_bd_pins AND_gt_pg/Res] [get_bd_pins gt_pd]
  connect_bd_net -net AND_rplllock_logic_Res [get_bd_pins AND_rplllock_logic/Res] [get_bd_pins rplllock]
  connect_bd_net -net GT_REFCLK1_1 [get_bd_pins GT_REFCLK1] [get_bd_pins gt_quad_RX1/GT_REFCLK0] [get_bd_pins gt_quad_RX1/GT_REFCLK1] [get_bd_pins gt_quad_RX0/GT_REFCLK0] [get_bd_pins gt_quad_RX0/GT_REFCLK1]
  connect_bd_net -net In9_1 [get_bd_pins In9] [get_bd_pins xlconcat_1/In9] [get_bd_pins xlconcat_1/In10] [get_bd_pins xlconcat_1/In11] [get_bd_pins xlconcat_1/In8]
  connect_bd_net -net Net [get_bd_pins s_axi_lite_clk] [get_bd_pins gt_quad_RX1/s_axi_lite_clk] [get_bd_pins gt_quad_TX1/s_axi_lite_clk] [get_bd_pins gt_quad_TX0/s_axi_lite_clk] [get_bd_pins gt_quad_RX0/s_axi_lite_clk]
  connect_bd_net -net Net1 [get_bd_pins ch1_loopback1] [get_bd_pins gt_quad_TX1/ch0_loopback] [get_bd_pins gt_quad_TX1/ch1_loopback] [get_bd_pins gt_quad_TX1/ch2_loopback] [get_bd_pins gt_quad_TX1/ch3_loopback]
  connect_bd_net -net ch2_txusrclk_1 [get_bd_pins ch2_txusrclk] [get_bd_pins gt_quad_RX1/ch0_txusrclk] [get_bd_pins gt_quad_RX1/ch1_txusrclk] [get_bd_pins gt_quad_RX1/ch2_txusrclk] [get_bd_pins gt_quad_RX1/ch3_txusrclk] [get_bd_pins gt_quad_RX1/ch0_rxusrclk] [get_bd_pins gt_quad_RX1/ch1_rxusrclk] [get_bd_pins gt_quad_RX1/ch2_rxusrclk] [get_bd_pins gt_quad_RX1/ch3_rxusrclk] [get_bd_pins gt_quad_RX0/ch0_txusrclk] [get_bd_pins gt_quad_RX0/ch1_txusrclk] [get_bd_pins gt_quad_RX0/ch2_txusrclk] [get_bd_pins gt_quad_RX0/ch3_txusrclk] [get_bd_pins gt_quad_RX0/ch0_rxusrclk] [get_bd_pins gt_quad_RX0/ch1_rxusrclk] [get_bd_pins gt_quad_RX0/ch2_rxusrclk] [get_bd_pins gt_quad_RX0/ch3_rxusrclk]
  connect_bd_net -net gt_quad_RX0_gtpowergood [get_bd_pins gt_quad_RX1/gtpowergood] [get_bd_pins xlconcat_2/In2]
  connect_bd_net -net gt_quad_RX0_hsclk0_rplllock [get_bd_pins gt_quad_RX1/hsclk0_rplllock] [get_bd_pins xlconcat_0/In4]
  connect_bd_net -net gt_quad_RX0_hsclk1_rplllock [get_bd_pins gt_quad_RX1/hsclk1_rplllock] [get_bd_pins xlconcat_0/In5]
  connect_bd_net -net gt_quad_RX1_gtpowergood [get_bd_pins gt_quad_RX0/gtpowergood] [get_bd_pins xlconcat_2/In3]
  connect_bd_net -net gt_quad_RX1_hsclk0_rplllock [get_bd_pins gt_quad_RX0/hsclk0_rplllock] [get_bd_pins xlconcat_0/In6]
  connect_bd_net -net gt_quad_RX1_hsclk1_rplllock [get_bd_pins gt_quad_RX0/hsclk1_rplllock] [get_bd_pins xlconcat_0/In7]
  connect_bd_net -net gt_quad_TX1_gtpowergood [get_bd_pins gt_quad_TX1/gtpowergood] [get_bd_pins xlconcat_2/In1]
  connect_bd_net -net gt_quad_TX1_hsclk0_rplllock [get_bd_pins gt_quad_TX1/hsclk0_rplllock] [get_bd_pins xlconcat_0/In0]
  connect_bd_net -net gt_quad_TX1_hsclk1_rplllock [get_bd_pins gt_quad_TX1/hsclk1_rplllock] [get_bd_pins xlconcat_0/In1]
  connect_bd_net -net gt_quad_base_1_gtpowergood [get_bd_pins gt_quad_TX0/gtpowergood] [get_bd_pins xlconcat_2/In0]
  connect_bd_net -net gt_quad_base_1_hsclk0_rplllock [get_bd_pins gt_quad_TX0/hsclk0_rplllock] [get_bd_pins xlconcat_0/In2]
  connect_bd_net -net gt_quad_base_1_hsclk1_rplllock [get_bd_pins gt_quad_TX0/hsclk1_rplllock] [get_bd_pins xlconcat_0/In3]
  connect_bd_net -net jesd204c_0_gt_loopback [get_bd_pins ch1_loopback] [get_bd_pins gt_quad_TX0/ch0_loopback] [get_bd_pins gt_quad_TX0/ch1_loopback] [get_bd_pins gt_quad_TX0/ch2_loopback] [get_bd_pins gt_quad_TX0/ch3_loopback]
  connect_bd_net -net jesd_clock_0_coreclk [get_bd_pins ch1_txusrclk] [get_bd_pins gt_quad_TX1/ch0_txusrclk] [get_bd_pins gt_quad_TX1/ch1_txusrclk] [get_bd_pins gt_quad_TX1/ch2_txusrclk] [get_bd_pins gt_quad_TX1/ch3_txusrclk] [get_bd_pins gt_quad_TX1/ch0_rxusrclk] [get_bd_pins gt_quad_TX1/ch1_rxusrclk] [get_bd_pins gt_quad_TX1/ch2_rxusrclk] [get_bd_pins gt_quad_TX1/ch3_rxusrclk] [get_bd_pins gt_quad_TX0/ch0_txusrclk] [get_bd_pins gt_quad_TX0/ch1_txusrclk] [get_bd_pins gt_quad_TX0/ch2_txusrclk] [get_bd_pins gt_quad_TX0/ch3_txusrclk] [get_bd_pins gt_quad_TX0/ch0_rxusrclk] [get_bd_pins gt_quad_TX0/ch1_rxusrclk] [get_bd_pins gt_quad_TX0/ch2_rxusrclk] [get_bd_pins gt_quad_TX0/ch3_rxusrclk]
  connect_bd_net -net jesd_clock_1_refclk [get_bd_pins GT_REFCLK0] [get_bd_pins gt_quad_TX1/GT_REFCLK0] [get_bd_pins gt_quad_TX1/GT_REFCLK1] [get_bd_pins gt_quad_TX0/GT_REFCLK0] [get_bd_pins gt_quad_TX0/GT_REFCLK1]
  connect_bd_net -net xlconcat_0_dout [get_bd_pins xlconcat_0/dout] [get_bd_pins AND_rplllock_logic/Op1]
  connect_bd_net -net xlconcat_1_dout [get_bd_pins xlconcat_1/dout] [get_bd_pins gt_quad_RX1/gpi] [get_bd_pins gt_quad_RX0/gpi]
  connect_bd_net -net xlconcat_2_dout [get_bd_pins xlconcat_2/dout] [get_bd_pins AND_gt_pg/Op1]

  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: SPI_CTRL
proc create_hier_cell_SPI_CTRL { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_SPI_CTRL() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI1

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI2

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI3

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI5


  # Create pins
  create_bd_pin -dir I -type clk s_axi_aclk
  create_bd_pin -dir I -type rst s_axi_aresetn
  create_bd_pin -dir O -from 0 -to 0 DA_SCLK
  create_bd_pin -dir I -from 0 -to 0 DA_SDI
  create_bd_pin -dir IO -from 0 -to 0 DA_SDIO
  create_bd_pin -dir O -from 0 -to 0 AD_CS
  create_bd_pin -dir O -from 0 -to 0 AD_SCLK
  create_bd_pin -dir O -from 0 -to 0 DA_CS
  create_bd_pin -dir O -from 0 -to 0 CLK_CS
  create_bd_pin -dir I -from 0 -to 0 AD_SDI
  create_bd_pin -dir O -from 0 -to 0 AD_SDO

  # Create instance: AD_SDI_SDO_gpio, and set properties
  set AD_SDI_SDO_gpio [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 AD_SDI_SDO_gpio ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {1} \
    CONFIG.C_ALL_OUTPUTS_2 {1} \
    CONFIG.C_GPIO2_WIDTH {1} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {1} \
  ] $AD_SDI_SDO_gpio


  # Create instance: AD_CS_SCLK_gpio, and set properties
  set AD_CS_SCLK_gpio [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 AD_CS_SCLK_gpio ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {0} \
    CONFIG.C_ALL_OUTPUTS {1} \
    CONFIG.C_ALL_OUTPUTS_2 {1} \
    CONFIG.C_DOUT_DEFAULT {0x00000001} \
    CONFIG.C_GPIO2_WIDTH {1} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {1} \
  ] $AD_CS_SCLK_gpio


  # Create instance: DACLK_SDIO_SDI_gpio, and set properties
  set DACLK_SDIO_SDI_gpio [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 DACLK_SDIO_SDI_gpio ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {0} \
    CONFIG.C_ALL_INPUTS_2 {1} \
    CONFIG.C_ALL_OUTPUTS {0} \
    CONFIG.C_GPIO2_WIDTH {1} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {1} \
  ] $DACLK_SDIO_SDI_gpio


  # Create instance: DACLK_CS_SCLK_gpio, and set properties
  set DACLK_CS_SCLK_gpio [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 DACLK_CS_SCLK_gpio ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {0} \
    CONFIG.C_ALL_OUTPUTS {1} \
    CONFIG.C_ALL_OUTPUTS_2 {1} \
    CONFIG.C_DOUT_DEFAULT {0x00000001} \
    CONFIG.C_GPIO2_WIDTH {1} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {1} \
  ] $DACLK_CS_SCLK_gpio


  # Create instance: CLK_CS_gpio, and set properties
  set CLK_CS_gpio [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 CLK_CS_gpio ]
  set_property -dict [list \
    CONFIG.C_ALL_OUTPUTS {1} \
    CONFIG.C_DOUT_DEFAULT {0x00000001} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {0} \
  ] $CLK_CS_gpio


  # Create instance: util_ds_buf_5, and set properties
  set util_ds_buf_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 util_ds_buf_5 ]
  set_property CONFIG.C_BUF_TYPE {IOBUF} $util_ds_buf_5


  # Create interface connections
  connect_bd_intf_net -intf_net Conn1 [get_bd_intf_pins S_AXI3] [get_bd_intf_pins AD_CS_SCLK_gpio/S_AXI]
  connect_bd_intf_net -intf_net Conn4 [get_bd_intf_pins S_AXI5] [get_bd_intf_pins CLK_CS_gpio/S_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M07_AXI [get_bd_intf_pins S_AXI1] [get_bd_intf_pins DACLK_CS_SCLK_gpio/S_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M08_AXI [get_bd_intf_pins S_AXI2] [get_bd_intf_pins DACLK_SDIO_SDI_gpio/S_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M10_AXI [get_bd_intf_pins S_AXI] [get_bd_intf_pins AD_SDI_SDO_gpio/S_AXI]

  # Create port connections
  connect_bd_net -net AD_CS_SCLK_gpio_gpio2_io_o [get_bd_pins AD_CS_SCLK_gpio/gpio2_io_o] [get_bd_pins AD_SCLK]
  connect_bd_net -net AD_CS_SCLK_gpio_gpio_io_o [get_bd_pins AD_CS_SCLK_gpio/gpio_io_o] [get_bd_pins AD_CS]
  connect_bd_net -net AD_SDI_SDO_gpio_gpio2_io_o [get_bd_pins AD_SDI_SDO_gpio/gpio2_io_o] [get_bd_pins AD_SDO]
  connect_bd_net -net DA_CS_SCLK_gpio_gpio_io_o [get_bd_pins DACLK_CS_SCLK_gpio/gpio_io_o] [get_bd_pins DA_CS]
  connect_bd_net -net DA_CS_gpio_gpio2_io_o [get_bd_pins DACLK_CS_SCLK_gpio/gpio2_io_o] [get_bd_pins DA_SCLK]
  connect_bd_net -net DA_SDIO_SDO_gpio_gpio_io_t [get_bd_pins DACLK_SDIO_SDI_gpio/gpio_io_t] [get_bd_pins util_ds_buf_5/IOBUF_IO_T]
  connect_bd_net -net DA_SDIO_gpio_gpio_io_o [get_bd_pins DACLK_SDIO_SDI_gpio/gpio_io_o] [get_bd_pins util_ds_buf_5/IOBUF_IO_I]
  connect_bd_net -net LMK_CS_SDIO_gpio_gpio_io_o [get_bd_pins CLK_CS_gpio/gpio_io_o] [get_bd_pins CLK_CS]
  connect_bd_net -net Net [get_bd_pins s_axi_aclk] [get_bd_pins AD_SDI_SDO_gpio/s_axi_aclk] [get_bd_pins AD_CS_SCLK_gpio/s_axi_aclk] [get_bd_pins DACLK_SDIO_SDI_gpio/s_axi_aclk] [get_bd_pins DACLK_CS_SCLK_gpio/s_axi_aclk] [get_bd_pins CLK_CS_gpio/s_axi_aclk]
  connect_bd_net -net Net1 [get_bd_pins DA_SDIO] [get_bd_pins util_ds_buf_5/IOBUF_IO_IO]
  connect_bd_net -net gpio2_io_i_0_1 [get_bd_pins DA_SDI] [get_bd_pins DACLK_SDIO_SDI_gpio/gpio2_io_i]
  connect_bd_net -net gpio_io_i_0_1 [get_bd_pins AD_SDI] [get_bd_pins AD_SDI_SDO_gpio/gpio_io_i]
  connect_bd_net -net rst_versal_cips_0_333M_peripheral_aresetn [get_bd_pins s_axi_aresetn] [get_bd_pins AD_SDI_SDO_gpio/s_axi_aresetn] [get_bd_pins AD_CS_SCLK_gpio/s_axi_aresetn] [get_bd_pins DACLK_SDIO_SDI_gpio/s_axi_aresetn] [get_bd_pins DACLK_CS_SCLK_gpio/s_axi_aresetn] [get_bd_pins CLK_CS_gpio/s_axi_aresetn]
  connect_bd_net -net util_ds_buf_5_IOBUF_IO_O [get_bd_pins util_ds_buf_5/IOBUF_IO_O] [get_bd_pins DACLK_SDIO_SDI_gpio/gpio_io_i]

  # Restore current instance
  current_bd_instance $oldCurInst
}


# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set GT_TX1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 GT_TX1 ]

  set GT_TX0 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 GT_TX0 ]

  set TX_SYNC1 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 TX_SYNC1 ]

  set RX_SYNC [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:diff_clock_rtl:1.0 RX_SYNC ]

  set SYSREF [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 SYSREF ]

  set TX_SYNC0 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 TX_SYNC0 ]

  set GT_RX1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 GT_RX1 ]

  set GT_RX0 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 GT_RX0 ]

  set ddr4_dimm1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddr4_rtl:1.0 ddr4_dimm1 ]

  set ddr4_dimm1_sma_clk [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 ddr4_dimm1_sma_clk ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {200000000} \
   ] $ddr4_dimm1_sma_clk

  set ddr4_dimm1_0 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddr4_rtl:1.0 ddr4_dimm1_0 ]

  set ddr4_dimm1_sma_clk_0 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 ddr4_dimm1_sma_clk_0 ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {200000000} \
   ] $ddr4_dimm1_sma_clk_0


  # Create ports
  set refclk_DAC_n [ create_bd_port -dir I refclk_DAC_n ]
  set refclk_DAC_p [ create_bd_port -dir I refclk_DAC_p ]
  set RESET [ create_bd_port -dir I RESET ]
  set DACLK_SCLK [ create_bd_port -dir O -from 0 -to 0 DACLK_SCLK ]
  set DACLK_SDIO [ create_bd_port -dir IO -from 0 -to 0 DACLK_SDIO ]
  set DACLK_SDI [ create_bd_port -dir I -from 0 -to 0 DACLK_SDI ]
  set AD_CS [ create_bd_port -dir O -from 0 -to 0 AD_CS ]
  set AD_SCLK [ create_bd_port -dir O -from 0 -to 0 AD_SCLK ]
  set DA_CS [ create_bd_port -dir O -from 0 -to 0 DA_CS ]
  set CLK_CS [ create_bd_port -dir O -from 0 -to 0 CLK_CS ]
  set TXEN [ create_bd_port -dir O -from 1 -to 0 TXEN ]
  set FMC_SPI_EN [ create_bd_port -dir O -from 0 -to 0 FMC_SPI_EN ]
  set AD_SDI [ create_bd_port -dir I -from 0 -to 0 AD_SDI ]
  set AD_SDO [ create_bd_port -dir O -from 0 -to 0 AD_SDO ]
  set AD_PWDN [ create_bd_port -dir O -from 0 -to 0 AD_PWDN ]
  set refclk_ADC_n [ create_bd_port -dir I refclk_ADC_n ]
  set refclk_ADC_p [ create_bd_port -dir I refclk_ADC_p ]
  set LED_OVF [ create_bd_port -dir O LED_OVF ]

  # Create instance: rst_versal_cips_0_333M, and set properties
  set rst_versal_cips_0_333M [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 rst_versal_cips_0_333M ]

  # Create instance: SPI_CTRL
  create_hier_cell_SPI_CTRL [current_bd_instance .] SPI_CTRL

  # Create instance: GT_PHY
  create_hier_cell_GT_PHY [current_bd_instance .] GT_PHY

  # Create instance: jesd_clock_DAC, and set properties
  set block_name jesd_clock
  set block_cell_name jesd_clock_DAC
  if { [catch {set jesd_clock_DAC [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $jesd_clock_DAC eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: jesd_clock_ADC, and set properties
  set block_name jesd_clock
  set block_cell_name jesd_clock_ADC
  if { [catch {set jesd_clock_ADC [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $jesd_clock_ADC eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: rx_mapper_0, and set properties
  set block_name rx_mapper
  set block_cell_name rx_mapper_0
  if { [catch {set rx_mapper_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $rx_mapper_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: tx_mapper_0, and set properties
  set block_name tx_mapper
  set block_cell_name tx_mapper_0
  if { [catch {set tx_mapper_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $tx_mapper_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: inOutAct_0, and set properties
  set block_name inOutAct
  set block_cell_name inOutAct_0
  if { [catch {set inOutAct_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $inOutAct_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: BRAM_FIFO
  create_hier_cell_BRAM_FIFO [current_bd_instance .] BRAM_FIFO

  # Create instance: RVTDNNTop_0, and set properties
  set block_name RVTDNNTop
  set block_cell_name RVTDNNTop_0
  if { [catch {set RVTDNNTop_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $RVTDNNTop_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: MP_bram_ctrl, and set properties
  set MP_bram_ctrl [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 MP_bram_ctrl ]
  set_property CONFIG.SINGLE_PORT_BRAM {1} $MP_bram_ctrl


  # Create instance: PRVTDNN_bram_ctrl, and set properties
  set PRVTDNN_bram_ctrl [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 PRVTDNN_bram_ctrl ]
  set_property CONFIG.SINGLE_PORT_BRAM {1} $PRVTDNN_bram_ctrl


  # Create instance: axi_dma_0, and set properties
  set axi_dma_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.1 axi_dma_0 ]
  set_property -dict [list \
    CONFIG.c_include_mm2s_dre {0} \
    CONFIG.c_include_sg {1} \
    CONFIG.c_m_axi_mm2s_data_width {512} \
    CONFIG.c_m_axi_s2mm_data_width {512} \
    CONFIG.c_m_axis_mm2s_tdata_width {256} \
    CONFIG.c_mm2s_burst_size {64} \
    CONFIG.c_s2mm_burst_size {64} \
    CONFIG.c_s_axis_s2mm_tdata_width {256} \
    CONFIG.c_sg_include_stscntrl_strm {0} \
    CONFIG.c_sg_length_width {26} \
  ] $axi_dma_0


  # Create instance: plllock_gpio, and set properties
  set plllock_gpio [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 plllock_gpio ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {1} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {0} \
  ] $plllock_gpio


  # Create instance: PRVTDNNload_gpio, and set properties
  set PRVTDNNload_gpio [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 PRVTDNNload_gpio ]
  set_property -dict [list \
    CONFIG.C_ALL_OUTPUTS {1} \
    CONFIG.C_GPIO_WIDTH {1} \
  ] $PRVTDNNload_gpio


  # Create instance: axis_ila_RX, and set properties
  set axis_ila_RX [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_ila:1.2 axis_ila_RX ]
  set_property -dict [list \
    CONFIG.ALL_PROBE_SAME_MU_CNT {2} \
    CONFIG.C_DATA_DEPTH {4096} \
    CONFIG.C_NUM_OF_PROBES {7} \
    CONFIG.C_PROBE0_MU_CNT {2} \
    CONFIG.C_PROBE0_WIDTH {16} \
    CONFIG.C_PROBE1_MU_CNT {2} \
    CONFIG.C_PROBE1_WIDTH {16} \
    CONFIG.C_PROBE2_MU_CNT {2} \
    CONFIG.C_PROBE2_WIDTH {16} \
    CONFIG.C_PROBE3_MU_CNT {2} \
    CONFIG.C_PROBE3_WIDTH {16} \
    CONFIG.C_PROBE4_MU_CNT {2} \
    CONFIG.C_PROBE5_MU_CNT {2} \
    CONFIG.C_PROBE6_MU_CNT {2} \
  ] $axis_ila_RX


  # Create instance: axis_ila_BRAM, and set properties
  set axis_ila_BRAM [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_ila:1.2 axis_ila_BRAM ]
  set_property -dict [list \
    CONFIG.C_DATA_DEPTH {4096} \
    CONFIG.C_NUM_OF_PROBES {6} \
    CONFIG.C_PROBE0_WIDTH {16} \
    CONFIG.C_PROBE1_WIDTH {32} \
    CONFIG.C_PROBE3_WIDTH {10} \
    CONFIG.C_PROBE4_WIDTH {32} \
  ] $axis_ila_BRAM


  # Create instance: axis_ila_TX, and set properties
  set axis_ila_TX [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_ila:1.2 axis_ila_TX ]
  set_property -dict [list \
    CONFIG.ALL_PROBE_SAME_MU_CNT {2} \
    CONFIG.C_DATA_DEPTH {4096} \
    CONFIG.C_NUM_OF_PROBES {8} \
    CONFIG.C_PROBE0_MU_CNT {2} \
    CONFIG.C_PROBE0_WIDTH {16} \
    CONFIG.C_PROBE1_MU_CNT {2} \
    CONFIG.C_PROBE1_WIDTH {16} \
    CONFIG.C_PROBE2_MU_CNT {2} \
    CONFIG.C_PROBE2_WIDTH {16} \
    CONFIG.C_PROBE3_MU_CNT {2} \
    CONFIG.C_PROBE3_WIDTH {16} \
    CONFIG.C_PROBE4_MU_CNT {2} \
    CONFIG.C_PROBE4_WIDTH {16} \
    CONFIG.C_PROBE5_MU_CNT {2} \
    CONFIG.C_PROBE5_WIDTH {16} \
    CONFIG.C_PROBE6_MU_CNT {2} \
    CONFIG.C_PROBE6_WIDTH {16} \
    CONFIG.C_PROBE7_MU_CNT {2} \
    CONFIG.C_PROBE7_WIDTH {16} \
  ] $axis_ila_TX


  # Create instance: axis_ila_DMA, and set properties
  set axis_ila_DMA [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_ila:1.2 axis_ila_DMA ]
  set_property -dict [list \
    CONFIG.C_DATA_DEPTH {1024} \
    CONFIG.C_MON_TYPE {Interface_Monitor} \
    CONFIG.C_NUM_MONITOR_SLOTS {3} \
    CONFIG.C_SLOT {0} \
    CONFIG.C_SLOT_0_APC_EN {0} \
  ] $axis_ila_DMA


  # Create instance: axis_ila_JESD, and set properties
  set axis_ila_JESD [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_ila:1.2 axis_ila_JESD ]
  set_property -dict [list \
    CONFIG.ALL_PROBE_SAME_MU {true} \
    CONFIG.C_DATA_DEPTH {1024} \
    CONFIG.C_MON_TYPE {Net_Probes} \
    CONFIG.C_NUM_MONITOR_SLOTS {4} \
    CONFIG.C_NUM_OF_PROBES {11} \
    CONFIG.C_PROBE0_WIDTH {32} \
    CONFIG.C_PROBE1_WIDTH {4} \
    CONFIG.C_PROBE2_WIDTH {32} \
    CONFIG.C_PROBE3_WIDTH {4} \
    CONFIG.C_PROBE4_WIDTH {32} \
    CONFIG.C_PROBE5_WIDTH {4} \
    CONFIG.C_PROBE6_WIDTH {32} \
    CONFIG.C_PROBE7_WIDTH {4} \
    CONFIG.C_PROBE8_WIDTH {4} \
    CONFIG.C_PROBE9_WIDTH {4} \
    CONFIG.C_SLOT {0} \
    CONFIG.C_SLOT_0_INTF_TYPE {xilinx.com:display_jesd204:jesd204_tx_bus_rtl:1.0} \
    CONFIG.C_SLOT_1_INTF_TYPE {xilinx.com:display_jesd204:jesd204_tx_bus_rtl:1.0} \
    CONFIG.C_SLOT_2_INTF_TYPE {xilinx.com:display_jesd204:jesd204_tx_bus_rtl:1.0} \
    CONFIG.C_SLOT_3_INTF_TYPE {xilinx.com:display_jesd204:jesd204_tx_bus_rtl:1.0} \
  ] $axis_ila_JESD


  # Create instance: OTHER_CTRL, and set properties
  set OTHER_CTRL [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_vio:1.0 OTHER_CTRL ]
  set_property -dict [list \
    CONFIG.C_NUM_PROBE_IN {0} \
    CONFIG.C_NUM_PROBE_OUT {3} \
    CONFIG.C_PROBE_OUT1_INIT_VAL {0x3} \
    CONFIG.C_PROBE_OUT1_WIDTH {2} \
  ] $OTHER_CTRL


  # Create instance: DPD_en, and set properties
  set DPD_en [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_vio:1.0 DPD_en ]
  set_property -dict [list \
    CONFIG.C_NUM_PROBE_IN {0} \
    CONFIG.C_PROBE_OUT0_INIT_VAL {0x0} \
  ] $DPD_en


  # Create instance: jesd204c_0, and set properties
  set jesd204c_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:jesd204c:4.2 jesd204c_0 ]
  set_property -dict [list \
    CONFIG.C_ENCODING {0} \
    CONFIG.C_LANES {4} \
    CONFIG.GT_Line_Rate {9.8304} \
    CONFIG.GT_REFCLK_FREQ_REQUEST {245.76} \
  ] $jesd204c_0


  # Create instance: jesd204c_1, and set properties
  set jesd204c_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:jesd204c:4.2 jesd204c_1 ]
  set_property -dict [list \
    CONFIG.C_ENCODING {0} \
    CONFIG.C_LANES {4} \
    CONFIG.GT_Line_Rate {9.8304} \
    CONFIG.GT_REFCLK_FREQ_REQUEST {245.76} \
  ] $jesd204c_1


  # Create instance: jesd204c_2, and set properties
  set jesd204c_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:jesd204c:4.2 jesd204c_2 ]
  set_property -dict [list \
    CONFIG.C_ENCODING {0} \
    CONFIG.C_LANES {8} \
    CONFIG.C_NODE_IS_TRANSMIT {0} \
    CONFIG.GT_Line_Rate {9.8304} \
    CONFIG.GT_REFCLK_FREQ_REQUEST {245.76} \
  ] $jesd204c_2


  # Create instance: smartconnect_0, and set properties
  set smartconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 smartconnect_0 ]
  set_property -dict [list \
    CONFIG.NUM_MI {17} \
    CONFIG.NUM_SI {1} \
  ] $smartconnect_0


  # Create instance: util_ds_buf_0, and set properties
  set util_ds_buf_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 util_ds_buf_0 ]

  # Create instance: util_ds_buf_1, and set properties
  set util_ds_buf_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 util_ds_buf_1 ]

  # Create instance: util_ds_buf_3, and set properties
  set util_ds_buf_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 util_ds_buf_3 ]

  # Create instance: util_ds_buf_2, and set properties
  set util_ds_buf_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 util_ds_buf_2 ]
  set_property CONFIG.C_BUF_TYPE {OBUFDS} $util_ds_buf_2


  # Create instance: versal_cips_0, and set properties
  set versal_cips_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:versal_cips:3.3 versal_cips_0 ]
  set_property -dict [list \
    CONFIG.CLOCK_MODE {Custom} \
    CONFIG.CPM_CONFIG { \
      CPM_PCIE0_MODES {None} \
    } \
    CONFIG.DDR_MEMORY_MODE {Custom} \
    CONFIG.DEBUG_MODE {JTAG} \
    CONFIG.DESIGN_MODE {1} \
    CONFIG.IO_CONFIG_MODE {Custom} \
    CONFIG.PS_BOARD_INTERFACE {Custom} \
    CONFIG.PS_PL_CONNECTIVITY_MODE {Custom} \
    CONFIG.PS_PMC_CONFIG { \
      CLOCK_MODE {Custom} \
      DDR_MEMORY_MODE {Connectivity to DDR via NOC} \
      DEBUG_MODE {JTAG} \
      DESIGN_MODE {1} \
      IO_CONFIG_MODE {Custom} \
      PMC_CRP_PL0_REF_CTRL_FREQMHZ {100} \
      PMC_GPIO0_MIO_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 0 .. 25}}} \
      PMC_GPIO1_MIO_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 26 .. 51}}} \
      PMC_I2CPMC_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 46 .. 47}}} \
      PMC_MIO37 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA high} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE GPIO}} \
      PMC_OSPI_PERIPHERAL {{ENABLE 0} {IO {PMC_MIO 0 .. 11}} {MODE Single}} \
      PMC_QSPI_COHERENCY {0} \
      PMC_QSPI_FBCLK {{ENABLE 1} {IO {PMC_MIO 6}}} \
      PMC_QSPI_PERIPHERAL_DATA_MODE {x4} \
      PMC_QSPI_PERIPHERAL_ENABLE {1} \
      PMC_QSPI_PERIPHERAL_MODE {Dual Parallel} \
      PMC_REF_CLK_FREQMHZ {33.3333} \
      PMC_SD1 {{CD_ENABLE 1} {CD_IO {PMC_MIO 28}} {POW_ENABLE 1} {POW_IO {PMC_MIO 51}} {RESET_ENABLE 0} {RESET_IO {PMC_MIO 12}} {WP_ENABLE 0} {WP_IO {PMC_MIO 1}}} \
      PMC_SD1_COHERENCY {0} \
      PMC_SD1_DATA_TRANSFER_MODE {8Bit} \
      PMC_SD1_PERIPHERAL {{CLK_100_SDR_OTAP_DLY 0x3} {CLK_200_SDR_OTAP_DLY 0x2} {CLK_50_DDR_ITAP_DLY 0x36} {CLK_50_DDR_OTAP_DLY 0x3} {CLK_50_SDR_ITAP_DLY 0x2C} {CLK_50_SDR_OTAP_DLY 0x4} {ENABLE 1} {IO\
{PMC_MIO 26 .. 36}}} \
      PMC_SD1_SLOT_TYPE {SD 3.0} \
      PMC_USE_PMC_NOC_AXI0 {1} \
      PS_BOARD_INTERFACE {Custom} \
      PS_CAN1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 40 .. 41}}} \
      PS_ENET0_MDIO {{ENABLE 1} {IO {PS_MIO 24 .. 25}}} \
      PS_ENET0_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 0 .. 11}}} \
      PS_ENET1_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 12 .. 23}}} \
      PS_GEN_IPI0_ENABLE {1} \
      PS_GEN_IPI0_MASTER {A72} \
      PS_GEN_IPI1_ENABLE {1} \
      PS_GEN_IPI2_ENABLE {1} \
      PS_GEN_IPI3_ENABLE {1} \
      PS_GEN_IPI4_ENABLE {1} \
      PS_GEN_IPI5_ENABLE {1} \
      PS_GEN_IPI6_ENABLE {1} \
      PS_HSDP_EGRESS_TRAFFIC {JTAG} \
      PS_HSDP_INGRESS_TRAFFIC {JTAG} \
      PS_HSDP_MODE {NONE} \
      PS_I2C0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 38 .. 39}}} \
      PS_I2C1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 44 .. 45}}} \
      PS_IRQ_USAGE {{CH0 0} {CH1 0} {CH10 0} {CH11 0} {CH12 0} {CH13 0} {CH14 0} {CH15 0} {CH2 0} {CH3 0} {CH4 0} {CH5 0} {CH6 0} {CH7 0} {CH8 1} {CH9 1}} \
      PS_MIO19 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} \
      PS_MIO21 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} \
      PS_MIO7 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} \
      PS_MIO9 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} \
      PS_NUM_FABRIC_RESETS {1} \
      PS_PCIE_RESET {{ENABLE 1}} \
      PS_PL_CONNECTIVITY_MODE {Custom} \
      PS_TTC0_CLK {{ENABLE 0} {IO {PS_MIO 6}}} \
      PS_TTC0_PERIPHERAL_ENABLE {1} \
      PS_UART0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 42 .. 43}}} \
      PS_USB3_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 13 .. 25}}} \
      PS_USE_FPD_CCI_NOC {1} \
      PS_USE_FPD_CCI_NOC0 {1} \
      PS_USE_M_AXI_FPD {1} \
      PS_USE_NOC_LPD_AXI0 {1} \
      PS_USE_PMCPL_CLK0 {1} \
      SMON_ALARMS {Set_Alarms_On} \
      SMON_ENABLE_TEMP_AVERAGING {0} \
      SMON_TEMP_AVERAGING_SAMPLES {0} \
    } \
    CONFIG.PS_PMC_CONFIG_APPLIED {1} \
  ] $versal_cips_0


  # Create instance: xlconstant_vcc, and set properties
  set xlconstant_vcc [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_vcc ]

  # Create instance: axi_noc_0, and set properties
  set axi_noc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc:1.0 axi_noc_0 ]
  set_property -dict [list \
    CONFIG.CH0_DDR4_0_BOARD_INTERFACE {ddr4_dimm1} \
    CONFIG.MC_CHAN_REGION1 {DDR_LOW1} \
    CONFIG.NUM_CLKS {9} \
    CONFIG.NUM_MCP {4} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_SI {9} \
    CONFIG.sys_clk0_BOARD_INTERFACE {ddr4_dimm1_sma_clk} \
  ] $axi_noc_0


  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_0 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /axi_noc_0/S00_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_1 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /axi_noc_0/S01_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_2 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /axi_noc_0/S02_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_3 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /axi_noc_0/S03_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_0 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_rpu} \
 ] [get_bd_intf_pins /axi_noc_0/S04_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_0 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_pmc} \
 ] [get_bd_intf_pins /axi_noc_0/S05_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_1 {read_bw {10000} write_bw {1720} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /axi_noc_0/S06_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_2 {read_bw {1720} write_bw {10000} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /axi_noc_0/S07_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_3 {read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /axi_noc_0/S08_AXI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk0]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S01_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk1]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S02_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk2]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S03_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk3]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S04_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk4]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S05_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk5]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S06_AXI:S07_AXI:S08_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk6]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {} \
 ] [get_bd_pins /axi_noc_0/aclk7]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {} \
 ] [get_bd_pins /axi_noc_0/aclk8]

  # Create instance: loop_ctrl_0, and set properties
  set block_name loop_ctrl
  set block_cell_name loop_ctrl_0
  if { [catch {set loop_ctrl_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $loop_ctrl_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: axis_vio_0, and set properties
  set axis_vio_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_vio:1.0 axis_vio_0 ]
  set_property -dict [list \
    CONFIG.C_NUM_PROBE_IN {0} \
    CONFIG.C_NUM_PROBE_OUT {3} \
    CONFIG.C_PROBE_OUT0_WIDTH {2} \
  ] $axis_vio_0


  # Create instance: adc_to_axistream_0, and set properties
  set block_name adc_to_axistream
  set block_cell_name adc_to_axistream_0
  if { [catch {set adc_to_axistream_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $adc_to_axistream_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
    set_property CONFIG.ADC_DWIDTH {256} $adc_to_axistream_0


  set_property -dict [ list \
   CONFIG.FREQ_HZ {156250000} \
 ] [get_bd_intf_pins /adc_to_axistream_0/m_axis_adc]

  # Create instance: axis_vio_1, and set properties
  set axis_vio_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_vio:1.0 axis_vio_1 ]
  set_property -dict [list \
    CONFIG.C_NUM_PROBE_IN {0} \
    CONFIG.C_PROBE_OUT0_WIDTH {32} \
  ] $axis_vio_1


  # Create interface connections
  connect_bd_intf_net -intf_net CLK_IN_D_0_1 [get_bd_intf_ports TX_SYNC0] [get_bd_intf_pins util_ds_buf_1/CLK_IN_D]
  connect_bd_intf_net -intf_net CLK_IN_D_0_2 [get_bd_intf_ports TX_SYNC1] [get_bd_intf_pins util_ds_buf_0/CLK_IN_D]
  connect_bd_intf_net -intf_net CLK_IN_D_0_3 [get_bd_intf_ports SYSREF] [get_bd_intf_pins util_ds_buf_3/CLK_IN_D]
  connect_bd_intf_net -intf_net S_AXI5_1 [get_bd_intf_pins SPI_CTRL/S_AXI5] [get_bd_intf_pins smartconnect_0/M11_AXI]
  connect_bd_intf_net -intf_net adc_to_axistream_0_m_axis_adc [get_bd_intf_pins adc_to_axistream_0/m_axis_adc] [get_bd_intf_pins axi_dma_0/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net axi_dma_0_M_AXI_MM2S [get_bd_intf_pins axi_dma_0/M_AXI_MM2S] [get_bd_intf_pins axi_noc_0/S06_AXI]
connect_bd_intf_net -intf_net [get_bd_intf_nets axi_dma_0_M_AXI_MM2S] [get_bd_intf_pins axi_dma_0/M_AXI_MM2S] [get_bd_intf_pins axis_ila_DMA/SLOT_0_AXI]
  connect_bd_intf_net -intf_net axi_dma_0_M_AXI_S2MM [get_bd_intf_pins axi_dma_0/M_AXI_S2MM] [get_bd_intf_pins axi_noc_0/S07_AXI]
connect_bd_intf_net -intf_net [get_bd_intf_nets axi_dma_0_M_AXI_S2MM] [get_bd_intf_pins axi_dma_0/M_AXI_S2MM] [get_bd_intf_pins axis_ila_DMA/SLOT_1_AXI]
  connect_bd_intf_net -intf_net axi_dma_0_M_AXI_SG [get_bd_intf_pins axi_dma_0/M_AXI_SG] [get_bd_intf_pins axi_noc_0/S08_AXI]
connect_bd_intf_net -intf_net [get_bd_intf_nets axi_dma_0_M_AXI_SG] [get_bd_intf_pins axi_dma_0/M_AXI_SG] [get_bd_intf_pins axis_ila_DMA/SLOT_2_AXI]
  connect_bd_intf_net -intf_net axi_noc_0_CH0_DDR4_0 [get_bd_intf_ports ddr4_dimm1_0] [get_bd_intf_pins axi_noc_0/CH0_DDR4_0]
  connect_bd_intf_net -intf_net ddr4_dimm1_sma_clk_0_1 [get_bd_intf_ports ddr4_dimm1_sma_clk_0] [get_bd_intf_pins axi_noc_0/sys_clk0]
  connect_bd_intf_net -intf_net gt_quad_base_0_GT_Serial [get_bd_intf_ports GT_RX0] [get_bd_intf_pins GT_PHY/GT_Serial_2]
  connect_bd_intf_net -intf_net gt_quad_base_1_GT_Serial [get_bd_intf_ports GT_TX0] [get_bd_intf_pins GT_PHY/GT_Serial_1]
  connect_bd_intf_net -intf_net gt_quad_base_2_GT_Serial [get_bd_intf_ports GT_RX1] [get_bd_intf_pins GT_PHY/GT_Serial_0]
  connect_bd_intf_net -intf_net gt_quad_base_GT_Serial [get_bd_intf_ports GT_TX1] [get_bd_intf_pins GT_PHY/GT_Serial]
  connect_bd_intf_net -intf_net jesd204c_0_GT_TX0 [get_bd_intf_pins jesd204c_0/GT_TX0] [get_bd_intf_pins GT_PHY/TX0_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_0_GT_TX1 [get_bd_intf_pins jesd204c_0/GT_TX1] [get_bd_intf_pins GT_PHY/TX1_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_0_GT_TX2 [get_bd_intf_pins jesd204c_0/GT_TX2] [get_bd_intf_pins GT_PHY/TX2_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_0_GT_TX3 [get_bd_intf_pins jesd204c_0/GT_TX3] [get_bd_intf_pins GT_PHY/TX3_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_1_GT_TX0 [get_bd_intf_pins jesd204c_1/GT_TX0] [get_bd_intf_pins GT_PHY/TX0_GT_IP_Interface1]
  connect_bd_intf_net -intf_net jesd204c_1_GT_TX1 [get_bd_intf_pins jesd204c_1/GT_TX1] [get_bd_intf_pins GT_PHY/TX1_GT_IP_Interface1]
  connect_bd_intf_net -intf_net jesd204c_1_GT_TX2 [get_bd_intf_pins jesd204c_1/GT_TX2] [get_bd_intf_pins GT_PHY/TX2_GT_IP_Interface1]
  connect_bd_intf_net -intf_net jesd204c_1_GT_TX3 [get_bd_intf_pins jesd204c_1/GT_TX3] [get_bd_intf_pins GT_PHY/TX3_GT_IP_Interface1]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX0 [get_bd_intf_pins GT_PHY/RX0_GT_IP_Interface1] [get_bd_intf_pins jesd204c_2/GT_RX0]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX1 [get_bd_intf_pins jesd204c_2/GT_RX1] [get_bd_intf_pins GT_PHY/RX1_GT_IP_Interface1]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX2 [get_bd_intf_pins GT_PHY/RX2_GT_IP_Interface1] [get_bd_intf_pins jesd204c_2/GT_RX2]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX3 [get_bd_intf_pins jesd204c_2/GT_RX3] [get_bd_intf_pins GT_PHY/RX3_GT_IP_Interface1]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX4 [get_bd_intf_pins GT_PHY/RX0_GT_IP_Interface] [get_bd_intf_pins jesd204c_2/GT_RX4]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX5 [get_bd_intf_pins jesd204c_2/GT_RX5] [get_bd_intf_pins GT_PHY/RX1_GT_IP_Interface]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX6 [get_bd_intf_pins GT_PHY/RX2_GT_IP_Interface] [get_bd_intf_pins jesd204c_2/GT_RX6]
  connect_bd_intf_net -intf_net jesd204c_2_GT_RX7 [get_bd_intf_pins jesd204c_2/GT_RX7] [get_bd_intf_pins GT_PHY/RX3_GT_IP_Interface]
  connect_bd_intf_net -intf_net smartconnect_0_M00_AXI [get_bd_intf_pins smartconnect_0/M00_AXI] [get_bd_intf_pins jesd204c_0/s_axi]
  connect_bd_intf_net -intf_net smartconnect_0_M01_AXI [get_bd_intf_pins smartconnect_0/M01_AXI] [get_bd_intf_pins jesd204c_2/s_axi]
  connect_bd_intf_net -intf_net smartconnect_0_M02_AXI [get_bd_intf_pins smartconnect_0/M02_AXI] [get_bd_intf_pins jesd204c_1/s_axi]
  connect_bd_intf_net -intf_net smartconnect_0_M03_AXI [get_bd_intf_pins GT_PHY/AXI_LITE] [get_bd_intf_pins smartconnect_0/M03_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M04_AXI [get_bd_intf_pins GT_PHY/AXI_LITE3] [get_bd_intf_pins smartconnect_0/M04_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M05_AXI [get_bd_intf_pins smartconnect_0/M05_AXI] [get_bd_intf_pins axi_dma_0/S_AXI_LITE]
  connect_bd_intf_net -intf_net smartconnect_0_M06_AXI [get_bd_intf_pins smartconnect_0/M06_AXI] [get_bd_intf_pins plllock_gpio/S_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M07_AXI [get_bd_intf_pins smartconnect_0/M07_AXI] [get_bd_intf_pins SPI_CTRL/S_AXI1]
  connect_bd_intf_net -intf_net smartconnect_0_M08_AXI [get_bd_intf_pins smartconnect_0/M08_AXI] [get_bd_intf_pins SPI_CTRL/S_AXI2]
  connect_bd_intf_net -intf_net smartconnect_0_M09_AXI [get_bd_intf_pins smartconnect_0/M09_AXI] [get_bd_intf_pins SPI_CTRL/S_AXI3]
  connect_bd_intf_net -intf_net smartconnect_0_M10_AXI [get_bd_intf_pins SPI_CTRL/S_AXI] [get_bd_intf_pins smartconnect_0/M10_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M14_AXI [get_bd_intf_pins GT_PHY/AXI_LITE2] [get_bd_intf_pins smartconnect_0/M13_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M14_AXI1 [get_bd_intf_pins MP_bram_ctrl/S_AXI] [get_bd_intf_pins smartconnect_0/M14_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M15_AXI [get_bd_intf_pins GT_PHY/AXI_LITE1] [get_bd_intf_pins smartconnect_0/M12_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M15_AXI1 [get_bd_intf_pins PRVTDNN_bram_ctrl/S_AXI] [get_bd_intf_pins smartconnect_0/M15_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M16_AXI [get_bd_intf_pins smartconnect_0/M16_AXI] [get_bd_intf_pins PRVTDNNload_gpio/S_AXI]
  connect_bd_intf_net -intf_net util_ds_buf_2_CLK_OUT_D3 [get_bd_intf_ports RX_SYNC] [get_bd_intf_pins util_ds_buf_2/CLK_OUT_D3]
  connect_bd_intf_net -intf_net versal_cips_0_FPD_CCI_NOC_0 [get_bd_intf_pins versal_cips_0/FPD_CCI_NOC_0] [get_bd_intf_pins axi_noc_0/S00_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_FPD_CCI_NOC_1 [get_bd_intf_pins versal_cips_0/FPD_CCI_NOC_1] [get_bd_intf_pins axi_noc_0/S01_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_FPD_CCI_NOC_2 [get_bd_intf_pins versal_cips_0/FPD_CCI_NOC_2] [get_bd_intf_pins axi_noc_0/S02_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_FPD_CCI_NOC_3 [get_bd_intf_pins versal_cips_0/FPD_CCI_NOC_3] [get_bd_intf_pins axi_noc_0/S03_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_LPD_AXI_NOC_0 [get_bd_intf_pins versal_cips_0/LPD_AXI_NOC_0] [get_bd_intf_pins axi_noc_0/S04_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_M_AXI_FPD [get_bd_intf_pins versal_cips_0/M_AXI_FPD] [get_bd_intf_pins smartconnect_0/S00_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_PMC_NOC_AXI_0 [get_bd_intf_pins versal_cips_0/PMC_NOC_AXI_0] [get_bd_intf_pins axi_noc_0/S05_AXI]

  # Create port connections
  connect_bd_net -net AND_gt_pg_Res [get_bd_pins GT_PHY/gt_pd] [get_bd_pins jesd204c_0/gt_powergood] [get_bd_pins jesd204c_1/gt_powergood] [get_bd_pins jesd204c_2/gt_powergood]
  connect_bd_net -net AND_rplllock_logic_Res [get_bd_pins GT_PHY/rplllock] [get_bd_pins plllock_gpio/gpio_io_i]
  connect_bd_net -net DA_SDO_1 [get_bd_ports DACLK_SDI] [get_bd_pins SPI_CTRL/DA_SDI]
  connect_bd_net -net DPDenable_vio_probe [get_bd_pins DPD_en/probe_out0] [get_bd_pins inOutAct_0/vio_wdpd_i]
  connect_bd_net -net FMC_SPI_EN_probe_out0 [get_bd_pins OTHER_CTRL/probe_out0] [get_bd_ports FMC_SPI_EN]
  connect_bd_net -net FMC_SPI_EN_probe_out1 [get_bd_pins OTHER_CTRL/probe_out1] [get_bd_ports TXEN]
  connect_bd_net -net FMC_SPI_EN_probe_out2 [get_bd_pins OTHER_CTRL/probe_out2] [get_bd_ports AD_PWDN]
  connect_bd_net -net GT_REFCLK1_1 [get_bd_pins jesd_clock_ADC/refclk] [get_bd_pins GT_PHY/GT_REFCLK1]
  connect_bd_net -net Net2 [get_bd_ports DACLK_SDIO] [get_bd_pins SPI_CTRL/DA_SDIO]
  connect_bd_net -net PRVTDNN_bram_ctrl_bram_addr_a [get_bd_pins PRVTDNN_bram_ctrl/bram_addr_a] [get_bd_pins BRAM_FIFO/In_addr] [get_bd_pins axis_ila_BRAM/probe3]
  connect_bd_net -net PRVTDNN_bram_ctrl_bram_en_a [get_bd_pins PRVTDNN_bram_ctrl/bram_en_a] [get_bd_pins BRAM_FIFO/s_axis_tvalid] [get_bd_pins axis_ila_BRAM/probe5]
  connect_bd_net -net PRVTDNN_bram_ctrl_bram_wrdata_a [get_bd_pins PRVTDNN_bram_ctrl/bram_wrdata_a] [get_bd_pins BRAM_FIFO/In_data] [get_bd_pins axis_ila_BRAM/probe4]
  connect_bd_net -net RVTDNNTop_0_io_bramIF_rdData [get_bd_pins RVTDNNTop_0/io_bramIF_rdData] [get_bd_pins BRAM_FIFO/rddata]
  connect_bd_net -net RVTDNNTop_0_io_outAct_0 [get_bd_pins RVTDNNTop_0/io_outAct_0] [get_bd_pins inOutAct_0/outAct0] [get_bd_pins axis_ila_TX/probe6]
  connect_bd_net -net RVTDNNTop_0_io_outAct_1 [get_bd_pins RVTDNNTop_0/io_outAct_1] [get_bd_pins inOutAct_0/outAct1] [get_bd_pins axis_ila_TX/probe7]
  connect_bd_net -net RVTDNNTop_0_io_outAct_2 [get_bd_pins RVTDNNTop_0/io_outAct_2] [get_bd_pins inOutAct_0/outAct2]
  connect_bd_net -net RVTDNNTop_0_io_outAct_3 [get_bd_pins RVTDNNTop_0/io_outAct_3] [get_bd_pins inOutAct_0/outAct3]
  connect_bd_net -net RVTDNNTop_0_io_outAct_4 [get_bd_pins RVTDNNTop_0/io_outAct_4] [get_bd_pins inOutAct_0/outAct4]
  connect_bd_net -net RVTDNNTop_0_io_outAct_5 [get_bd_pins RVTDNNTop_0/io_outAct_5] [get_bd_pins inOutAct_0/outAct5]
  connect_bd_net -net RVTDNNTop_0_io_outAct_6 [get_bd_pins RVTDNNTop_0/io_outAct_6] [get_bd_pins inOutAct_0/outAct6]
  connect_bd_net -net RVTDNNTop_0_io_outAct_7 [get_bd_pins RVTDNNTop_0/io_outAct_7] [get_bd_pins inOutAct_0/outAct7]
  connect_bd_net -net SPI_CTRL_DA_CS [get_bd_pins SPI_CTRL/DA_CS] [get_bd_ports DA_CS]
  connect_bd_net -net SPI_CTRL_DA_SCLK [get_bd_pins SPI_CTRL/DA_SCLK] [get_bd_ports DACLK_SCLK]
  connect_bd_net -net SPI_CTRL_LMK_CS [get_bd_pins SPI_CTRL/CLK_CS] [get_bd_ports CLK_CS]
  connect_bd_net -net SPI_CTRL_gpio2_io_o_0 [get_bd_pins SPI_CTRL/AD_SCLK] [get_bd_ports AD_SCLK]
  connect_bd_net -net SPI_CTRL_gpio2_io_o_1 [get_bd_pins SPI_CTRL/AD_SDO] [get_bd_ports AD_SDO]
  connect_bd_net -net SPI_CTRL_gpio_io_o_0 [get_bd_pins SPI_CTRL/AD_CS] [get_bd_ports AD_CS]
  connect_bd_net -net TX2_GT_IP_Interface1_ch_txdata_1 [get_bd_pins jesd204c_1/ch2_txdata] [get_bd_pins GT_PHY/TX2_GT_IP_Interface1_ch_txdata] [get_bd_pins axis_ila_JESD/probe4]
  connect_bd_net -net TX3_GT_IP_Interface1_ch_txdata_1 [get_bd_pins jesd204c_1/ch3_txdata] [get_bd_pins GT_PHY/TX3_GT_IP_Interface1_ch_txdata] [get_bd_pins axis_ila_JESD/probe6]
  connect_bd_net -net axi_bram_ctrl_0_bram_addr_a [get_bd_pins MP_bram_ctrl/bram_addr_a] [get_bd_pins axis_ila_BRAM/probe0]
  connect_bd_net -net axi_bram_ctrl_0_bram_en_a [get_bd_pins MP_bram_ctrl/bram_en_a] [get_bd_pins axis_ila_BRAM/probe2]
  connect_bd_net -net axi_bram_ctrl_0_bram_wrdata_a [get_bd_pins MP_bram_ctrl/bram_wrdata_a] [get_bd_pins axis_ila_BRAM/probe1]
  connect_bd_net -net axi_dma_0_m_axis_mm2s_tdata [get_bd_pins axi_dma_0/m_axis_mm2s_tdata] [get_bd_pins tx_mapper_0/data_in]
  connect_bd_net -net axi_dma_0_m_axis_mm2s_tvalid [get_bd_pins axi_dma_0/m_axis_mm2s_tvalid] [get_bd_pins axis_ila_RX/probe6]
  connect_bd_net -net axi_dma_0_mm2s_introut [get_bd_pins axi_dma_0/mm2s_introut] [get_bd_pins versal_cips_0/pl_ps_irq8]
  connect_bd_net -net axi_dma_0_s2mm_introut [get_bd_pins axi_dma_0/s2mm_introut] [get_bd_pins versal_cips_0/pl_ps_irq9]
  connect_bd_net -net axi_gpio_0_gpio_io_o [get_bd_pins PRVTDNNload_gpio/gpio_io_o] [get_bd_pins BRAM_FIFO/load] [get_bd_pins RVTDNNTop_0/io_load]
  connect_bd_net -net axis_data_fifo_0_m_axis_tvalid [get_bd_pins BRAM_FIFO/BRAM_enable] [get_bd_pins RVTDNNTop_0/io_bramIF_wrEn]
  connect_bd_net -net axis_vio_0_probe_out0 [get_bd_pins axis_vio_0/probe_out0] [get_bd_pins loop_ctrl_0/index]
  connect_bd_net -net axis_vio_0_probe_out1 [get_bd_pins axis_vio_0/probe_out1] [get_bd_pins loop_ctrl_0/ri_sel]
  connect_bd_net -net axis_vio_0_probe_out2 [get_bd_pins axis_vio_0/probe_out2] [get_bd_pins loop_ctrl_0/switch_loop]
  connect_bd_net -net axis_vio_1_probe_out0 [get_bd_pins axis_vio_1/probe_out0] [get_bd_pins adc_to_axistream_0/length]
  connect_bd_net -net ch2_txusrclk_1 [get_bd_pins jesd_clock_ADC/coreclk] [get_bd_pins GT_PHY/ch2_txusrclk] [get_bd_pins jesd204c_2/rx_core_clk]
  connect_bd_net -net gpio_io_i_0_1 [get_bd_ports AD_SDI] [get_bd_pins SPI_CTRL/AD_SDI]
  connect_bd_net -net inOutAct_0_dpd_out [get_bd_pins inOutAct_0/dpd_out] [get_bd_pins jesd204c_0/tx_tdata]
  connect_bd_net -net inOutAct_0_inAct0 [get_bd_pins inOutAct_0/inAct0] [get_bd_pins RVTDNNTop_0/io_inAct_0]
  connect_bd_net -net inOutAct_0_inAct1 [get_bd_pins inOutAct_0/inAct1] [get_bd_pins RVTDNNTop_0/io_inAct_1]
  connect_bd_net -net inOutAct_0_inAct2 [get_bd_pins inOutAct_0/inAct2] [get_bd_pins RVTDNNTop_0/io_inAct_2]
  connect_bd_net -net inOutAct_0_inAct3 [get_bd_pins inOutAct_0/inAct3] [get_bd_pins RVTDNNTop_0/io_inAct_3]
  connect_bd_net -net inOutAct_0_inAct4 [get_bd_pins inOutAct_0/inAct4] [get_bd_pins RVTDNNTop_0/io_inAct_4]
  connect_bd_net -net inOutAct_0_inAct5 [get_bd_pins inOutAct_0/inAct5] [get_bd_pins RVTDNNTop_0/io_inAct_5]
  connect_bd_net -net inOutAct_0_inAct6 [get_bd_pins inOutAct_0/inAct6] [get_bd_pins RVTDNNTop_0/io_inAct_6]
  connect_bd_net -net inOutAct_0_inAct7 [get_bd_pins inOutAct_0/inAct7] [get_bd_pins RVTDNNTop_0/io_inAct_7]
  connect_bd_net -net jesd204c_0_gt_loopback [get_bd_pins jesd204c_0/gt_loopback] [get_bd_pins GT_PHY/ch1_loopback]
  connect_bd_net -net jesd204c_1_ch0_txctrl2 [get_bd_pins jesd204c_1/ch0_txctrl2] [get_bd_pins GT_PHY/TX0_GT_IP_Interface1_ch_txctrl2] [get_bd_pins axis_ila_JESD/probe1]
  connect_bd_net -net jesd204c_1_ch0_txdata [get_bd_pins jesd204c_1/ch0_txdata] [get_bd_pins GT_PHY/TX0_GT_IP_Interface1_ch_txdata] [get_bd_pins axis_ila_JESD/probe0]
  connect_bd_net -net jesd204c_1_ch1_txctrl2 [get_bd_pins jesd204c_1/ch1_txctrl2] [get_bd_pins GT_PHY/TX1_GT_IP_Interface1_ch_txctrl2] [get_bd_pins axis_ila_JESD/probe3]
  connect_bd_net -net jesd204c_1_ch1_txdata [get_bd_pins jesd204c_1/ch1_txdata] [get_bd_pins GT_PHY/TX1_GT_IP_Interface1_ch_txdata] [get_bd_pins axis_ila_JESD/probe2]
  connect_bd_net -net jesd204c_1_ch2_txctrl2 [get_bd_pins jesd204c_1/ch2_txctrl2] [get_bd_pins GT_PHY/TX2_GT_IP_Interface1_ch_txctrl2] [get_bd_pins axis_ila_JESD/probe5]
  connect_bd_net -net jesd204c_1_ch3_txctrl2 [get_bd_pins jesd204c_1/ch3_txctrl2] [get_bd_pins GT_PHY/TX3_GT_IP_Interface1_ch_txctrl2] [get_bd_pins axis_ila_JESD/probe7]
  connect_bd_net -net jesd204c_1_gt_loopback [get_bd_pins jesd204c_1/gt_loopback] [get_bd_pins GT_PHY/ch1_loopback1]
  connect_bd_net -net jesd204c_1_tx_sof [get_bd_pins jesd204c_1/tx_sof] [get_bd_pins axis_ila_JESD/probe8]
  connect_bd_net -net jesd204c_1_tx_somf [get_bd_pins jesd204c_1/tx_somf] [get_bd_pins axis_ila_JESD/probe9]
  connect_bd_net -net jesd204c_2_encommaalign [get_bd_pins jesd204c_2/encommaalign] [get_bd_pins GT_PHY/In9]
  connect_bd_net -net jesd204c_2_rx_sync [get_bd_pins jesd204c_2/rx_sync] [get_bd_pins axis_ila_RX/probe4] [get_bd_pins util_ds_buf_2/OBUF_IN]
  connect_bd_net -net jesd204c_2_rx_tdata [get_bd_pins jesd204c_2/rx_tdata] [get_bd_pins rx_mapper_0/data_in]
  connect_bd_net -net jesd204c_2_rx_tvalid [get_bd_pins jesd204c_2/rx_tvalid] [get_bd_pins adc_to_axistream_0/adc_data_valid_i]
  connect_bd_net -net jesd_clock_0_coreclk [get_bd_pins jesd_clock_DAC/coreclk] [get_bd_pins GT_PHY/ch1_txusrclk] [get_bd_pins BRAM_FIFO/m_axis_aclk] [get_bd_pins RVTDNNTop_0/clock] [get_bd_pins axi_dma_0/m_axi_sg_aclk] [get_bd_pins axi_dma_0/m_axi_mm2s_aclk] [get_bd_pins axi_dma_0/m_axi_s2mm_aclk] [get_bd_pins axis_ila_RX/clk] [get_bd_pins axis_ila_TX/clk] [get_bd_pins axis_ila_DMA/clk] [get_bd_pins axis_ila_JESD/clk] [get_bd_pins jesd204c_0/tx_core_clk] [get_bd_pins jesd204c_1/tx_core_clk] [get_bd_pins axi_noc_0/aclk6] [get_bd_pins axi_noc_0/aclk7] [get_bd_pins axi_noc_0/aclk8] [get_bd_pins loop_ctrl_0/clk] [get_bd_pins axis_vio_0/clk] [get_bd_pins adc_to_axistream_0/clk_i] [get_bd_pins axis_vio_1/clk]
  connect_bd_net -net jesd_clock_0_refclk [get_bd_pins jesd_clock_DAC/refclk] [get_bd_pins GT_PHY/GT_REFCLK0]
  connect_bd_net -net loop_ctrl_0_da_out [get_bd_pins loop_ctrl_0/da_out] [get_bd_pins inOutAct_0/dac_i]
  connect_bd_net -net loop_ctrl_0_sample0_i_ila [get_bd_pins loop_ctrl_0/sample0_i_ila] [get_bd_pins axis_ila_TX/probe1]
  connect_bd_net -net loop_ctrl_0_sample0_r_ila [get_bd_pins loop_ctrl_0/sample0_r_ila] [get_bd_pins axis_ila_TX/probe0]
  connect_bd_net -net refclk_pad_n_0_1 [get_bd_ports refclk_DAC_n] [get_bd_pins jesd_clock_DAC/refclk_pad_n]
  connect_bd_net -net refclk_pad_n_0_2 [get_bd_ports refclk_ADC_n] [get_bd_pins jesd_clock_ADC/refclk_pad_n]
  connect_bd_net -net refclk_pad_p_0_1 [get_bd_ports refclk_DAC_p] [get_bd_pins jesd_clock_DAC/refclk_pad_p]
  connect_bd_net -net refclk_pad_p_0_2 [get_bd_ports refclk_ADC_p] [get_bd_pins jesd_clock_ADC/refclk_pad_p]
  connect_bd_net -net rst_versal_cips_0_333M_peripheral_aresetn [get_bd_pins rst_versal_cips_0_333M/peripheral_aresetn] [get_bd_pins SPI_CTRL/s_axi_aresetn] [get_bd_pins BRAM_FIFO/s_axis_aresetn] [get_bd_pins MP_bram_ctrl/s_axi_aresetn] [get_bd_pins PRVTDNN_bram_ctrl/s_axi_aresetn] [get_bd_pins axi_dma_0/axi_resetn] [get_bd_pins plllock_gpio/s_axi_aresetn] [get_bd_pins PRVTDNNload_gpio/s_axi_aresetn] [get_bd_pins axis_ila_DMA/resetn] [get_bd_pins jesd204c_0/s_axi_aresetn] [get_bd_pins jesd204c_1/s_axi_aresetn] [get_bd_pins jesd204c_2/s_axi_aresetn] [get_bd_pins smartconnect_0/aresetn] [get_bd_pins loop_ctrl_0/rstn]
  connect_bd_net -net rst_versal_cips_0_333M_peripheral_reset [get_bd_pins rst_versal_cips_0_333M/peripheral_reset] [get_bd_pins RVTDNNTop_0/reset] [get_bd_pins adc_to_axistream_0/reset_i]
  connect_bd_net -net rx_mapper_0_data_out [get_bd_pins rx_mapper_0/data_out] [get_bd_pins loop_ctrl_0/ad_in] [get_bd_pins adc_to_axistream_0/adc_data_i]
  connect_bd_net -net rx_mapper_0_sample0_0_i [get_bd_pins rx_mapper_0/sample0_0_i] [get_bd_pins axis_ila_RX/probe1]
  connect_bd_net -net rx_mapper_0_sample0_0_r [get_bd_pins rx_mapper_0/sample0_0_r] [get_bd_pins axis_ila_RX/probe0]
  connect_bd_net -net rx_mapper_0_sample0_1_i [get_bd_pins rx_mapper_0/sample0_1_i] [get_bd_pins axis_ila_RX/probe3]
  connect_bd_net -net rx_mapper_0_sample0_1_r [get_bd_pins rx_mapper_0/sample0_1_r] [get_bd_pins axis_ila_RX/probe2]
  connect_bd_net -net tx_core_reset_0_1 [get_bd_ports RESET] [get_bd_pins jesd204c_0/tx_core_reset] [get_bd_pins jesd204c_1/tx_core_reset] [get_bd_pins jesd204c_2/rx_core_reset]
  connect_bd_net -net tx_mapper_0_dac2_sample0_ila [get_bd_pins tx_mapper_0/dac2_sample0_ila] [get_bd_pins axis_ila_TX/probe2]
  connect_bd_net -net tx_mapper_0_dac3_sample0_ila [get_bd_pins tx_mapper_0/dac3_sample0_ila] [get_bd_pins axis_ila_TX/probe3]
  connect_bd_net -net tx_mapper_0_data_in_ready [get_bd_pins tx_mapper_0/data_in_ready] [get_bd_pins axi_dma_0/m_axis_mm2s_tready]
  connect_bd_net -net tx_mapper_0_data_out0 [get_bd_pins tx_mapper_0/data_out0] [get_bd_pins loop_ctrl_0/da_ddr_in]
  connect_bd_net -net tx_mapper_0_data_out1 [get_bd_pins tx_mapper_0/data_out1] [get_bd_pins jesd204c_1/tx_tdata]
  connect_bd_net -net util_ds_buf_0_IBUF_OUT [get_bd_pins util_ds_buf_0/IBUF_OUT] [get_bd_pins axis_ila_JESD/probe10] [get_bd_pins jesd204c_1/tx_sync]
  connect_bd_net -net util_ds_buf_1_IBUF_OUT [get_bd_pins util_ds_buf_1/IBUF_OUT] [get_bd_pins jesd204c_0/tx_sync]
  connect_bd_net -net util_ds_buf_3_IBUF_OUT [get_bd_pins util_ds_buf_3/IBUF_OUT] [get_bd_pins jesd204c_0/tx_sysref] [get_bd_pins jesd204c_1/tx_sysref] [get_bd_pins jesd204c_2/rx_sysref]
  connect_bd_net -net versal_cips_0_fpd_cci_noc_axi0_clk [get_bd_pins versal_cips_0/fpd_cci_noc_axi0_clk] [get_bd_pins axi_noc_0/aclk0]
  connect_bd_net -net versal_cips_0_fpd_cci_noc_axi1_clk [get_bd_pins versal_cips_0/fpd_cci_noc_axi1_clk] [get_bd_pins axi_noc_0/aclk1]
  connect_bd_net -net versal_cips_0_fpd_cci_noc_axi2_clk [get_bd_pins versal_cips_0/fpd_cci_noc_axi2_clk] [get_bd_pins axi_noc_0/aclk2]
  connect_bd_net -net versal_cips_0_fpd_cci_noc_axi3_clk [get_bd_pins versal_cips_0/fpd_cci_noc_axi3_clk] [get_bd_pins axi_noc_0/aclk3]
  connect_bd_net -net versal_cips_0_lpd_axi_noc_clk [get_bd_pins versal_cips_0/lpd_axi_noc_clk] [get_bd_pins axi_noc_0/aclk4]
  connect_bd_net -net versal_cips_0_pl0_ref_clk [get_bd_pins versal_cips_0/pl0_ref_clk] [get_bd_pins rst_versal_cips_0_333M/slowest_sync_clk] [get_bd_pins SPI_CTRL/s_axi_aclk] [get_bd_pins GT_PHY/s_axi_lite_clk] [get_bd_pins BRAM_FIFO/s_axis_aclk] [get_bd_pins MP_bram_ctrl/s_axi_aclk] [get_bd_pins PRVTDNN_bram_ctrl/s_axi_aclk] [get_bd_pins axi_dma_0/s_axi_lite_aclk] [get_bd_pins plllock_gpio/s_axi_aclk] [get_bd_pins PRVTDNNload_gpio/s_axi_aclk] [get_bd_pins axis_ila_BRAM/clk] [get_bd_pins OTHER_CTRL/clk] [get_bd_pins DPD_en/clk] [get_bd_pins jesd204c_0/s_axi_aclk] [get_bd_pins jesd204c_1/s_axi_aclk] [get_bd_pins jesd204c_2/s_axi_aclk] [get_bd_pins smartconnect_0/aclk] [get_bd_pins versal_cips_0/m_axi_fpd_aclk]
  connect_bd_net -net versal_cips_0_pl0_resetn [get_bd_pins versal_cips_0/pl0_resetn] [get_bd_pins rst_versal_cips_0_333M/ext_reset_in]
  connect_bd_net -net versal_cips_0_pmc_axi_noc_axi0_clk [get_bd_pins versal_cips_0/pmc_axi_noc_axi0_clk] [get_bd_pins axi_noc_0/aclk5]
  connect_bd_net -net xlconstant_vcc_dout [get_bd_pins xlconstant_vcc/dout] [get_bd_pins adc_to_axistream_0/adc_capture_en_i]
  connect_bd_net -net xlslice_0_Dout [get_bd_pins BRAM_FIFO/BRAM_data] [get_bd_pins RVTDNNTop_0/io_bramIF_wrData]
  connect_bd_net -net xlslice_1_Dout [get_bd_pins BRAM_FIFO/BRAM_addr] [get_bd_pins RVTDNNTop_0/io_bramIF_wrAddr] [get_bd_pins RVTDNNTop_0/io_bramIF_rdAddr]

  # Create address segments
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_noc_0/S06_AXI/C1_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_noc_0/S07_AXI/C2_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces axi_dma_0/Data_SG] [get_bd_addr_segs axi_noc_0/S08_AXI/C3_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces versal_cips_0/FPD_CCI_NOC_0] [get_bd_addr_segs axi_noc_0/S00_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x000800000000 -range 0x000180000000 -target_address_space [get_bd_addr_spaces versal_cips_0/FPD_CCI_NOC_0] [get_bd_addr_segs axi_noc_0/S00_AXI/C0_DDR_LOW1] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces versal_cips_0/FPD_CCI_NOC_1] [get_bd_addr_segs axi_noc_0/S01_AXI/C1_DDR_LOW0] -force
  assign_bd_address -offset 0x000800000000 -range 0x000180000000 -target_address_space [get_bd_addr_spaces versal_cips_0/FPD_CCI_NOC_1] [get_bd_addr_segs axi_noc_0/S01_AXI/C1_DDR_LOW1] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces versal_cips_0/FPD_CCI_NOC_2] [get_bd_addr_segs axi_noc_0/S02_AXI/C2_DDR_LOW0] -force
  assign_bd_address -offset 0x000800000000 -range 0x000180000000 -target_address_space [get_bd_addr_spaces versal_cips_0/FPD_CCI_NOC_2] [get_bd_addr_segs axi_noc_0/S02_AXI/C2_DDR_LOW1] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces versal_cips_0/FPD_CCI_NOC_3] [get_bd_addr_segs axi_noc_0/S03_AXI/C3_DDR_LOW0] -force
  assign_bd_address -offset 0x000800000000 -range 0x000180000000 -target_address_space [get_bd_addr_spaces versal_cips_0/FPD_CCI_NOC_3] [get_bd_addr_segs axi_noc_0/S03_AXI/C3_DDR_LOW1] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces versal_cips_0/LPD_AXI_NOC_0] [get_bd_addr_segs axi_noc_0/S04_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x000800000000 -range 0x000180000000 -target_address_space [get_bd_addr_spaces versal_cips_0/LPD_AXI_NOC_0] [get_bd_addr_segs axi_noc_0/S04_AXI/C0_DDR_LOW1] -force
  assign_bd_address -offset 0xA4060000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs SPI_CTRL/AD_CS_SCLK_gpio/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4070000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs SPI_CTRL/AD_SDI_SDO_gpio/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4080000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs SPI_CTRL/CLK_CS_gpio/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4090000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs SPI_CTRL/DACLK_CS_SCLK_gpio/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40A0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs SPI_CTRL/DACLK_SDIO_SDI_gpio/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs MP_bram_ctrl/S_AXI/Mem0] -force
  assign_bd_address -offset 0xA4100000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs PRVTDNN_bram_ctrl/S_AXI/Mem0] -force
  assign_bd_address -offset 0xA4050000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs PRVTDNNload_gpio/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40B0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs axi_dma_0/S_AXI_LITE/Reg] -force
  assign_bd_address -offset 0xA4010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs GT_PHY/gt_quad_RX0/AXI_LITE/Reg] -force
  assign_bd_address -offset 0xA4020000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs GT_PHY/gt_quad_RX1/AXI_LITE/Reg] -force
  assign_bd_address -offset 0xA4030000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs GT_PHY/gt_quad_TX0/AXI_LITE/Reg] -force
  assign_bd_address -offset 0xA4040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs GT_PHY/gt_quad_TX1/AXI_LITE/Reg] -force
  assign_bd_address -offset 0xA40C0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs jesd204c_0/s_axi/Reg] -force
  assign_bd_address -offset 0xA40D0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs jesd204c_1/s_axi/Reg] -force
  assign_bd_address -offset 0xA40E0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs jesd204c_2/s_axi/Reg] -force
  assign_bd_address -offset 0xA40F0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces versal_cips_0/M_AXI_FPD] [get_bd_addr_segs plllock_gpio/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces versal_cips_0/PMC_NOC_AXI_0] [get_bd_addr_segs axi_noc_0/S05_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x000800000000 -range 0x000180000000 -target_address_space [get_bd_addr_spaces versal_cips_0/PMC_NOC_AXI_0] [get_bd_addr_segs axi_noc_0/S05_AXI/C0_DDR_LOW1] -force

  # Exclude Address Segments
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_noc_0/S06_AXI/C1_DDR_LOW1]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_noc_0/S07_AXI/C2_DDR_LOW1]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces axi_dma_0/Data_SG] [get_bd_addr_segs axi_noc_0/S08_AXI/C3_DDR_LOW1]


  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


common::send_gid_msg -ssname BD::TCL -id 2053 -severity "WARNING" "This Tcl script was generated from a block design that has not been validated. It is possible that design <$design_name> may result in errors during validation."

