This was the sequence generator made in systemverilog. It tested correctly on the verify script. 

```sv
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
```
For the test me section the 7 bit version required the XOR'd bits to be the 7th and 3rd bits of the register. the ammended version is below and worked in testing. 

```sv
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

```

