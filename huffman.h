#include "BitWriter.h"
#include "BitReader.h"
#include <unordered_map>
#include <iostream>

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
        Node(BitReader *br, int ps) {
            int type = 0;
            if (br->valid_data_remaining() >= 1) {
                type = br->read_bit();
            }
            if (type == 1) {
                if (br->valid_data_remaining() >= 8) {
                    ch = br->read_char();
                }
            } else {
                if (br->valid_data_remaining() > 0) {
                    this->lChild = new Node(br, ps+1);
                    this->rChild = new Node(br, ps+1);
                } 
            }
            count = -1;
        }
        ~Node() {
            delete lChild;
            delete rChild;
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
                lChild->getEncs(encs, (path << 1) );
                rChild->getEncs(encs, ((path<< 1) + 1));
            } else { // leaf
                encs.insert({this->ch, path});
            }
        }
        void getSizes(unordered_map<char, int> &sizes, int pathSize) {
            if (lChild != nullptr || rChild != nullptr) { // inner
                // path has been shifted appropriately
                // add and shift
                lChild->getSizes(sizes, pathSize+1);
                rChild->getSizes(sizes, pathSize+1);
            } else { // leaf
                sizes.insert({this->ch, pathSize});
            }
        }

        int getCount() {return count;}
        char getChar() {return ch;}
        Node * getLeft() {return lChild;}
        Node * getRight() {return rChild;}
    private:
        char ch = -1;
        int count;
        Node * lChild = nullptr;
        Node * rChild = nullptr;
};

class Tree {
    private:
        Node * root;
        BitReader * br = 0;
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
        Tree(string filename) {
            br = new BitReader(filename);
            root = new Node(br, 0); 
        }
        ~Tree() {
            delete br;
            delete root;
        }
        void Print() {
            root->Print();
        }
        BitReader* getBR() {
            return br;
        }
        BitWriter WriteTree(string filename) {
            BitWriter outfile(filename);
            root->writeTree(outfile);

            return outfile;  
        }
        unordered_map<char, int> getEncodings() {
            unordered_map<char, int> enc;
            // traversal
            this->root->getEncs(enc, 0);
            return enc;
        }
        unordered_map<char, int> getPathSizes() {
            unordered_map<char, int> sizes;
            // traversal
            this->root->getSizes(sizes, 0);
            return sizes;
        }
        void setRootNull() {
            this->root = nullptr;
        }
        Node * getRoot() {return root;}
};