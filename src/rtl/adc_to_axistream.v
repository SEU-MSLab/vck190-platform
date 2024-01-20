// 代码来源于https://mp.weixin.qq.com/s/o7AfM_9JmmhVgL6vWvMPfw
// 米联客裸机CH28 利用LWIP实现ADC DAQ7606数据采集教程
// 进行了部分修改
`include "defines.sv"
module adc_to_axistream #(
    parameter ADC_DWIDTH = 256
) (
    input                  clk_i,
    input                  reset_i,
    input                  adc_capture_en_i,  //使能转换信号
    input [ADC_DWIDTH-1:0] adc_data_i,
    input                  adc_data_valid_i,

    input                          m_axis_adc_tready,
    output reg                     m_axis_adc_tvalid,
    output reg [   ADC_DWIDTH-1:0] m_axis_adc_tdata,
    output     [ADC_DWIDTH/8 -1:0] m_axis_adc_tkeep,
    output reg                     m_axis_adc_tlast,

    // 默认length是0x1FFFF，传输2 * 2,097,152字节数据
    // 最大值可传输4GB数据，但DMA的BD做不到，且DDR也装不满
    input       [31:0]             length
);


  wire [31:0] length;



  reg [ADC_DWIDTH-1:0] adc_data_reg;
  reg                  adc_data_valid_reg;
  reg                  adc_capture_en_reg;

  reg [          31:0] data_cnt = 'd0;  // 记录多少数据拉高tlast
  reg [           1:0] state;


  assign m_axis_adc_tkeep = -1;  // 赋全1



  // 本以为不打拍的话时序是过不了的，但实际可能是复位问题
  always @(posedge clk_i) begin
    adc_data_reg       <= adc_data_i;
    adc_data_valid_reg <= adc_data_valid_i;
    adc_capture_en_reg <= adc_capture_en_i;
  end

  // 原代码用的就是同步高复位，但因为reset_i和clk_i是异步时钟源
  // 但改成异步复位也不能解决时序问题
  always @(posedge clk_i) begin
    if (reset_i) begin
      m_axis_adc_tvalid <= 1'b0;
      m_axis_adc_tlast  <= 1'b0;
      state             <= 0;
    end else begin
      case (state)
        0: begin
          if (adc_capture_en_reg && adc_data_valid_reg && m_axis_adc_tready) begin
            m_axis_adc_tvalid <= 1'b1;
            state             <= 1;
          end else begin
            m_axis_adc_tvalid <= 1'b0;
            state             <= 0;
          end
        end
        1: begin
          if (m_axis_adc_tready && adc_data_valid_reg) begin
            m_axis_adc_tvalid <= 1'b1;
            if (data_cnt == length) begin
              m_axis_adc_tlast <= 1'b1;
              state            <= 2;
            end else begin
              m_axis_adc_tlast <= 1'b0;
              state            <= 1;
            end
          end else begin
            m_axis_adc_tlast  <= 1'b0;
            m_axis_adc_tvalid <= 1'b0;
            state             <= 1;
          end
        end
        2: begin
          if (!m_axis_adc_tready) begin
            // 如果tready没拉高，从机可能没有检测到tlast
            m_axis_adc_tvalid <= 1'b1;
            m_axis_adc_tlast  <= 1'b1;
            state             <= 2;
          end else begin
            // 在进入该状态时检测到从机的ready信号为1说明数据和last信号已经成功传递给从机
            // 所以可以直接将master的valid信号拉低，返回到idle状态，等到下一次的触发条件满足即可
            m_axis_adc_tvalid <= 1'b0;
            m_axis_adc_tlast  <= 1'b0;
            state             <= 0;
          end
        end
        default: state <= 0;
      endcase
    end
  end

  // 一开始，m_axis_adc_tdata的逻辑是和状态机放一起的，并且用了复位
  // 但是这样不行，即使注释复位也不行，因为会让时钟连接到触发器CE端口
  // 导致时序slack过不了
  // 具体见UG949第54页 Issues When Trying to Eliminate Reset in HDL Code
  // 因此额外用always来写
  always @(posedge clk_i) begin
    case (state)
      0: begin
        if (adc_capture_en_reg && adc_data_valid_reg && m_axis_adc_tready) begin
          m_axis_adc_tdata <= adc_data_reg;
        end else begin
          m_axis_adc_tdata <= 'b0;
        end
      end
      1: begin
        if (m_axis_adc_tready && adc_data_valid_reg) begin
          m_axis_adc_tdata <= adc_data_reg;
          data_cnt         <= data_cnt + 1'b1;
        end else begin
          m_axis_adc_tdata <= m_axis_adc_tdata;
          data_cnt         <= data_cnt;
        end
      end
      2: begin
        if (!m_axis_adc_tready) begin
          m_axis_adc_tdata <= m_axis_adc_tdata;
          data_cnt         <= data_cnt;
        end else begin
          m_axis_adc_tdata <= 'b0;
          data_cnt         <= 'd0;
        end
      end
      default: m_axis_adc_tdata <= 'b0;
    endcase
  end


endmodule
