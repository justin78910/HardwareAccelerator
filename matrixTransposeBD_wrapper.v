//Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2020.2 (win64) Build 3064766 Wed Nov 18 09:12:45 MST 2020
//Date        : Sat May 15 15:19:07 2021
//Host        : MSI running 64-bit major release  (build 9200)
//Command     : generate_target matrixTransposeBD_wrapper.bd
//Design      : matrixTransposeBD_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module matrixTransposeBD_wrapper
   (clk,
    reset,
    usb_uart_0_rxd,
    usb_uart_0_txd);
  input clk;
  input reset;
  input usb_uart_0_rxd;
  output usb_uart_0_txd;

  wire clk;
  wire reset;
  wire usb_uart_0_rxd;
  wire usb_uart_0_txd;

  matrixTransposeBD matrixTransposeBD_i
       (.clk(clk),
        .reset(reset),
        .usb_uart_0_rxd(usb_uart_0_rxd),
        .usb_uart_0_txd(usb_uart_0_txd));
endmodule
