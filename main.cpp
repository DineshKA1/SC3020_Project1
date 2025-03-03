#include "storage.h"
#include "bPlusTree.h"
#include "const.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    string filename = "games.txt";
    Storage storageManager;
    storageManager.loadData(filename);
    // storageManager.printAllBlocks();
    storageManager.printStatistics();

    // int num = storageManager.getNumOfBlocks();
    // BPlusTree bpTree;
    // for(int i = 0; i < num; ++i) {
    //     Block block = storageManager.getBlock(i);
    //     for(Record record:block.records) {
    //         bpTree.insert(&record);
    //     }
    // }
    // bpTree.printTree();
    // std::ofstream outFile("bplustree.dat", std::ios::binary);
    // bpTree.serialize(outFile);
    // outFile.close();
    // for(auto record:result) {
    //     cout << record->toString() << endl;
    // }

    // std::ifstream inFile("bplustree.dat", std::ios::binary);
    // BPlusTree bpTree;
    // bpTree.deserialize(inFile);

    // auto result = bpTree.search(0.41, 0.6);
    return 0;
}
