The first task was to just set up the counter which worked as expected. 

The first test yourself task was to alter the testbench code such that when the counter value was 0x9 the counter would pause for 3 clock cycles before returning to normal function. 

The alteration can be seen below. 

```c++
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
```

The second test yourself task involved inclusion of an asynchronous reset. 

The sensitivites now include the rest signal directly which means the always block can be triggered by either the clock or reset signals independently.

```sv
always_ff @ (posedge clk, posedge rst)
    if (rst) count <= {WIDTH{1'b0}};
    else count <= count + {{WIDTH-1{1'b0}}, en};
```
