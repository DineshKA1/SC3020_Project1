#include "storage.h"
#include <vector>
#include <iostream>

NBARecord::NBARecord(const std::string& line){
    //see parsing of exact line for debugging
    std::cout << "Parsing line: " << line << std::endl;

    //parsing the line
    int parsed = sscanf(line.c_str(), "%s %d %d %f %f %f %d %d %d", 
                        &GAME_DATE_EST[0], &TEAM_ID_home, &PTS_home, &FG_PCT_home, 
                        &FT_PCT_home, &FG3_PCT_home, &AST_home, &REB_home, &HOME_TEAM_WINS);
    
    if (parsed != 9) {
        std::cerr << "Error: Failed to parse the line correctly\n";
    }
}

NBARecord::NBARecord() : TEAM_ID_home(0), PTS_home(0), FG_PCT_home(0.0f), FT_PCT_home(0.0f),
        FG3_PCT_home(0.0f), AST_home(0), REB_home(0), HOME_TEAM_WINS(0) {}

Block::Block() {
    records = std::vector<NBARecord>(BLOCK_SIZE);
}

Storage::Storage() {
    blocks = std::vector<Block>(0);
}

void Storage::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }

    Block currBlock;
    std::string line;
    int totalRecords = 0;

    //skip the header line 
    getline(file, line);

    while(getline(file, line)){

        NBARecord record(line);
        currBlock.records.push_back(record);
        totalRecords++;

        if(currBlock.records.size() >= BLOCK_SIZE){
            blocks.push_back(currBlock);
            currBlock.records.clear();
        }

    }

    if(!currBlock.records.empty()){
        blocks.push_back(currBlock);
    }

    file.close();
    std::cout << "Size of record: " << sizeof(NBARecord) << " bytes" << std::endl;
    std::cout << "Total no. of records: " << totalRecords << std::endl;
    std::cout << "Records per block: " << BLOCK_SIZE << std::endl;
    std::cout << "Total no. of blocks: " << blocks.size() << std::endl;
}
