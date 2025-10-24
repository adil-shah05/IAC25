module sinegen #(
    parameter ADDRESS_WIDTH = 8,
    parameter DATA_WIDTH = 8
) (
    input logic clk,
    input logic rst,
    input logic en,
    input logic [ADDRESS_WIDTH-1:0] incr,
    input logic [ADDRESS_WIDTH-1:0] offset,
    output logic [DATA_WIDTH-1:0] dout1,
    output logic [DATA_WIDTH-1:0] dout2
);

logic [ADDRESS_WIDTH-1:0] count;
logic [ADDRESS_WIDTH-1:0] addr;

assign addr = count;

rom #(
    .ADDRESS_WIDTH(ADDRESS_WIDTH),
    .DATA_WIDTH(DATA_WIDTH)
) rom1 (
    .clk(clk),
    .addr(addr),
    .dout1(dout1),
    .dout2(dout2),
    .offset(offset)
);

counter #(
    .WIDTH(ADDRESS_WIDTH)
) counter1 (
    .clk(clk),
    .rst(rst),
    .en(en),
    .incr(incr),
    .count(count)
);

    
endmodule

