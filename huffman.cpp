#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <bitset>
#include <math.h>
#include "BitWriter.h"
#include "huffman.h"

using namespace std;

union charCountUnion {
    char c[4];
    int count;
};

void print_vec(const vector<entry> &vec) {
    stringstream ss;
    for(size_t i = 0; i < vec.size(); i++)
    {
        if(i != 0) ss << ",";
        ss << vec[i].ch << " " << vec[i].count;
    }
    std::string s = ss.str();
    cout << s << endl;
}

void PrintForest(vector<Tree*> &f) {
    int lineReturn = 0;
    for (int i = 0; (size_t) i < f.size(); i++) {
        cout << f[i]->getRoot()->getChar() << " " << f[i]->getRoot()->getCount() << "    ";
        if (lineReturn == 5) {
            lineReturn = 0;
            cout << endl;
        }
        lineReturn++;
    }
    cout << endl;
}

int getCharFreq(string filename, vector<entry> &chars) {
    ifstream ifs(filename, ifstream::in);
    int charCount = 0;
    while (ifs.good()) {
        charCount++;
        char c = ifs.get();
        int found_index = -1;
        for (int i = 0; (size_t) i < chars.size(); i++) {
            if (chars[i].ch == c) {
                found_index = i;
                break;
            }
        }
        if (found_index == -1) {
            entry e = {c, 1};
            chars.push_back(e);
        } else {
            chars[found_index].count++;
        }
    }

    ifs.close();
    return charCount;
}

void CreateHuffmanTree(vector<Tree*> &forest) {
    while (forest.size() > 1) {
        // find two smallest weighted trees and remove from forest
        int min1 = INT_MAX, minInd1 = 0;
        int min2 = INT_MAX, minInd2 = 0; // TODO: better values
        for (int i = 0; (size_t) i < forest.size(); i++) {
            if (int curr = forest[i]->getRoot()->getCount() < min1) {
                min1 = curr;
                minInd1 = i;
            }
        }
        for (int i = 0; (size_t) i < forest.size(); i++) {
            if (i == minInd1) continue;
            if (int curr = forest[i]->getRoot()->getCount() < min2) {
                min2 = curr;
                minInd2 = i;
            }
        }
        // create new tree with sum
        Tree *joinedTree = new Tree(forest[minInd1], forest[minInd2]);
        forest.erase(forest.begin() + std::max(minInd1, minInd2));
        forest.erase(forest.begin() + std::min(minInd1, minInd2));
        // add tree to forest
        forest.push_back(joinedTree);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "usage: ./huffman <file>" << endl;
        exit(1);
    }
    charCountUnion countUn = {{0}};
    // ---------- Encoding --------------
    vector<entry> chars;
    int charCount = getCharFreq(argv[1], chars); // populate entry vector and get total char count
    // initialize forest with singleton trees
    vector<Tree*> forest;
    for (int i = 0; (size_t) i < chars.size(); i++) {
        if (chars[i].ch == -1) continue; // don't create node for EOF
        Tree *newTree = new Tree(chars[i]);
        forest.push_back(newTree);
    }

    CreateHuffmanTree(forest); // merge trees into huffman tree

    // view encodings, not needed
    unordered_map<char, int> encodings = forest[0]->getEncodings();
    unordered_map<char, int> pathSizes = forest[0]->getPathSizes();
    /* for (auto i: encodings) {
        bitset<32> enc(i.second);
        if (i.first != '\n') {
            cout << i.first << " " << enc << endl;
        } else {
            cout << "\\n" << " " << enc << endl;
        }
    } */

    // write tree to file and receive tree's BitWriter
    BitWriter bw = forest[0]->WriteTree("treeOut");

    // write number of characters
    bw.write_bits(32, (unsigned long long int) charCount);

    // write encoded data to file
    ifstream ifs(argv[1]);
    char curr;
    for (int i = 0; i < charCount; i++) {
        ifs.get(curr);
        unsigned long long int enc = encodings[curr];
        int length = pathSizes[curr];
        bw.write_bits(length, enc);
    }
    ifs.close();
    bw.flush();

    // ---------- Decoding --------------
    // read and construct tree from file
    Tree conTree("treeOut");

    // read encoding and output uncompressed file
    ofstream ofs("uncomp.txt");
    BitReader main_br = *(conTree.getBR()); // get BR already progressed past tree

    // read number of characters
    for (int i = 3; i >= 0; i--) {
        countUn.c[i] = main_br.read_char();
    }
    int recCharCount = countUn.count;

    for (int i = 0; i < recCharCount-1; i++) { // one char
        if (main_br.valid_data_remaining() <= 0) break;
        // read one encoded char using tree
        Node * currNode = conTree.getRoot();
        while (true) {  // one bit
            
            if (currNode->getChar() != -1) { // leaf
                ofs.put(currNode->getChar());
                break;
            }
            if (main_br.read_bit() == 0) currNode = currNode->getLeft(); // inner, continue down path
            else currNode = currNode->getRight();
        }
    }
    ofs.flush();
    ofs.close();

    return 0;
}