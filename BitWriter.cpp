#include "BitWriter.h"

using namespace std;
    
BitWriter::BitWriter(string filename) {
    out = new ofstream(filename);
}

int BitWriter::write_bits(int num_bits, int data) {
    // writes to buffer and left justifies
    if (num_bits + counter > 64) { // error
        cout << "will overwrite content of buffer" << endl;
        abort();
    }

    // add data to buffer after shifting over
    buffer |= (((unsigned long long int) data ) << (64-counter-num_bits));
    counter += num_bits;
    // actual writing performed internally
    this->_write();

    return num_bits;
}

void BitWriter::flush() {
    // write final byte
    this->_final_write();
}


void BitWriter::_write() { // write bits until count < 8
    while (counter >= 8) {
        char byte = (char) (buffer >> 56);
        out->put(byte);
        out->flush();
        buffer <<= 8;
        counter -= 8;
    }
}

void BitWriter::_final_write() { // write final byte if counter != 0
    if (counter == 0) return;
    char byte = (char) (buffer >> 56);
    out->put(byte);
    out->flush();
}

void BitWriter::_print_state() {
    bitset<64> bs(buffer);
    cout << "buffer: " << bs << endl;
}