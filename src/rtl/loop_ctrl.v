// 因为只取了高128位，所以全是第二个ADC的数据
module loop_ctrl #(
    parameter AD_DATA_WIDTH = 256
) (
    input               clk,
    input               rstn,
    input  [255:0]      ad_in,
    input  [127:0]      da_ddr_in,

    output [127:0]      da_out,
    output reg [15:0]   sample0_r_ila,
    output reg [15:0]   sample0_i_ila,

    input [1:0]         index,          // Default: 0
    input               ri_sel,         // Default: 1
    input               switch_loop     // Default: 1
);
  reg [15:0] sample0_r, sample1_r, sample2_r, sample3_r;
  reg [15:0] sample0_i, sample1_i, sample2_i, sample3_i;
  reg [15:0] sample0_r_reg, sample1_r_reg, sample2_r_reg, sample3_r_reg;
  reg [15:0] sample0_i_reg, sample1_i_reg, sample2_i_reg, sample3_i_reg;


  always @(*) begin
    if (switch_loop) begin
      sample0_r_ila = sample0_r_reg;
      sample0_i_ila = sample0_i_reg;
    end else begin
      sample0_r_ila = {da_ddr_in[0+:8], da_ddr_in[32+:8]};
      sample0_i_ila = {da_ddr_in[64+:8], da_ddr_in[96+:8]};
    end
  end

  always @(posedge clk or negedge rstn) begin
    if (!rstn) begin
      sample0_r <= 'd0;
      sample1_r <= 'd0;
      sample2_r <= 'd0;
      sample3_r <= 'd0;
      sample0_i <= 'd0;
      sample1_i <= 'd0;
      sample2_i <= 'd0;
      sample3_i <= 'd0;
    end else begin
      // 参考rx_mapper的data_out赋值
      sample0_r <= ad_in[207:192];
      sample1_r <= ad_in[223:208];
      sample2_r <= ad_in[239:224];
      sample3_r <= ad_in[255:240];
      sample0_i <= ad_in[143:128];
      sample1_i <= ad_in[159:144];
      sample2_i <= ad_in[175:160];
      sample3_i <= ad_in[191:176];
    end
  end

  always @(posedge clk or negedge rstn)
    if (!rstn) begin
      sample0_r_reg <= 'd0;
      sample1_r_reg <= 'd0;
      sample2_r_reg <= 'd0;
      sample3_r_reg <= 'd0;
      sample0_i_reg <= 'd0;
      sample1_i_reg <= 'd0;
      sample2_i_reg <= 'd0;
      sample3_i_reg <= 'd0;
    end else begin
      case (index)
        2'd0: begin
          sample0_r_reg <= sample0_r;
          sample1_r_reg <= sample1_r;
          sample2_r_reg <= sample2_r;
          sample3_r_reg <= sample3_r;
          sample0_i_reg <= sample0_i;
          sample1_i_reg <= sample1_i;
          sample2_i_reg <= sample2_i;
          sample3_i_reg <= sample3_i;
        end
        2'd1: begin
          sample0_r_reg <= {sample0_r[15], sample0_r[15:1]};
          sample1_r_reg <= {sample1_r[15], sample1_r[15:1]};
          sample2_r_reg <= {sample2_r[15], sample2_r[15:1]};
          sample3_r_reg <= {sample3_r[15], sample3_r[15:1]};
          sample0_i_reg <= {sample0_i[15], sample0_i[15:1]};
          sample1_i_reg <= {sample1_i[15], sample1_i[15:1]};
          sample2_i_reg <= {sample1_i[15], sample2_i[15:1]};
          sample3_i_reg <= {sample3_i[15], sample3_i[15:1]};
        end
        2'd2: begin
          sample0_r_reg <= {{2{sample0_r[15]}}, sample0_r[15:2]};
          sample1_r_reg <= {{2{sample1_r[15]}}, sample1_r[15:2]};
          sample2_r_reg <= {{2{sample2_r[15]}}, sample2_r[15:2]};
          sample3_r_reg <= {{2{sample3_r[15]}}, sample3_r[15:2]};
          sample0_i_reg <= {{2{sample0_i[15]}}, sample0_i[15:2]};
          sample1_i_reg <= {{2{sample1_i[15]}}, sample1_i[15:2]};
          sample2_i_reg <= {{2{sample2_i[15]}}, sample2_i[15:2]};
          sample3_i_reg <= {{2{sample3_i[15]}}, sample3_i[15:2]};
        end
        2'd3: begin
          sample0_r_reg <= {{3{sample0_r[15]}}, sample0_r[15:3]};
          sample1_r_reg <= {{3{sample1_r[15]}}, sample1_r[15:3]};
          sample2_r_reg <= {{3{sample2_r[15]}}, sample2_r[15:3]};
          sample3_r_reg <= {{3{sample3_r[15]}}, sample3_r[15:3]};
          sample0_i_reg <= {{3{sample0_i[15]}}, sample0_i[15:3]};
          sample1_i_reg <= {{3{sample1_i[15]}}, sample1_i[15:3]};
          sample2_i_reg <= {{3{sample2_i[15]}}, sample2_i[15:3]};
          sample3_i_reg <= {{3{sample3_i[15]}}, sample3_i[15:3]};
        end
        default: begin
          sample0_r_reg <= 'd0;
          sample1_r_reg <= 'd0;
          sample2_r_reg <= 'd0;
          sample3_r_reg <= 'd0;
          sample0_i_reg <= 'd0;
          sample1_i_reg <= 'd0;
          sample2_i_reg <= 'd0;
          sample3_i_reg <= 'd0;
        end
      endcase
    end

  wire [31:0] lane0_i;
  wire [31:0] lane1_i;
  wire [31:0] lane2_i;
  wire [31:0] lane3_i;
  // 重新对数据做映射回去传给DAC，参考AD9173表17，先传的放低位
  assign lane0_i = {
    sample3_r_reg[15:8], sample2_r_reg[15:8], sample1_r_reg[15:8], sample0_r_reg[15:8]
  };
  assign lane1_i = {sample3_r_reg[7:0], sample2_r_reg[7:0], sample1_r_reg[7:0], sample0_r_reg[7:0]};
  assign lane2_i = {
    sample3_i_reg[15:8], sample2_i_reg[15:8], sample1_i_reg[15:8], sample0_i_reg[15:8]
  };
  assign lane3_i = {sample3_i_reg[7:0], sample2_i_reg[7:0], sample1_i_reg[7:0], sample0_i_reg[7:0]};

  reg [127:0] da_data_loop;
  always @(posedge clk or negedge rstn)
    if (!rstn) da_data_loop <= 'd0;
    else if (ri_sel)
      // 第一条lane放低32位
      da_data_loop <= {
        lane3_i, lane2_i, lane1_i, lane0_i
      };
    else da_data_loop <= {lane1_i, lane0_i, lane3_i, lane2_i};
  assign da_out = switch_loop ? da_data_loop : da_ddr_in;
endmodule
