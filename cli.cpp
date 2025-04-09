#include <iostream>
#include <string>
#include <fstream>

using namespace std;

extern void huffmanEncode(string inFile);
extern void huffmanDecode(string inFile);

int main(int argc, char *argv[]) {
    if (argc != 3) { // length check
        cout << "usage ./huffman <mode> <file>" << endl;
        cout << "<mode> = --encode OR --decode" << endl;
        return -1;
    }

    string mode(argv[1]);
    string inFile(argv[2]);
    if (mode == "--encode" || mode == "--decode") {
        ifstream file(inFile);
        if (!file) { // valid file check
            std::cerr << "Error: File not found\n";
            return 1;
        }
        file.close();

        if (mode == "--encode") { // --Encoding--
            huffmanEncode(inFile);
            string str(inFile);
            string outFile = str.substr(0, str.find(".txt")) + string(".huff");
            cout << "file encoded and output to " << outFile << endl;
        } else if (mode == "--decode") { // --Decoding--
            huffmanDecode(inFile);
            string str(inFile);
            string outFile = str.substr(0, str.find(".huff")) + string(".txt.uncomp");
            cout << "file decoded and output to " << outFile << endl;
        }
    } else { // valid mode check
        cout << "usage ./huffman <mode> <file>" << endl;
    }
}