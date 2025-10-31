#include "Vf1_fsm.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab1: Counter");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;
    top->v = 5;

    vbdSetMode(1);

    // run simulation for many clock cycles
    for (i=0; i<10000; i++) {

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++) {
            tfp->dump (2*i+clk);        // unit is in ps!!!
            top->clk = !top->clk;
            top->eval ();
        }

        top->rst = 0
        //top->en = (i>4);
        top->en = vbdFlag();
        vbdBar(top->data_out & 0xFF);

        // vbdHex(4, (int(top->count) >> 16) & 0XF);
        // vbdHex(3, (int(top->count) >> 8) & 0XF);
        // vbdHex(2, (int(top->count) >> 4) & 0XF);
        // vbdHex(1, int(top->count) & 0XF);
        //vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);


        if (Verilated::gotFinish())  exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}