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

void getCharFreq(string filename, vector<entry> &chars) {
    ifstream ifs(filename, ifstream::in);

    while (ifs.good()) {
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
}

void CreateHuffmanTree(vector<Tree*> &forest) {
    while (forest.size() > 1) {
        // find two smallest weighted trees and remove from forest
        int min1 = 1000, minInd1 = 0;
        int min2 = 1000, minInd2 = 0; // TODO: better values
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

int main() {
    // ---------- Encoding --------------
    vector<entry> chars;
    getCharFreq("lorem.txt", chars);  

    // initialize forest with singleton trees
    vector<Tree*> forest;
    for (int i = 0; (size_t) i < chars.size(); i++) {
        if (chars[i].ch == -1) continue; // don't create node for EOF
        Tree *newTree = new Tree(chars[i]);
        forest.push_back(newTree);
    }

    cout << "forest size: " << forest.size() << endl;
    CreateHuffmanTree(forest); // merge trees into huffman tree
    cout << "-------" << endl;
    cout << "forest size: " << forest.size() << endl;

    // view encodings, not needed
    unordered_map<char, int> encodings = forest[0]->getEncodings();
    for (auto i: encodings) {
        bitset<32> enc(i.second);
        if (i.first != '\n') {
            cout << i.first << " " << enc << endl;
        } else {
            cout << "\n" << " " << enc << endl;
        }
    }

    // write tree to file
    forest[0]->WriteTree("treeOut");

    // write encoded data to file
    ifstream ifs("lorem.txt");
    BitWriter bw("treeOut");
    char curr;
    while (ifs.get(curr)) {
        unsigned long long int enc = encodings[curr];
        int length = enc != 0 ? log2(enc) + 1 : 2; // TODO: for this input file specifically, needs to be changed
        bw.write_bits(length, enc);
    }

    // ---------- Decoding --------------
    // read and construct tree from file
    Tree conTree("treeOut");

    // read encoding and output uncompressed file

    return 0;
}