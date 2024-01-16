// For PRVTDNN etc.
module inOutAct (
    input  [127:0] dac_i,
    output [127:0] dpd_out,

    output [15:0] inAct0,
    output [15:0] inAct1,
    output [15:0] inAct2,
    output [15:0] inAct3,
    output [15:0] inAct4,
    output [15:0] inAct5,
    output [15:0] inAct6,
    output [15:0] inAct7,

    input [15:0] outAct0,
    input [15:0] outAct1,
    input [15:0] outAct2,
    input [15:0] outAct3,
    input [15:0] outAct4,
    input [15:0] outAct5,
    input [15:0] outAct6,
    input [15:0] outAct7,

    input vio_wdpd_i
);

  assign inAct0 = {dac_i[24+:8], dac_i[56+:8]};
  assign inAct1 = {dac_i[88+:8], dac_i[120+:8]};
  assign inAct2 = {dac_i[16+:8], dac_i[48+:8]};
  assign inAct3 = {dac_i[80+:8], dac_i[112+:8]};
  assign inAct4 = {dac_i[8+:8], dac_i[40+:8]};
  assign inAct5 = {dac_i[72+:8], dac_i[104+:8]};
  assign inAct6 = {dac_i[0+:8], dac_i[32+:8]};
  assign inAct7 = {dac_i[64+:8], dac_i[96+:8]};



  wire [31:0] lane0;
  wire [31:0] lane1;
  wire [31:0] lane2;
  wire [31:0] lane3;

  assign lane0   = {outAct1[15:8], outAct3[15:8], outAct5[15:8], outAct7[15:8]};
  assign lane1   = {outAct1[7:0], outAct3[7:0], outAct5[7:0], outAct7[7:0]};
  assign lane2   = {outAct0[15:8], outAct2[15:8], outAct4[15:8], outAct6[15:8]};
  assign lane3   = {outAct0[7:0], outAct2[7:0], outAct4[7:0], outAct6[7:0]};

  assign dpd_out = vio_wdpd_i ? {lane3, lane2, lane1, lane0} : dac_i;


endmodule

