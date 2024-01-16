`ifndef ARCH
`define ARCH "VERSAL"
`endif
module jesd_clock (
    input  refclk_pad_n,
    input  refclk_pad_p,
    output refclk,

    output coreclk
);

  wire tied_to_ground_i;
  wire tied_to_vcc_i;
  wire refclk_i;
  wire coreclk_i;

  wire refclk_buf_i;
  wire refclk_copy;

  assign tied_to_ground_i = 1'b0;
  assign tied_to_vcc_i    = 1'b1;

  if(`ARCH == "ULTRASCALE") begin
    IBUFDS_GTE4 ibufds_refclk0 (
        .O    (refclk_i),
        .ODIV2(refclk_copy),
        .CEB  (tied_to_ground_i),
        .I    (refclk_pad_p),
        .IB   (refclk_pad_n)
    );

    BUFG_GT #(
        .SIM_DEVICE("ULTRASCALE_PLUS")
    ) refclk_bufg_gt_i (
      .I      (refclk_copy),
      .CE     (1'b1),
      .CEMASK (1'b1),
      .CLR    (1'b0),
      .CLRMASK(1'b1),
      .DIV    (3'b000),
      .O      (refclk_buf_i)
    );
  end else if(`ARCH == "VERSAL") begin
    IBUFDS_GTE5 ibufds_refclk0 (
        .O    (refclk_i),
        .ODIV2(refclk_copy),
        .CEB  (tied_to_ground_i),
        .I    (refclk_pad_p),
        .IB   (refclk_pad_n)
    );

    BUFG_GT #(
        .SIM_DEVICE("VERSAL_AI_CORE")
    ) refclk_bufg_gt_i (
        .I      (refclk_copy),
        .CE     (1'b1),
        .CEMASK (1'b1),
        .CLR    (1'b0),
        .CLRMASK(1'b1),
        .DIV    (3'b000),
        .O      (refclk_buf_i)
    );
  end

  assign refclk  = refclk_i;
  assign coreclk = refclk_buf_i;

endmodule

