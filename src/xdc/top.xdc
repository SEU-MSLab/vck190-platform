
#------------------------------------------
# TIMING CONSTRAINTS
#------------------------------------------
# Set Reference clock to 245.760011672974MHz by default
# GT的参考时钟频率必须在综合前指定
# 直接用下2行创建会导致methodology报warning TIMING-6
create_clock -period 4.069 -name jesd204c_DAC_refclk_pad [get_ports refclk_DAC_p]
create_clock -period 4.069 -name jesd204c_ADC_refclk_pad [get_ports refclk_ADC_p]
# 根据UG906，用下行应该是能解决警告，但是综合会失败，故暂且搁置
# create_clock -period 4.069 -name jesd204c_refclk_pad [get_ports {refclk_DAC_p refclk_ADC_p}]

# clk_pl_0在bd_70da_pspmc_0_0.xdc中定义，在Constraints Wizard里有显示
set_clock_groups -name async_refclk_plclk_DAC -asynchronous -group [get_clocks -include_generated_clocks jesd204c_DAC_refclk_pad] -group [get_clocks -include_generated_clocks clk_pl_0]
set_clock_groups -name async_refclk_plclk_ADC -asynchronous -group [get_clocks -include_generated_clocks jesd204c_ADC_refclk_pad] -group [get_clocks -include_generated_clocks clk_pl_0]
# set_clock_groups -name async_refclk_plclk -asynchronous -group [get_clocks -include_generated_clocks jesd204c_refclk_pad] -group [get_clocks -include_generated_clocks clk_pl_0]



# Versal Transceiver and RefClk location constraints.
## GTY_QUAD_X0Y3 GTY_QUAD_X0Y4 GTY_QUAD_X0Y5 GTY_QUAD_X0Y6 GTY_QUAD_X1Y0 GTY_QUAD_X1Y1 GTY_QUAD_X1Y2 GTY_QUAD_X1Y3 GTY_QUAD_X1Y4 GTY_QUAD_X1Y5 GTY_QUAD_X1Y6
## GTY_REFCLK_X0Y6 GTY_REFCLK_X0Y7 GTY_REFCLK_X0Y8 GTY_REFCLK_X0Y9 GTY_REFCLK_X0Y10 GTY_REFCLK_X0Y11 GTY_REFCLK_X0Y12 GTY_REFCLK_X0Y13 GTY_REFCLK_X1Y0 GTY_REFCLK_X1Y1 GTY_REFCLK_X1Y2 GTY_REFCLK_X1Y3 GTY_REFCLK_X1Y4 GTY_REFCLK_X1Y5 GTY_REFCLK_X1Y6 GTY_REFCLK_X1Y7 GTY_REFCLK_X1Y8 GTY_REFCLK_X1Y9 GTY_REFCLK_X1Y10 GTY_REFCLK_X1Y11 GTY_REFCLK_X1Y12 GTY_REFCLK_X1Y13
# 这个位置约束不确定是否是一定要加的，不确定是否会与后面的定义冲突，所以暂时注释
# set_property LOC GTY_QUAD_X0Y5 [get_cells -hierarchical -filter {NAME =~ */gt_quad_base_0/inst/quad_inst}]
# set_property LOC GTY_REFCLK_X0Y10 [get_cells -hierarchical -filter {NAME =~ *ibufds_refclk0}]
# set_property LOC GTY_QUAD_X0Y6 [get_cells -hierarchical -filter {NAME =~ */gt_quad_base_1/inst/quad_inst}]
# set_property LOC GTY_REFCLK_X0Y12 [get_cells -hierarchical -filter {NAME =~ *ibufds_refclk0}]

#-----------------------------

# 复位引脚是G36，连接SW5
set_property PACKAGE_PIN G36 [get_ports RESET]
set_property IOSTANDARD LVCMOS33 [get_ports RESET]

# M15和M14引脚是AD9173的refclk
set_property PACKAGE_PIN M15 [get_ports refclk_DAC_p]
# G13由8A34001产生参考时钟, Bank 204输入
set_property PACKAGE_PIN G13 [get_ports refclk_ADC_p]

# 溢出的LED引脚，DS6灯
set_property PACKAGE_PIN H34 [get_ports LED_OVF]
set_property IOSTANDARD LVCMOS33 [get_ports LED_OVF]

# AD9173评估板的SYSREF
set_property PACKAGE_PIN BD23 [get_ports {SYSREF_clk_p[0]}]
set_property IOSTANDARD LVDS15 [get_ports {SYSREF_clk_p[0]}]
# sysref_clk的引脚是XPIO（high Performance），所以内部加100欧姆电阻。VCCO供电是VADJ_FMC，也就是1.5V
set_property DIFF_TERM_ADV TERM_100 [get_ports {SYSREF_clk_p[0]}]

# tx_sync和rx_sync的引脚
set_property PACKAGE_PIN BC23 [get_ports {TX_SYNC0_clk_p[0]}]
set_property IOSTANDARD LVDS15 [get_ports {TX_SYNC0_clk_p[0]}]
set_property PACKAGE_PIN AW24 [get_ports {TX_SYNC1_clk_p[0]}]
set_property IOSTANDARD LVDS15 [get_ports {TX_SYNC1_clk_p[0]}]
set_property PACKAGE_PIN BF11 [get_ports {RX_SYNC_clk_p[0]}]
set_property IOSTANDARD LVDS15 [get_ports {RX_SYNC_clk_p[0]}]
# 只需要给接收端加100欧姆，AD6688的SYNC电路端已经有了100欧姆电阻，所以只需要给tx加
set_property DIFF_TERM_ADV TERM_100 [get_ports {TX_SYNC0_clk_p[0]}]
set_property DIFF_TERM_ADV TERM_100 [get_ports {TX_SYNC1_clk_p[0]}]

# 下面是AD9173和HMC7044的SPI接口定义
set_property PACKAGE_PIN AV21 [get_ports {DA_CS[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {DA_CS[0]}]
set_property PACKAGE_PIN BF24 [get_ports {CLK_CS[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {CLK_CS[0]}]
set_property PACKAGE_PIN AU21 [get_ports {DACLK_SDI[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {DACLK_SDI[0]}]
set_property PACKAGE_PIN AV22 [get_ports {DACLK_SCLK[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {DACLK_SCLK[0]}]
set_property PACKAGE_PIN AW21 [get_ports {DACLK_SDIO[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {DACLK_SDIO[0]}]

# 下面是AD6688的SPI接口定义
set_property PACKAGE_PIN AW13 [get_ports {AD_SDO[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {AD_SDO[0]}]
set_property PACKAGE_PIN BF14 [get_ports {AD_SDI[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {AD_SDI[0]}]
set_property PACKAGE_PIN AW12 [get_ports {AD_SCLK[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {AD_SCLK[0]}]
set_property PACKAGE_PIN AU12 [get_ports {AD_CS[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {AD_CS[0]}]

# 给AD9173的TXEN与AD6688的PWDN，还有拉低FMC_SPI_EN
set_property PACKAGE_PIN BC20 [get_ports {TXEN[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {TXEN[0]}]
set_property PACKAGE_PIN BD20 [get_ports {TXEN[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {TXEN[1]}]
set_property PACKAGE_PIN BG13 [get_ports {AD_PWDN}]
set_property IOSTANDARD LVCMOS15 [get_ports {AD_PWDN}]
set_property PACKAGE_PIN BG23 [get_ports {FMC_SPI_EN}]
set_property IOSTANDARD LVCMOS15 [get_ports {FMC_SPI_EN}]


# 下面是工具自动添加的，每个Quad只需要添加一行即足够了
# X1Yx->GTY 20x
set_property LOC GTY_QUAD_X1Y1 [get_cells design_1_i/GT_PHY/gt_quad_TX0/inst/quad_inst]
set_property LOC GTY_QUAD_X1Y2 [get_cells design_1_i/GT_PHY/gt_quad_TX1/inst/quad_inst]
set_property LOC GTY_QUAD_X1Y4 [get_cells design_1_i/GT_PHY/gt_quad_RX0/inst/quad_inst]
set_property LOC GTY_QUAD_X1Y5 [get_cells design_1_i/GT_PHY/gt_quad_RX1/inst/quad_inst]

# 见ug1387，参考时钟给了跨越时钟域的GT口
# set_property CLOCK_DEDICATED_ROUTE ANY_CMT_REGION [get_nets design_1_i/jesd_clock_0/inst/refclk]

# TODO:input和output delay
