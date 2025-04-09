#include <iostream>
#include <string>
//#include "huffman.cpp"

using namespace std;

extern void huffmanEncode(string inFile);
extern void huffmanDecode(string inFile);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "usage ./huffman <mode> <file>" << endl;
        return -1;
    }

    string mode(argv[1]);
    string file(argv[2]);
    if (mode == "--encode") {
        huffmanEncode(argv[2]);
        cout << "file encoded and output to <>" << endl;
    } else if (mode == "--decode") {
        huffmanDecode(argv[2]);
        cout << "file decoded and output to <>" << endl;
    } else {
        cout << "usage ./huffman <mode> <file>" << endl;
    }

}