#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;


class BitReader {
private:
    ifstream * ifs;
    unsigned long long int buffer = 0;
    int counter = 0;
    int eof_flag = 0;

public:
    BitReader(string filename) {
        ifs = new ifstream(filename);
        cout << "init bitreader" << endl;
        this->_init_buffer();
    }
    ~BitReader() {
        delete ifs;
    }
    int read_bits(int num_bits, unsigned long long int &data_ret) {
        if (num_bits > 64) { 
            cout << "too many bits read\n"; 
            return -1;
        }
        if (num_bits > counter) {
            cout << "more than counter" << endl;
            return -1;
        }
        data_ret = _read_value_from_buffer(num_bits);
        _left_adj_buffer(num_bits);
        _refill_buffer();

        this->_print_buffer();
        return num_bits;
    }
private:
    void _print_buffer() {
        bitset<64> bs(buffer);
        cout << "buffer: " << bs << endl;
    }
    void _init_buffer() {
        for (int i = 0; i < 8; i++) { // loop bounds may cause problems
            char curr;
            if (ifs->get(curr)) {
                buffer <<= 8;
                buffer |= (unsigned char) curr;
                counter += 8;
            } else break;
        }
        if (counter < 64) { // less than 8 bytes read, needs to be left adjusted
            buffer <<= (64-counter);
        }
    }
    unsigned long long int _read_value_from_buffer(int num_bits) {
        unsigned long long int mask = 1;
        for (int i = 0; i < num_bits-1; i++) {
            mask = (mask << 1) | 1;
        }
        mask <<= (64-num_bits);

        unsigned long long int value = (buffer & mask) >> (64-num_bits);
        return value;
    }
    void _left_adj_buffer(int num_bits) {
        buffer <<= num_bits;
        counter -= num_bits;
    }
    void _refill_buffer() {
        if (eof_flag) return;
        while (counter <= (64-8)){ // space for another byte
            char curr;
            if (ifs->get(curr)) {
                buffer |= (unsigned char) curr;
                counter += 8;
            } else {
                eof_flag = 1;
                break;
            }
        }
        // on break: no space without writing past bounds, buffer is full
    }
};