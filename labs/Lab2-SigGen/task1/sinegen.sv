module sinegen #(
    parameter ADDRESS_WIDTH = 8,
    parameter DATA_WIDTH = 8
) (
    input logic clk,
    input logic rst,
    input logic en,
    input logic [ADDRESS_WIDTH-1:0] incr,
    output logic [DATA_WIDTH-1:0] dout
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
    .dout(dout)
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

