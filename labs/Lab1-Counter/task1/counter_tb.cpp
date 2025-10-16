#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter *top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    int stopclk = 0;      // counter for stopping cycles
    bool stopped = false; // flag to ensure we only stop once

    for (i = 0; i < 300; i++)
    {

        for (clk = 0; clk < 2; clk++)
        {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }

        // Reset only at the beginning
        top->rst = (i < 2);

        // Check if counter reached 0x9 for the first time
        if (top->count == 0x9 && !stopped)
        {
            stopclk = 3;    // stop for 3 cycles
            stopped = true; // mark that we've stopped
        }

        // Handle enable signal
        if (stopclk > 0)
        {
            top->en = 0; // disable counting during pause
            stopclk--;
        }
        else
        {
            top->en = (i > 4); // enable after initial reset
        }

        if (Verilated::gotFinish())
            exit(0);
    }
    tfp->close();
    exit(0);
}