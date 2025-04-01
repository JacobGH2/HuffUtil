#include "BitWriter.h"
#include <unordered_map>

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