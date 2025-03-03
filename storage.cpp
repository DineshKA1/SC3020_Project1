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
        string game_data_set;
        int team_id_home, pts_home, AST_home, REB_home;
        bool HOME_TEAM_WINS;
        float FG3_PCT_home;

        float FG_PCT_home, FG_PCT_away;
        ss >> game_data_set >> team_id_home >> pts_home >> FG_PCT_home >> FG_PCT_away >> FG3_PCT_home >> AST_home >> REB_home >> HOME_TEAM_WINS;
        Record record(game_data_set, team_id_home, pts_home, FG_PCT_home, FG_PCT_away, FG3_PCT_home, AST_home, REB_home, HOME_TEAM_WINS);

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
    int sizeOfRecord;
    for (const Block &block : blocks) {
        totalRecords += block.records.size();
        sizeOfRecord = block.records[0].size();
    }

    cout << "Size of each record: " << sizeOfRecord << " bytes" << endl;
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