module top #(
    parameter WIDTH = 16
) (
    input  logic             clk,      // clock 
    input  logic             rst,      // reset
    input  logic [WIDTH-1:0] N,        // clock divided by N+1
    output  logic [7:0] data_out
     
);

logic tick;
logic en;

assign en = tick;

clktick #(
    .WIDTH(WIDTH)
) clktick1 (
    .clk(clk),
    .rst(rst),
    .en(1),
    .N(N),
    .tick(tick)
);

f1_fsm f1 (
    .clk(clk),
    .rst(rst),
    .en(en),
    .data_out(data_out)
);

endmodule

