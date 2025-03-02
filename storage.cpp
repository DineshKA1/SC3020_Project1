#include "Storage.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void Storage::loadData(const string &filename) {
    ifstream file(filename);
    string line;
    getline(file, line);
    Block currentBlock(currentBlockId);
    while (getline(file, line)) {
        stringstream ss(line);
        string game_id, team_home, team_away;
        float FG_PCT_home, FG_PCT_away;
        ss >> game_id >> team_home >> team_away >> FG_PCT_home >> FG_PCT_away;
        Record record(game_id, team_home, team_away, FG_PCT_home, FG_PCT_away);

        if (!currentBlock.addRecord(record)) {
            blocks.push_back(currentBlock);
            currentBlockId++;
            currentBlock = Block(currentBlockId);
            currentBlock.addRecord(record);
        }
    }

    if (!currentBlock.records.empty()) {
        blocks.push_back(currentBlock);
    }
}

void Storage::printAllBlocks() const {
    for (const Block &block : blocks) {
        block.printBlock();
    }
}

void Storage::printStatistics() const {
    int totalRecords = 0;
    for (const Block &block : blocks) {
        totalRecords += block.records.size();
    }

    cout << "Total number of records: " << totalRecords << endl;
    cout << "Total number of blocks: " << blocks.size() << endl;
    cout << "Block size: " << 4096 << " bytes" << endl; // add to const
}

int Storage::getNumOfBlocks() {
    return blocks.size();
}

Block Storage::getBlock(int offset) {
    return blocks[offset];
}