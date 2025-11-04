This was the top level sheet that connected the counter to the RAM module.The RAM module was the same as the one shown in the lecture slides for memories.

```sv
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
```
The testbench used can also be seen below.

```cpp
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vsigdelay.h"

#include "vbuddy.cpp" // include vbuddy code
#define MAX_SIM_CYC 1000000
#define ADDRESS_WIDTH 9
#define RAM_SZ pow(2, ADDRESS_WIDTH)

int main(int argc, char **argv, char **env)
{
  int simcyc; // simulation clock count
  int tick;   // each clk cycle has two ticks for two edges

  Verilated::commandArgs(argc, argv);
  // init top verilog instance
  Vsigdelay *top = new Vsigdelay;
  // init trace dump
  Verilated::traceEverOn(true);
  VerilatedVcdC *tfp = new VerilatedVcdC;
  top->trace(tfp, 99);
  tfp->open("sigdelay.vcd");

  // init Vbuddy
  if (vbdOpen() != 1)
    return (-1);
  vbdHeader("L2T3:Delay");
  // vbdSetMode(1);        // Flag mode set to one-shot

  // initialize simulation input
  top->clk = 1;
  top->rst = 1;
  top->wr_en = 1;
  top->rd_en = 1;
  top->en = 1;
  top->incr = 1;
  top->offset = 64;

  // intialize variables for analogue output
  vbdInitMicIn(RAM_SZ);

  // run simulation for MAX_SIM_CYC clock cycles
  for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
  {
    // dump variables into VCD file and toggle clock
    for (tick = 0; tick < 2; tick++)
    {
      tfp->dump(2 * simcyc + tick);
      top->clk = !top->clk;
      top->eval();
    }

    if (simcyc == 1)
      top->rst = 0; // release reset after first cycle

    top->din = vbdMicValue();
    top->offset = abs(vbdValue()); // adjust delay by changing incr

    // plot RAM input/output, send sample to DAC buffer, and print cycle count
    vbdPlot(int(top->din), 0, 255);
    vbdPlot(int(top->dout), 0, 255);
    vbdCycle(simcyc);

    // either simulation finished, or 'q' is pressed
    if ((Verilated::gotFinish()) || (vbdGetkey() == 'q'))
      exit(0);
  }

  vbdClose(); // ++++
  tfp->close();
  printf("Exiting\n");
  exit(0);
}
```