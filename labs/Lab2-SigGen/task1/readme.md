The task required making a custom top level module singen, the diagram was translated into the systemverilog below. 

```sv
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
```
The following cpp tesbtbench was also produced. 

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

        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++)
        {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }

        // ++++ Send sine wave value to Vbuddy
        vbdPlot(int(top->dout), 0, 255);
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

To complete the extension the frequency had to be determined by the vbdValue() function, unfortunately for large values of about 255 the samples skipped too many values and did not look like a coherent sine wave. I used a scaling factor of /10 which made it work for the full range of the rotary switch.

```cpp
top->incr = vbdValue() / 25;
```

This line was added inside the loop to achieve this effect. 

