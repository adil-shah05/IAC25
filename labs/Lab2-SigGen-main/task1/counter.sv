module counter #(
    parameter WIDTH = 8
)(
    //interface signals
    input logic clk,
    input logic rst,
    input logic en,
    input logic [WIDTH-1:0] incr,
    output logic [WIDTH-1:0] count
);

always_ff @ (posedge clk)
    if (en) begin
        if (rst) count <= {WIDTH{1'b0}};
        else count <= count + incr;
    end

endmodule
