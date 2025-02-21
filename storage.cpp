#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "NBARecord.h"

using namespace std;

struct Block
{
    vector<NBARecord> records;
};


vector<Block> readDataStoreInBlocks(const string& filename, int blockSize){

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return {};  //returning empty vector to prevent crashes
    }

    vector<Block> blocks;
    Block currBlock;
    string line;
    int totalRecords = 0;

    //skip the header line 
    getline(file, line);

    while(getline(file, line)){

        NBARecord record(line);
        currBlock.records.push_back(record);
        totalRecords++;

        if(currBlock.records.size() >= blockSize){
            blocks.push_back(currBlock);
            currBlock.records.clear();
        }

    }

    if(!currBlock.records.empty()){
        blocks.push_back(currBlock);
    }

    file.close();

    cout << "Size of record: " << sizeof(NBARecord) << " bytes" << endl;
    cout << "Total no. of records: " << totalRecords << endl;
    cout << "Records per block: " << blockSize << endl;
    cout << "Total no. of blocks: " << blocks.size() << endl;

    return blocks;

}
/*
int main()
{
    string filename = "games.txt";
    int blockSize = 4;

    vector<Block> blocks = readDataStoreInBlocks(filename, blockSize);

    cout << "No. of blocks: " << blocks.size() << endl;
    cout << "No. of records in each block: " << blockSize << endl;

    return 0;
}
*/
