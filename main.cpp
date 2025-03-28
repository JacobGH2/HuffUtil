#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <bitset>
#include "BitWriter.h"

using namespace std;

typedef struct {
    char ch;
    int count;
} entry;

class Node {
    public:
        Node(entry e) { // leaf 
            ch = e.ch;
            count = e.count;
        }
        Node(int count, Node *lc, Node *rc) { // inner
            ch = -1;
            this->count = count;
            lChild = lc;
            rChild = rc;
        }
        void Print() {
            cout << ch << endl;
            if (lChild != nullptr) lChild->Print();
            if (rChild != nullptr) rChild->Print();
        }
        void writeTree(BitWriter &out) {
            if (lChild == nullptr && rChild == nullptr) { // leaf
                out.write_bits(1, 0b1); // write 1
                out.write_bits(8, ch); // write char
            } else { // inner
                out.write_bits(1, 0b0);
                lChild->writeTree(out);
                rChild->writeTree(out);
            }
        }
        void getEncs(unordered_map<char, int> &encs, int path) {
            if (lChild != nullptr || rChild != nullptr) { // inner
                // path has been shifted appropriately
                // add and shift
                lChild->getEncs(encs, ((path) << 1));
                rChild->getEncs(encs, ((path+1) << 1));
            } else { // leaf
                encs.insert({this->ch, path});
            }
        }

        int getCount() {return count;}
        char getChar() {return ch;}
    private:
        char ch;
        int count;
        Node * lChild = nullptr;
        Node * rChild = nullptr;
};
class Tree {
    private:
        Node * root;
    public:
        Tree(Node * e) {    
            root = e;
        }
        Tree(entry e) {
            root = new Node(e);
        }
        Tree(Tree *t1, Tree *t2) {
            root = new Node(t1->root->getCount() + t2->root->getCount(), t1->root, t2->root);
        }
        void Print() {
            root->Print();
        }
        void WriteTree(string filename) {
            BitWriter outfile(filename);
            root->writeTree(outfile);

            outfile.flush();
        }
        unordered_map<char, int> getEncodings() {
            unordered_map<char, int> enc;
            // traversal
            this->root->getEncs(enc, 0);
            return enc;
        }
        Node * getRoot() {return root;}
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
    for (int i = 0; i < f.size(); i++) {
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

    int fileCharCount = 0;
    while (ifs.good()) {
        char c = ifs.get();
        int found_index = -1;
        for (int i = 0; i < chars.size(); i++) {
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

        if (c != -1) fileCharCount++;
    }

    ifs.close();
}

void CreateHuffmanTree(vector<Tree*> &forest) {
    while (forest.size() > 1) {
        // find two smallest weighted trees and remove from forest
        int min1 = 1000, minInd1 = 0;
        int min2 = 1000, minInd2 = 0; // TODO: better values
        for (int i = 0; i < forest.size(); i++) {
            if (int curr = forest[i]->getRoot()->getCount() < min1) {
                min1 = curr;
                minInd1 = i;
            }
        }
        for (int i = 0; i < forest.size(); i++) {
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
    
    vector<entry> chars;
    getCharFreq("lorem.txt", chars);  

    // initialize forest with singleton trees
    vector<Tree*> forest;
    for (int i = 0; i < chars.size(); i++) {
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

    return 0;
}