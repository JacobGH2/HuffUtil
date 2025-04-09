# HuffUtil
A lightweight C++ implementation of Huffman encoding for text documents. 

The program implements a tree structure, bit-level wrappers over fstream objects, and a command line interface.

### Build Instructions
```make HuffUtil``` will build the executable. 

### Usage
Basic usage is: ```./HuffUtil <mode> <file>```

```<mode>``` is either ```--encode``` or ```--decode```

For encoding, the input file can be any ASCII encoded text file. The encoded output will have a ```.huff``` extension.

For decoding, the input file must be a HuffUtil encoded binary file (```.huff```). The output will be an ASCII encoded text file.