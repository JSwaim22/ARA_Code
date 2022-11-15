# Branch Predictor Code Guide


sim_bp.cpp requires either three or four arguments to run.

The first argument sets the mode of the branch predictor and determines the total argument count.
There are two modes: bimodal and gshare.
Bimodal requires only three total arguments while gshare requires four.

Regardless of which mode is chosen, the final argument determines the text file to use.
The other arguments are integers that define certain parameters of the branch predictor.

Any of the text files that end in "_trace.txt" in the directory can be used.
These files are a series of adresses of branch instructions and an indictation of whether each branch was taken or not taken.

Below are two example branch predictor configurations:

./sim_bp bimodal 6 gcc_trace.txt

./sim_bp gshare 9 3 gcc_trace.txt

Running the program with these configurations produces the text files "val_bimodal_1.txt" and "val_gshare_1.txt".
