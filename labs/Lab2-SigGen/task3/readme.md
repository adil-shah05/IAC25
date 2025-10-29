To allow for two data ouputs from the ROM the sv file was changed.

```sv
module rom #(
    parameter ADDRESS_WIDTH = 8,
              DATA_WIDTH = 8
)(
    input logic                      clk,
    input logic [ADDRESS_WIDTH-1:0] addr,
    input logic [ADDRESS_WIDTH-1:0] offset, // decides the address for output port 2
    output logic [DATA_WIDTH-1:0]    dout1, // 2 output ports
    output logic [DATA_WIDTH-1:0]    dout2

);

logic [DATA_WIDTH-1:0] rom_array [2**ADDRESS_WIDTH-1:0];

initial begin
    $display("Loading rom.");
    $readmemh("sinerom.mem", rom_array);
end

always_ff @(posedge clk) begin
    // output is synchronous
    dout1 <= rom_array [addr];
    dout2 <= rom_array [addr + offset];
end

endmodule
```

The testbench was also changed. 

```cpp
#include "Vsinegen.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vsinegen *top = new Vsinegen;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("sinegen.vcd");

    // init Vbuddy
    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("Lab 2: SigGen");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 1;
    top->incr = 1;

    // run simulation for many clock cycles
    for (i = 0; i < 1000000; i++)
    {
        // Set inputs BEFORE clock edges
        top->rst = (i < 2);
        top->en = 1;
        top->offset = vbdValue();

        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++)
        {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }

        // ++++ Send sine wave value to Vbuddy
        vbdPlot(int(top->dout1), 0, 255);
        vbdPlot(int(top->dout2), 0, 255);
        vbdCycle(i + 1);
        // ---- end of Vbuddy output section

        if ((Verilated::gotFinish()) || (vbdGetkey() == 'q'))
            exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
```

