#ifndef BIT_READER
#define BIT_READER

#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

class BitReader {
    private:
        ifstream * ifs;
        unsigned long long int buffer = 0; // buffer of binary data
        int counter = 0; // bits of valid data in buffer
        int eof_flag = 0; // signals that wrapped file stream has terminated
    public:
        BitReader(string filename);
        ~BitReader();
        char read_char();
        int read_bit();
        int valid_data_remaining();
    private:
        int _read_bits(int num_bits, unsigned long long int &data_ret);
        void _print_buffer();
        void _init_buffer();
        unsigned long long int _read_value_from_buffer(int num_bits);
        void _adj_and_refill(int num_bits);
        void _refill_buffer();
};

#endif