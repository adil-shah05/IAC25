module sigdelay #(
    parameter ADDRESS_WIDTH = 9,
    parameter DATA_WIDTH = 8
) (
    input logic clk,
    input logic rst,
    input logic en,
    input logic rd_en,
    input logic wr_en,
    input logic incr,
    input logic [ADDRESS_WIDTH-1:0] offset,
    input logic [DATA_WIDTH-1:0] din,
    output logic [DATA_WIDTH-1:0] dout
);

logic [ADDRESS_WIDTH-1:0] count;
logic [ADDRESS_WIDTH-1:0] rd_addr;
logic [ADDRESS_WIDTH-1:0] wr_addr;

assign wr_addr = count;
assign rd_addr = count + offset;

ram #(
    .ADDRESS_WIDTH(ADDRESS_WIDTH),
    .DATA_WIDTH(DATA_WIDTH)
) ram1 (
    .clk(clk),
    .wr_en(wr_en),
    .rd_en(rd_en),
    .wr_addr(wr_addr),
    .rd_addr(rd_addr),
    .din(din),
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