#include "storage.h"
#include "bPlusTree.h"
#include <iostream>

using namespace std;

int main() {
    string filename = "games.txt";
    Storage storageManager;
    storageManager.loadData(filename);
    storageManager.printAllBlocks();
    // storageManager.printStatistics();

    int num = storageManager.getNumOfBlocks();
    BPlusTree bpTree;
    for(int i = 0; i < num; ++i) {
        Block block = storageManager.getBlock(i);
        for(Record record:block.records) {
            bpTree.insert(&record);
        }
    }
    bpTree.printTree();

    return 0;
}
