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
        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++)
        {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }

        // ++++ Send sine wave value to Vbuddy
        vbdPlot(int(top->dout), 0, 255); // Plot the sine output
        vbdCycle(i + 1);
        // ---- end of Vbuddy output section

        // change input stimuli
        top->rst = (i < 2);
        top->en = 1; // Keep enabled, or use vbdFlag() if you want control
        // top->incr = vbdValue();  // Optional: control frequency with rotary encoder

        if ((Verilated::gotFinish()) || (vbdGetkey() == 'q'))
            exit(0);
    }

    vbdClose(); // ++++
    tfp->close();
    exit(0);
}