/*-------------------------------
ECE 463 Project 1 Cache Simulation
        Joshua Swaim
--------------------------------*/

// Libraries
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <sstream>
using namespace std;

// Constant Values
#define hit                     1
#define miss                    0
#define L1                      0
#define L2                      1
#define L1_read                 0
#define L1_read_miss            1
#define L1_write                2
#define L1_write_miss           3
#define L1_writeback            4
#define L1_prefetch             5
#define L2_read_demand          6
#define L2_read_miss_demand     7
#define L2_read_prefetch        8
#define L2_read_miss_prefetch   9
#define L2_write                10
#define L2_write_miss           11
#define L2_writeback            12
#define L2_prefetch             13
#define memory_traffic          14

// Classes
class Block {
    private:
        // Attributes
        bool valid;
        bool dirty;
        unsigned int tag;

    public:
        // Constructor
        Block(bool v, bool d, unsigned int t) {
            valid = v;
            dirty = d;
            tag = t;
        } 

        // Methods
        bool isValid() {
            return valid;
        }

        bool isDirty() {
            return dirty;
        }

        unsigned int getTag() {
            return tag;
        }

        void makeDirty() {
            dirty = true;
        }

        void makeClean() {
            dirty = false;
        }
};

class Cache {
    private:
        // Attributes
        unsigned int size;
        unsigned int associativity;
        unsigned int block_size;
        unsigned int number_Of_Sets;
        unsigned int number_Of_Ways;
        unsigned int number_Of_Blocks;
        unsigned int number_Of_Block_Offset_Bits;
        unsigned int number_Of_Index_Bits;
        unsigned int number_Of_Tag_Bits;
        vector<deque<Block> > cache;

    public:
        // Constructor
        Cache(int s, int a, int b_s) {
            size = s;
            associativity = a;
            block_size = b_s;
            number_Of_Sets = size / (associativity * block_size);
            number_Of_Ways = associativity;
            number_Of_Blocks = size / block_size;
            number_Of_Block_Offset_Bits = log2(block_size);
            number_Of_Index_Bits = log2(number_Of_Sets);
            number_Of_Tag_Bits = 32 - number_Of_Index_Bits - number_Of_Block_Offset_Bits;

            for (unsigned int set_number = 0; set_number < number_Of_Sets; set_number++) {
                deque<Block> *set = new deque<Block>;
                for (unsigned int way_number = 0; way_number < number_Of_Ways; way_number++) {
                    Block *block = new Block (false, false, 0);
                    set->push_front(*block);
                }
                cache.push_back(*set);
            }
        }

        // Methods
        void printCache(unsigned int cache_index) {
            printf("===== L%u contents =====\n", ++cache_index);
            for (unsigned int set_number = 0; set_number < number_Of_Sets; set_number++) {
                string block_string = "";
                for (deque<Block>::iterator block = cache[set_number].begin(); block != cache[set_number].end(); block++) {
                    if(block->isValid()) {
                        block_string += "  " + getHex(block->getTag());
                        if(block->isDirty()) {
                            block_string += " D";
                        } else {
                            block_string += "  ";
                        }
                    } else {
                        break;
                    }
                }
                if (!block_string.empty()) {
                    if (set_number > 9) printf("set     %u: %s\n", set_number, block_string.c_str());
                    else printf("set      %u: %s\n", set_number, block_string.c_str());
                }
            }
            printf("\n");
        }

        string getHex(unsigned int i ) {
            stringstream stream;
            stream << std::hex << i;
            return stream.str();
        }

        unsigned int getIndex(unsigned int address) {
            return (((1 << number_Of_Index_Bits) - 1) & (address >> number_Of_Block_Offset_Bits));
        }

        unsigned int getTag(unsigned int address) {
            return (address >> (number_Of_Index_Bits + number_Of_Block_Offset_Bits));
        }

        unsigned int getAddress(unsigned int tag, unsigned int index) {
            return ((tag << (number_Of_Index_Bits + number_Of_Block_Offset_Bits)) + (index << (number_Of_Block_Offset_Bits)));
        }

        unsigned int read(unsigned int address) {
            unsigned int index = getIndex(address);
            unsigned int tag = getTag(address);

            Block hit_block(false, false, 0);
            for (deque<Block>::iterator block = cache[index].begin(); block != cache[index].end(); block++) {
                if (block->isValid()) {
                    if (tag == block->getTag()) {      
                        hit_block = *block;
                        cache[index].erase(block);
                        cache[index].push_front(hit_block);
                        return hit;
                    }
                } else {
                    break;
                }
            }

            cache[index].push_front(*(new Block (true, false, tag)));
            if (cache[index].back().isValid() && cache[index].back().isDirty()) {
                unsigned int writebackAddress = getAddress(cache[index].back().getTag(), index);
                cache[index].pop_back();
                return writebackAddress;
            }
            cache[index].pop_back();
            return miss;
        }

        unsigned int write(unsigned int address) {
            unsigned int index = getIndex(address);
            unsigned int tag = getTag(address);

            Block hit_block(false, false, 0);
            for (deque<Block>::iterator block = cache[index].begin(); block != cache[index].end(); block++) {
                if (block->isValid()) {
                    if (tag == block->getTag()) {
                        hit_block = *block;
                        cache[index].erase(block);
                        hit_block.makeDirty();
                        cache[index].push_front(hit_block);
                        return hit;
                    }
                } else {
                    break;
                }
            }
            cache[index].push_front(*(new Block (true, true, tag)));
            if (cache[index].back().isValid() && cache[index].back().isDirty()) {
                unsigned int writebackAddress = getAddress(cache[index].back().getTag(), index);
                cache[index].pop_back();
                return writebackAddress;
            }
            cache[index].pop_back();
            return miss;
        }
};

class CacheLog {
    private:
        // Attributes                               Index
        unsigned int L1_reads;                      // 0
        unsigned int L1_read_misses;                // 1
        unsigned int L1_writes;                     // 2
        unsigned int L1_write_misses;               // 3
        float L1_miss_rate;                         // NA
        unsigned int L1_writebacks;                 // 4
        unsigned int L1_prefetches;                 // 5
        unsigned int L2_reads_demand;               // 6
        unsigned int L2_read_misses_demand;         // 7
        unsigned int L2_reads_prefetch;             // 8
        unsigned int L2_read_misses_prefetch;       // 9
        unsigned int L2_writes;                     // 10
        unsigned int L2_write_misses;               // 11
        float L2_miss_rate;                         // NA
        unsigned int L2_writebacks;                 // 12
        unsigned int L2_prefetches;                 // 13
        unsigned int memory_traffics;               // 14

    public:
        // Constructor
        CacheLog() {
            // Attributes                  Index
            L1_reads = 0;                  // 0
            L1_read_misses = 0;            // 1
            L1_writes = 0;                 // 2
            L1_write_misses = 0;           // 3
            L1_miss_rate = 0.000;          // NA
            L1_writebacks = 0;             // 4
            L1_prefetches = 0;             // 5
            L2_reads_demand = 0;           // 6
            L2_read_misses_demand = 0;     // 7
            L2_reads_prefetch = 0;         // 8
            L2_read_misses_prefetch = 0;   // 9
            L2_writes = 0;                 // 10
            L2_write_misses = 0;           // 11
            L2_miss_rate = 0.000;          // NA
            L2_writebacks = 0;             // 12
            L2_prefetches = 0;             // 13
            memory_traffics = 0;           // 14
        }

        // Methods
        void updateLog(int item_index) {
            switch (item_index) {
                case 0:  L1_reads++;                break;
                case 1:  L1_read_misses++;          break;
                case 2:  L1_writes++;               break;
                case 3:  L1_write_misses++;         break;
                case 4:  L1_writebacks++;           break;
                case 5:  L1_prefetches++;           break;
                case 6:  L2_reads_demand++;         break;
                case 7:  L2_read_misses_demand++;   break;
                case 8:  L2_reads_prefetch++;       break;
                case 9:  L2_read_misses_prefetch++; break;
                case 10: L2_writes++;               break;
                case 11: L2_write_misses++;         break;
                case 12: L2_writebacks++;           break;
                case 13: L2_prefetches++;           break;
                case 14: memory_traffics++;         break;
            }
        }

        // Methods
        void uploadLog(CacheLog log) {
            L1_reads += log.L1_reads;
            L1_read_misses += log.L1_read_misses;
            L1_writes += log.L1_writes;
            L1_write_misses += log.L1_write_misses;
            L1_writebacks += log.L1_writebacks;
            L1_prefetches += log.L1_prefetches;
            L2_reads_demand += log.L2_reads_demand;
            L2_read_misses_demand += log.L2_read_misses_demand;
            L2_reads_prefetch += log.L2_reads_prefetch;
            L2_read_misses_prefetch += log.L2_read_misses_prefetch;
            L2_writes += log.L2_writes;
            L2_write_misses += log.L2_write_misses;
            L2_writebacks += log.L2_writebacks;
            L2_prefetches += log.L2_prefetches;
            memory_traffics += log.memory_traffics;
        }

        void printCacheLog() {
            if ((L1_reads + L1_writes) != 0) {
                L1_miss_rate = (double) (L1_read_misses + L1_write_misses) / (double) (L1_reads + L1_writes);
            }
            if (L2_reads_demand != 0) {
                L2_miss_rate = (double) L2_read_misses_demand / (double) L2_reads_demand;
            }
            printf("===== Measurements =====\n");
            printf("a. L1 reads:                   %u\n", L1_reads);
            printf("b. L1 read misses:             %u\n", L1_read_misses);
            printf("c. L1 writes:                  %u\n", L1_writes);
            printf("d. L1 write misses:            %u\n", L1_write_misses);
            printf("e. L1 miss rate:               %.4f\n", L1_miss_rate);
            printf("f. L1 writebacks:              %u\n", L1_writebacks);
            printf("g. L1 prefetches:              %u\n", L1_prefetches);
            printf("h. L2 reads (demand):          %u\n", L2_reads_demand);
            printf("i. L2 read misses (demand):    %u\n", L2_read_misses_demand);
            printf("j. L2 reads (prefetch):        %u\n", L2_reads_prefetch);
            printf("k. L2 read misses (prefetch):  %u\n", L2_read_misses_prefetch);
            printf("l. L2 writes:                  %u\n", L2_writes);
            printf("m. L2 write misses:            %u\n", L2_write_misses);
            printf("n. L2 miss rate:               %.4f\n", L2_miss_rate);
            printf("o. L2 writebacks:              %u\n", L2_writebacks);
            printf("p. L2 prefetches:              %u\n", L2_prefetches);
            printf("q. memory traffic:             %u\n", memory_traffics);
        }

};

class Heirarchy {
    private:
        // Atributes
        vector<Cache> Caches;
        bool two_levels;
    
    public:
        // Constructor
        Heirarchy(unsigned int blocksize, unsigned int L1_size, unsigned int L1_assoc, unsigned int L2_size, unsigned int L2_assoc) {
            Caches.push_back(*(new Cache (L1_size, L1_assoc, blocksize)));
            if (L2_size > 0) {
                two_levels = true;
                Caches.push_back(*(new Cache (L2_size, L2_assoc, blocksize)));
            } else {
                two_levels = false;
            }
        }

        //Methods
        CacheLog readRequest(unsigned int address) {
            CacheLog readLog;
            // Do a read request
            unsigned int L1_result = Caches[L1].read(address);
            readLog.updateLog(L1_read);
            if (L1_result != hit) {
                // L1 read miss
                readLog.updateLog(L1_read_miss);
                if (two_levels) {
                    if (L1_result != miss) {
                        // Write back
                        unsigned int L1_WB_result = Caches[L2].write(L1_result);
                        readLog.updateLog(L1_writeback);
                        readLog.updateLog(L2_write);
                        if (L1_WB_result != hit) {
                            readLog.updateLog(L2_write_miss);
                            if (L1_WB_result != miss) {
                                readLog.updateLog(memory_traffic);
                            }
                        }
                    }
                    unsigned int L2_result = Caches[L2].read(address);
                    readLog.updateLog(L2_read_demand);
                    if (L2_result != hit) {
                        readLog.updateLog(L2_read_miss_demand);
                        readLog.updateLog(memory_traffic);
                        if (L2_result != miss) {
                            readLog.updateLog(L2_writeback);
                            readLog.updateLog(memory_traffic);
                        }
                    }
                } else {
                    readLog.updateLog(memory_traffic);
                    if (L1_result != miss) {
                        readLog.updateLog(L1_writeback);
                        readLog.updateLog(memory_traffic);
                    }
                }
            }
            return readLog;
        }

        CacheLog writeRequest(unsigned int address) {
            CacheLog writeLog;
            // Do a write request
            unsigned int L1_result = Caches[L1].write(address);
            writeLog.updateLog(L1_write);
            if (L1_result != hit) {
                // L1 write miss
                writeLog.updateLog(L1_write_miss);
                if (two_levels) {
                    if (L1_result != miss) {
                        // Write back
                        unsigned int L1_WB_result = Caches[L2].write(L1_result);
                        writeLog.updateLog(L1_writeback);
                        writeLog.updateLog(L2_write);
                        if (L1_WB_result != hit) {
                            writeLog.updateLog(L2_write_miss);
                            if (L1_WB_result != miss) {
                                writeLog.updateLog(memory_traffic);
                            }
                        }
                    }
                    unsigned int L2_result = Caches[L2].read(address);
                    writeLog.updateLog(L2_read_demand);
                    if (L2_result != hit) {
                        writeLog.updateLog(L2_read_miss_demand);
                        writeLog.updateLog(memory_traffic);
                        if (L2_result != miss) {
                            writeLog.updateLog(L2_writeback);
                            writeLog.updateLog(memory_traffic);
                        }
                    }
                } else {
                    writeLog.updateLog(memory_traffic);
                    if (L1_result != miss) {
                        writeLog.updateLog(L1_writeback);
                        writeLog.updateLog(memory_traffic);
                    }
                }
            }
            return writeLog;
        }

        void printContents() {
            Caches[L1].printCache(L1);
            if (two_levels) {
                Caches[L2].printCache(L2);
            }
        }

};

// Main
int main (int argc, char *argv[]) {
    // Local variables
    FILE *fp;               // File pointer.
    char *trace_file;       // This variable holds the trace file name.
    char rw;			    // This variable holds the request's type (read or write) obtained from the trace.
    uint32_t addr;		    // This variable holds the request's address obtained from the trace.

    // Exit with an error if the number of command-line arguments is incorrect.
    if (argc != 9) {
        printf("Error: Expected 8 command-line arguments but was provided %d.\n", (argc - 1));
        exit(EXIT_FAILURE);
    }

    // Print simulator configuration.
    printf("===== Simulator configuration =====\n");
    printf("BLOCKSIZE:  %u\n", (uint32_t) atoi(argv[1]));
    printf("L1_SIZE:    %u\n", (uint32_t) atoi(argv[2]));
    printf("L1_ASSOC:   %u\n", (uint32_t) atoi(argv[3]));
    printf("L2_SIZE:    %u\n", (uint32_t) atoi(argv[4]));
    printf("L2_ASSOC:   %u\n", (uint32_t) atoi(argv[5]));
    printf("PREF_N:     %u\n", (uint32_t) atoi(argv[6]));
    printf("PREF_M:     %u\n", (uint32_t) atoi(argv[7]));
    printf("trace_file: %s\n\n", argv[8]);

    // Creating caches and log
    CacheLog Measurements;
    Heirarchy Caches((unsigned int) atoi(argv[1]), (unsigned int) atoi(argv[2]), (unsigned int) atoi(argv[3]), (unsigned int) atoi(argv[4]), (unsigned int) atoi(argv[5]));

    // Open the trace file for reading.
    trace_file = argv[8];
    fp = fopen(trace_file, "r");
    if (fp == (FILE *) NULL) {
        // Exit with an error if file open failed.
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    // Read requests from the trace file and echo them back.
    while (fscanf(fp, "%c %x\n", &rw, &addr) == 2) {	// Stay in the loop if fscanf() successfully parsed two tokens as specified.
        if (rw == 'r') {
            Measurements.uploadLog(Caches.readRequest(addr));
        } else if (rw == 'w') {
            Measurements.uploadLog(Caches.writeRequest(addr));
        } else {
            printf("Error: Unknown request type %c.\n", rw);
	        exit(EXIT_FAILURE);
        }
    }
   
    Caches.printContents();
    Measurements.printCacheLog();

    return(0);
}