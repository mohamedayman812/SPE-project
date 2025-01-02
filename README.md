# SPE-project
This project implements a Logic Gate Simulator capable of evaluating logic circuits based on user input. It also includes profiling capabilities to track the performance of gate operations, such as execution counts and execution times.
How It Works

User Input:

The user enters a circuit description (e.g., AND 1 0 OR 1 1).

The program tokenizes the input into gates and variables.

Gate Simulation:

Each gate operation is executed based on the input.

Logical operations (e.g., AND, OR) are performed using helper functions.

Profiling:

Tracks how many times each gate is used.

Measures execution time for each gate operation.

Logs usage and timing data.

Output:

Displays the simulation result and profiling data.

Logs detailed profiling and gate usage in event_log.txt
