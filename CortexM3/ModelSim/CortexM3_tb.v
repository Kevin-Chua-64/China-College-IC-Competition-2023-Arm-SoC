`timescale 1ps/1ps

module CortexM3_tb();

reg clk;
reg rstn;

CortexM3 #(
    .isSim(1),
    .isQuartus(1)
)   i1(
    .CLK50m(clk),
    .RSTn(rstn),
    .RXD(1'b0)
);

initial begin
    clk = 1;
    rstn = 0;
    #100
    rstn = 1;
end

always begin
    #10 clk = ~clk;
end

endmodule
