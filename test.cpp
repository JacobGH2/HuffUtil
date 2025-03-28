#include <iostream>
#include "BitWriter.h"

using namespace std;

int main() {
    BitWriter bw("output.huff");
    
    bw.write_bits(8, 'c');
    bw.write_bits(8, 'h');
    bw.write_bits(8, 'l');
    bw.write_bits(8, 'o');
    bw.write_bits(8, 'e');

    bw.flush();
    

    return 0;
}