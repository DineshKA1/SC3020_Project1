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
    BPlusTree bpTree;
    bpTree.deserialize("bplustree.dat");
    cout << "B+ tree loaded." << endl;
    auto result = bpTree.search(0.6, 0.9);
    float sumFG3PCTHome = 0;
    for(auto record:result) {
        sumFG3PCTHome += record->FG3_PCT_home;
    }
    cout << "Average FG3_PCT_home: " << sumFG3PCTHome<< ' ' << result.size() << endl;

    //brute-force
    float sumFG3PCTHomeBrute = 0;
    int count = 0;
    float max = 0;
    for(int i = 0; i < num; ++i) {
        Block block = storageManager.getBlock(i);
        for(Record record:block.records) {
            if(record.FG_PCT_home >= 0.6 && record.FG_PCT_home <= 0.9) {
                sumFG3PCTHomeBrute += record.FG3_PCT_home;
                count++;
            }
        }
    }
    
    cout << "Average FG3_PCT_home (brute-force): " << sumFG3PCTHomeBrute<< ' ' << count << endl;
    return 0;
}
