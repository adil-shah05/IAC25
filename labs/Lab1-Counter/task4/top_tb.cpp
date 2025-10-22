#include "Vtop.h" // Changed from "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp" // Add vbuddy

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vtop *top = new Vtop; // Changed from Vcounter to Vtop
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("top.vcd"); // Changed to top.vcd

    // init Vbuddy
    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("Lab 1: BCD Counter");

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

        // Send BCD value to Vbuddy display
        vbdHex(4, (int(top->bcd) >> 12) & 0xF); // Not used (only 3 digits)
        vbdHex(3, (int(top->bcd) >> 8) & 0xF);  // Hundreds digit
        vbdHex(2, (int(top->bcd) >> 4) & 0xF);  // Tens digit
        vbdHex(1, int(top->bcd) & 0xF);         // Ones digit
        vbdCycle(i + 1);

        // Reset only at the beginning
        top->rst = (i < 2);

        // Check if BCD reached 9 for the first time
        // Note: top->bcd instead of top->count
        if ((top->bcd & 0xF) == 0x9 && !stopped)
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

    vbdClose(); // Close Vbuddy
    tfp->close();
    exit(0);
}