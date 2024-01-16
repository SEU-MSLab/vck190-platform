module tx_mapper (
    input  [255:0] data_in,
    output         data_in_ready,

    output [127:0] data_out0,
    output [127:0] data_out1,

    output [15:0] dac0_sample0_ila,
    output [15:0] dac1_sample0_ila,
    output [15:0] dac2_sample0_ila,
    output [15:0] dac3_sample0_ila
);
  //    For DAC0
  wire [15:0] dac0_sample0;
  wire [15:0] dac0_sample1;
  wire [15:0] dac0_sample2;
  wire [15:0] dac0_sample3;

  //    For DAC1
  wire [15:0] dac1_sample0;
  wire [15:0] dac1_sample1;
  wire [15:0] dac1_sample2;
  wire [15:0] dac1_sample3;

  //    For DAC2
  wire [15:0] dac2_sample0;
  wire [15:0] dac2_sample1;
  wire [15:0] dac2_sample2;
  wire [15:0] dac2_sample3;

  //    For DAC3
  wire [15:0] dac3_sample0;
  wire [15:0] dac3_sample1;
  wire [15:0] dac3_sample2;
  wire [15:0] dac3_sample3;

  assign data_in_ready = 1'b1;

  assign dac0_sample0_ila = dac0_sample0;
  assign dac1_sample0_ila = dac1_sample0;
  assign dac2_sample0_ila = dac2_sample0;
  assign dac3_sample0_ila = dac3_sample0;

  assign dac0_sample0 = data_in[207:192];
  assign dac0_sample1 = data_in[223:208];
  assign dac0_sample2 = data_in[239:224];
  assign dac0_sample3 = data_in[255:240];

  assign dac1_sample0 = data_in[143:128];
  assign dac1_sample1 = data_in[159:144];
  assign dac1_sample2 = data_in[175:160];
  assign dac1_sample3 = data_in[191:176];

  assign dac2_sample0 = data_in[79:64];
  assign dac2_sample1 = data_in[95:80];
  assign dac2_sample2 = data_in[111:96];
  assign dac2_sample3 = data_in[127:112];


  assign dac3_sample0 = data_in[15:0];
  assign dac3_sample1 = data_in[31:16];
  assign dac3_sample2 = data_in[47:32];
  assign dac3_sample3 = data_in[63:48];



  wire [31:0] lane0_i;
  wire [31:0] lane1_i;
  wire [31:0] lane2_i;
  wire [31:0] lane3_i;
  wire [31:0] lane4_i;
  wire [31:0] lane5_i;
  wire [31:0] lane6_i;
  wire [31:0] lane7_i;

  // 先传输的放低位
  assign lane0_i = {dac0_sample3[15:8], dac0_sample2[15:8], dac0_sample1[15:8], dac0_sample0[15:8]};
  assign lane1_i = {dac0_sample3[7:0], dac0_sample2[7:0], dac0_sample1[7:0], dac0_sample0[7:0]};
  assign lane2_i = {dac1_sample3[15:8], dac1_sample2[15:8], dac1_sample1[15:8], dac1_sample0[15:8]};
  assign lane3_i = {dac1_sample3[7:0], dac1_sample2[7:0], dac1_sample1[7:0], dac1_sample0[7:0]};
  assign lane4_i = {dac2_sample3[15:8], dac2_sample2[15:8], dac2_sample1[15:8], dac2_sample0[15:8]};
  assign lane5_i = {dac2_sample3[7:0], dac2_sample2[7:0], dac2_sample1[7:0], dac2_sample0[7:0]};
  assign lane6_i = {dac3_sample3[15:8], dac3_sample2[15:8], dac3_sample1[15:8], dac3_sample0[15:8]};
  assign lane7_i = {dac3_sample3[7:0], dac3_sample2[7:0], dac3_sample1[7:0], dac3_sample0[7:0]};


  assign data_out0 = {lane3_i, lane2_i, lane1_i, lane0_i};  //I
  assign data_out1 = {lane7_i, lane6_i, lane5_i, lane4_i};  //Q
endmodule

