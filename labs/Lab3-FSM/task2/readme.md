The F1 FSM was implemented by altering the shift register design. It detects the 'end' of the chain by AND-ing the MSB of the state register and the enable signal. This acts as an interrupt that resets the signal to the 0 state. 

```sv
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
```
The testbnch was implemented as follows. 


```cpp
#include "Vf1_fsm.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vf1_fsm *top = new Vf1_fsm;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_fsm.vcd");

    // init Vbuddy
    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("Lab 3: FSM");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 1;

    // run simulation for many clock cycles
    for (i = 0; i < 1000000; i++)
    {
        // Set inputs BEFORE clock edges
        top->rst = (i < 2);
        top->en = vbdFlag();

        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++)
        {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }

        // ++++ Send sine wave value to Vbuddy
        vbdBar(top->data_out & 0xFF);
        vbdCycle(i + 1);
        // ---- end of Vbuddy output section

        if (Verilated::gotFinish())
        {
            exit(0);
        }
    }

    vbdClose();
    tfp->close();
    exit(0);
}
```