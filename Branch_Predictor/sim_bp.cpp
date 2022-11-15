#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Constant Values
#define bimodal                 0
#define gshare                  1

// Classes
class Counter {
    private:
        // Attributes
        unsigned int value;

    public:
        // Constructor
        Counter() {
            value = 2;
        } 

        // Methods
        unsigned int getValue() {
            return value;
        }

        bool getPrediction() {
            if (value > 1) return true;
            else return false;
        }

        void increment() {
            if (value < 3) value++;
        }

        void decrement() {
            if (value > 0) value--;
        }
};

class Predictions {
    private:
        // Attributes
        unsigned long int amount;
        unsigned long int misses;

    public:
        // Constructor
        Predictions() {
            amount = 0;
            misses = 0;
        } 

        // Methods
        void addTrue() {
            amount++;
        }

        void addFalse() {
            amount++;
            misses++;
        }

        unsigned long int getAmount() {
            return amount;
        }

        unsigned long int getMisses() {
            return misses;
        }
};

class History {
    private:
        // Attributes
        unsigned long int value;
        unsigned long int msb;

    public:
        // Constructor
        History(unsigned long int GBH_bits) {
            value = 0;
            msb = pow(2, (GBH_bits-1));
        } 

        // Methods
        void updateTaken() {
            value = (value >> 1) + msb;
        }

        void updateNotTaken() {
            value = value >> 1;
        }

        unsigned long int getValue() {
            return value;
        }
};

void printPredictions(Predictions results) {
    unsigned long int amount = results.getAmount();
    unsigned long int misses = results.getMisses();
    float rate = float (100 * misses) / float (amount);
    printf("OUTPUT\n");
    printf(" number of predictions: %lu\n", amount);
    printf(" number of mispredictions: %lu\n", misses);
    printf(" misprediction rate: %.2f%\n", rate);
}

void printCounters(Counter *BTB, int size, bool mode) {
    if (mode) printf("Final GSHARE CONTENTS\n");
    else printf("Final BIMODAL CONTENTS\n"); 
    for (int i = 0; i < size; i++) {
        printf("%u %u\n", i, BTB[i].getValue());
    }
}


int main (int argc, char* argv[]) {
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file
    unsigned long int index;
    unsigned long int PC_bits;
    unsigned long int GBH_bits;
    unsigned long int DIFF_bits;
    char* BP_type;
    bool mode;
    
    // Getting Branch Predictor Type
    BP_type = argv[1];
    // Bimodal
    if(strcmp(BP_type, "bimodal") == 0) {
        if(argc != 4) {
            printf("Error: %s wrong number of inputs:%d\n", BP_type, argc-1);
            exit(EXIT_FAILURE);
        }
        PC_bits = strtoul(argv[2], NULL, 10);
        trace_file = argv[3];
        mode = bimodal;
        printf("COMMAND\n%s %s %lu %s\n", argv[0], BP_type, PC_bits, trace_file);
    // Gshare
    } else if(strcmp(BP_type, "gshare") == 0) {
        if(argc != 5) {
            printf("Error: %s wrong number of inputs:%d\n", BP_type, argc-1);
            exit(EXIT_FAILURE);
        }
        PC_bits = strtoul(argv[2], NULL, 10);
        GBH_bits = strtoul(argv[3], NULL, 10);
        DIFF_bits = PC_bits - GBH_bits;
        trace_file = argv[4];
        mode = gshare;
        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], BP_type, PC_bits, GBH_bits, trace_file);
    // None of the above
    } else {
        printf("Error: Wrong branch predictor name:%s\n", BP_type);
        exit(EXIT_FAILURE);
    }
    
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL) {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    int size = int (pow(2, PC_bits));
    Counter *BTB = new Counter[size];
    Predictions results;
    
    char str[2];
    if (mode == bimodal) {
        while(fscanf(FP, "%lx %s", &addr, str) != EOF) { 
            outcome = str[0];
            index = ((1 << PC_bits) - 1) & (addr >> 2);
            if (outcome == 't') {
                if (BTB[index].getPrediction()) results.addTrue();
                else results.addFalse();
                BTB[index].increment();
            } else if (outcome == 'n') {
                if (!BTB[index].getPrediction()) results.addTrue();
                else results.addFalse();
                BTB[index].decrement();
            }
        }
    } else if (mode == gshare) {
        History GBH(GBH_bits);
        while(fscanf(FP, "%lx %s", &addr, str) != EOF) { 
            outcome = str[0];
            index = (GBH.getValue() << DIFF_bits) ^ (((1 << PC_bits) - 1) & (addr >> 2));
            if (outcome == 't') {
                if (BTB[index].getPrediction()) results.addTrue();
                else results.addFalse();
                BTB[index].increment();
                GBH.updateTaken();
            } else if (outcome == 'n') {
                if (!BTB[index].getPrediction()) results.addTrue();
                else results.addFalse();
                BTB[index].decrement();
                GBH.updateNotTaken();
            }
        }
    }
    printPredictions(results);
    printCounters(BTB, size, mode);
    return 0;
}