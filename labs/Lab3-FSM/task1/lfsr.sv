module lfsr(
    input   logic       clk,
    input   logic       rst,
    input   logic       en,
    output  logic [3:0] data_out
);
    logic [3:0] sreg;

    logic next;

    assign next = sreg[3] ^ sreg [2];
    assign data_out = sreg;

    always_ff @ (posedge clk, posedge rst) begin

        if (rst) sreg <= 4'b0001;
        else if (en) sreg <= {sreg[2:0], next};
        else sreg <= sreg;

    end
endmodule

