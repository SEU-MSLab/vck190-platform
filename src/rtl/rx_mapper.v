// 命名规则统一为sample<样本编号>_<转换器编号>
// AD6688和AD9173的data structure是一样的
// 根据AD9173手册表17，每条lane是32位接口可以装下4个sample，M-4，则一个core clock传输16个sample
module rx_mapper (
    input  [255:0] data_in,
    output [255:0] data_out,
    output [ 15:0] sample0_0_r,
    output [ 15:0] sample1_0_r,
    output [ 15:0] sample2_0_r,
    output [ 15:0] sample3_0_r,

    output [15:0] sample0_0_i,
    output [15:0] sample1_0_i,
    output [15:0] sample2_0_i,
    output [15:0] sample3_0_i,

    output [15:0] sample0_1_r,
    output [15:0] sample1_1_r,
    output [15:0] sample2_1_r,
    output [15:0] sample3_1_r,

    output [15:0] sample0_1_i,
    output [15:0] sample1_1_i,
    output [15:0] sample2_1_i,
    output [15:0] sample3_1_i

);

  wire [31:0] lane0_i;
  wire [31:0] lane1_i;
  wire [31:0] lane2_i;
  wire [31:0] lane3_i;

  wire [31:0] lane4_i;
  wire [31:0] lane5_i;
  wire [31:0] lane6_i;
  wire [31:0] lane7_i;

  // 数据接口是32位的，所以每条lane有32位，总共8*32=256位
  // 根据pg066 Figure 3-6，data_in的低32位是lane0，依次排下
  // adc 0
  assign lane0_i = data_in[31:0];  // H
  assign lane1_i = data_in[63:32];  // L
  assign lane2_i = data_in[95:64];  // H
  assign lane3_i = data_in[127:96];  // L

  //adc 1
  assign lane4_i = data_in[159:128];  // H
  assign lane5_i = data_in[191:160];  // L
  assign lane6_i = data_in[223:192];  // H
  assign lane7_i = data_in[255:224];  // L

  wire [31:0] sample0_0;  // 这里转换器编号是ADC 0和ADC 1
  wire [31:0] sample1_0;
  wire [31:0] sample2_0;
  wire [31:0] sample3_0;

  wire [31:0] sample0_1;
  wire [31:0] sample1_1;
  wire [31:0] sample2_1;
  wire [31:0] sample3_1;

  assign sample0_0 = {lane0_i[7:0], lane1_i[7:0], lane2_i[7:0], lane3_i[7:0]};
  assign sample1_0 = {lane0_i[15:8], lane1_i[15:8], lane2_i[15:8], lane3_i[15:8]};
  assign sample2_0 = {lane0_i[23:16], lane1_i[23:16], lane2_i[23:16], lane3_i[23:16]};
  assign sample3_0 = {lane0_i[31:24], lane1_i[31:24], lane2_i[31:24], lane3_i[31:24]};

  assign sample0_1 = {lane4_i[7:0], lane5_i[7:0], lane6_i[7:0], lane7_i[7:0]};
  assign sample1_1 = {lane4_i[15:8], lane5_i[15:8], lane6_i[15:8], lane7_i[15:8]};
  assign sample2_1 = {lane4_i[23:16], lane5_i[23:16], lane6_i[23:16], lane7_i[23:16]};
  assign sample3_1 = {lane4_i[31:24], lane5_i[31:24], lane6_i[31:24], lane7_i[31:24]};

  // 输出重新组合了一下，原本每条lane传的都只是半个样本
  // 这里怎么映射就很随意了，应用层的格式自己规定
  assign data_out = {
    sample3_0_r,
    sample2_0_r,
    sample1_0_r,
    sample0_0_r,
    sample3_0_i,
    sample2_0_i,
    sample1_0_i,
    sample0_0_i,
    sample3_1_r,
    sample2_1_r,
    sample1_1_r,
    sample0_1_r,
    sample3_1_i,
    sample2_1_i,
    sample1_1_i,
    sample0_1_i
  };
  assign sample0_0_r = sample0_0[31:16];  // 第一个转换器的第一个样本
  assign sample1_0_r = sample1_0[31:16];
  assign sample2_0_r = sample2_0[31:16];
  assign sample3_0_r = sample3_0[31:16];

  assign sample0_0_i = sample0_0[15:0];
  assign sample1_0_i = sample1_0[15:0];
  assign sample2_0_i = sample2_0[15:0];
  assign sample3_0_i = sample3_0[15:0];

  assign sample0_1_r = sample0_1[31:16];  // 第二个转换器的第一个样本
  assign sample1_1_r = sample1_1[31:16];
  assign sample2_1_r = sample2_1[31:16];
  assign sample3_1_r = sample3_1[31:16];

  assign sample0_1_i = sample0_1[15:0];
  assign sample1_1_i = sample1_1[15:0];
  assign sample2_1_i = sample2_1[15:0];
  assign sample3_1_i = sample3_1[15:0];
endmodule
