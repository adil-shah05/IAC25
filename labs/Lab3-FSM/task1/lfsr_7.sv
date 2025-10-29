module lfsr(
    input   logic       clk,
    input   logic       rst,
    input   logic       en,
    output  logic [6:0] data_out
);
    logic [6:0] sreg;

    logic next;

    assign next = sreg[6] ^ sreg [2];
    assign data_out = sreg;

    always_ff @ (posedge clk, posedge rst) begin

        if (rst) sreg <= 7'b1;
        else if (en) sreg <= {sreg[5:0], next};
        else sreg <= sreg;

    end
endmodule

