#ifndef BIT_WRITER
#define BIT_WRITER

#include <fstream>
#include <iostream>
#include <bitset>

using namespace std;

class BitWriter {
    private: 
        ofstream * out;
        unsigned long long int buffer = 0;
        int counter = 0;
    public:
        BitWriter(string filename);
        ~BitWriter();
        int write_bits(int num_bits, unsigned long long int data);
        void flush();
    private:
        void _write();
        void _final_write();
        void _print_state();
};

#endif