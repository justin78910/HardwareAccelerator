`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/13/2021 01:51:31 PM
// Design Name: 
// Module Name: matrixTranspose
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module matrixTranspose(clk,reset,receiveIn,sendOut,intIn,intOut,speed); //supports up to 5x5 matrix
input clk,reset;

input [4:0] receiveIn;/*specify which element is being received -- ex: e20 is the element in row2,c0 which corresponds to receiveIn = 0 1 2 3 4
                                                                                                                                      5 6 7 8 9
                                                                                                                                     |10|
                       */
input [31:0] intIn;//integer element that is received from Vitis
input [4:0] sendOut;//Vitis will specify which element of the transposed matrix it wants to receive
output reg [ 31:0] intOut;//this is the selected integer element from the transposed matrix that will be sent out to Vitis
output reg [  9:0] speed; //determine the time it takes to run Vivado                     

reg [799:0] matIn,matOut;//transposed matrix output
reg READY_TO_TRANSPOSE; //all inputs received -> ready for matrix operations -- also start counting
reg stop_count;
//Step One: Receive Inputs
always@(receiveIn)
    begin
        case(receiveIn)//specify in Vitis
        5'b00001: matIn[799:768] = intIn;//1 
        5'b00010: matIn[767:736] = intIn;//2
        5'b00011: matIn[735:704] = intIn;//3
        5'b00100: matIn[703:672] = intIn;//4
        5'b00101: matIn[671:640] = intIn;//5
        5'b00110: matIn[639:608] = intIn;//6
        5'b00111: matIn[607:576] = intIn;//7
        5'b01000: matIn[575:544] = intIn;//8
        5'b01001: matIn[543:512] = intIn;//9
        5'b01010: matIn[511:480] = intIn;//10
        5'b01011: matIn[479:448] = intIn;//11
        5'b01100: matIn[447:416] = intIn;//12
        5'b01101: matIn[415:384] = intIn;//13
        5'b01110: matIn[383:352] = intIn;//14
        5'b01111: matIn[351:320] = intIn;//15
        5'b10000: matIn[319:288] = intIn;//16
        5'b10001: matIn[287:256] = intIn;//17
        5'b10010: matIn[255:224] = intIn;//18
        5'b10011: matIn[223:192] = intIn;//19
        5'b10100: matIn[191:160] = intIn;//20
        5'b10101: matIn[159:128] = intIn;//21
        5'b10110: matIn[127: 96] = intIn;//22
        5'b10111: matIn[ 95: 64] = intIn;//23
        5'b11000: matIn[ 63: 32] = intIn;//24
        5'b11001: begin matIn[ 31:  0] = intIn; READY_TO_TRANSPOSE = 1; end//25
        endcase
    end

//Step Two: Transpose
always@(READY_TO_TRANSPOSE)
    begin
        matOut = {matIn[799:768],matIn[639:608],matIn[479:448],matIn[319:288],matIn[159:128],
                  matIn[767:736],matIn[607:576],matIn[447:416],matIn[287:256],matIn[127: 96],
                  matIn[735:704],matIn[575:544],matIn[415:384],matIn[255:224],matIn[ 95: 64],
                  matIn[703:672],matIn[543:512],matIn[383:352],matIn[223:192],matIn[ 63: 32],
                  matIn[671:640],matIn[511:480],matIn[351:320],matIn[191:160],matIn[ 31:  0]};
    end

//Step Three: Send Out                  
always@(sendOut)
    begin
        case(sendOut)
            5'b00001: intOut = matOut[799:768];//1 
            5'b00010: intOut = matOut[767:736];//2
            5'b00011: intOut = matOut[735:704];//3
            5'b00100: intOut = matOut[703:672];//4
            5'b00101: intOut = matOut[671:640];//5
            5'b00110: intOut = matOut[639:608];//6
            5'b00111: intOut = matOut[607:576];//7
            5'b01000: intOut = matOut[575:544];//8
            5'b01001: intOut = matOut[543:512];//9
            5'b01010: intOut = matOut[511:480];//10
            5'b01011: intOut = matOut[479:448];//11
            5'b01100: intOut = matOut[447:416];//12
            5'b01101: intOut = matOut[415:384];//13
            5'b01110: intOut = matOut[383:352];//14
            5'b01111: intOut = matOut[351:320];//15
            5'b10000: intOut = matOut[319:288];//16
            5'b10001: intOut = matOut[287:256];//17
            5'b10010: intOut = matOut[255:224];//18
            5'b10011: intOut = matOut[223:192];//19
            5'b10100: intOut = matOut[191:160];//20
            5'b10101: intOut = matOut[159:128];//21
            5'b10110: intOut = matOut[127: 96];//22
            5'b10111: intOut = matOut[ 95: 64];//23
            5'b11000: intOut = matOut[ 63: 32];//24
            5'b11001: 
                begin
                intOut = matOut[ 31:  0];//25
                stop_count = 1;
                end
        endcase
    end
    
    always@(posedge clk,posedge reset)
        if(reset) speed <= 0; else
        if(stop_count) speed <= speed; else
        if(READY_TO_TRANSPOSE) speed <= speed + 1;
endmodule