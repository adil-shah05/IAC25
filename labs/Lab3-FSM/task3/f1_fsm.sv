module f1_fsm (
    input   logic       rst,
    input   logic       en,
    input   logic       clk,
    output  logic [7:0] data_out
);

    logic [7:0] sreg;
;
    assign data_out = sreg;

    logic finish;

    assign finish = sreg[7] && en;

    always_ff @ (posedge clk, posedge rst) begin

        if (rst) sreg <= 8'b0;
        else if (finish) sreg <= 8'b0;
        else if (en) sreg <= {sreg[6:0], en};
        else sreg <= sreg;

    end

endmodule
