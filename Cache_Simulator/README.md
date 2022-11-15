
# Cache Simulator Code Guide

sim.cpp requires eight arguments to run. The first seven are integers followed by the file name of a text file in the same directory.

Any of the text files that end in "_trace.txt" in the directory can be used. These text files are a series of read and write requests to the cache simulator with an address.

Below is an example simulator configuration:

./sim.cpp 16 2024 64 0 0 0 0 gcc_trace.txt

Running the program with this configuration produces the text file "output.txt".
