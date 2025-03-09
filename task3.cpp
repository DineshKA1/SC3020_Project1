#include "storage.h"
#include "bPlusTree.h"
#include "const.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>


using namespace std;



int main() {
    int timeOfExecution = 1000;
    Storage storageManager;
    storageManager.loadFromDB("games.db");
    int num = storageManager.getNumOfBlocks();
    
    auto startTime = chrono::high_resolution_clock::now();
    int blockAccess = 0, siz = 0;
    float sumFGPCTHome = 0;
    BPlusTree bpTree;
    bpTree.deserialize("bplustree.dat");
    for(int i = 0; i < timeOfExecution; ++i) {
        blockAccess = 0;
        auto result = bpTree.search(0.6, 0.9, blockAccess);
        sumFGPCTHome = 0;
        siz = 0;
        for(auto record:result) 
            sumFGPCTHome += record->FG_PCT_home, siz++;
    }
    auto endTime = chrono::high_resolution_clock::now();
    cout << "Stats:" << endl;     
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "Time consumed: " << elapsed.count() / (float) timeOfExecution << " ms" << std::endl;
    cout << "Block access: " << blockAccess << endl;
    cout << "Average FG_PCT_home: " << sumFGPCTHome << '/' << siz << '=' << sumFGPCTHome / siz<< endl;
    cout << endl;

    //brute-force
    
    auto startTimeBrute = chrono::high_resolution_clock::now();
    float sumFGPCTHomeBrute = 0;
    int count = 0;
    float max = 0;
    blockAccess = 0;
    for(int i = 0; i < num; ++i) {
        Block block = storageManager.getBlock(i);
        blockAccess++;
        for(Record record:block.records) {
            if(record.FG_PCT_home >= 0.6 && record.FG_PCT_home <= 0.9) {
                sumFGPCTHomeBrute += record.FG_PCT_home;
                count++;
            }
        }
    }
    auto endTimeBrute = chrono::high_resolution_clock::now();
    cout << "Stats - brute-force:" << endl; 
    auto elapsedBrute = std::chrono::duration_cast<std::chrono::microseconds>(endTimeBrute - startTimeBrute);
    std::cout << "Time consumed: " << elapsedBrute.count() << " ms" << std::endl;
    cout << "Block access: " << blockAccess << endl;
    cout << "Average FG_PCT_home (brute-force): " << sumFGPCTHomeBrute<< '/' << count << '=' << sumFGPCTHomeBrute / count << endl;
    return 0;
}
