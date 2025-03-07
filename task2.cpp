#include "storage.h"
#include "bPlusTree.h"
#include "const.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    Storage storageManager;
    storageManager.loadFromDB("games.db");
    int num = storageManager.getNumOfBlocks();
    // Consturct B+ tree
    BPlusTree bpTree;
    for(int i = 0; i < num; ++i) {
        Block block = storageManager.getBlock(i);
        for(Record record:block.records) {
            bpTree.insert(&record);
        }
    }
    bpTree.printStatics();
    bpTree.serialize("bplustree.dat");
    return 0;
}
