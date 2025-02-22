#ifndef STORAGE_H
#define STORAGE_H

#include "const.h"
#include <fstream>
#include <string>
#include <vector>

class NBARecord
{
    private:
        std::string GAME_DATE_EST;
        int TEAM_ID_home;
        int PTS_home;
        float FG_PCT_home;
        float FT_PCT_home;
        float FG3_PCT_home;
        int AST_home;
        int REB_home;
        int HOME_TEAM_WINS;
    public:
        NBARecord(const std::string& line);
        NBARecord();
};

class Block {
    public:
        std::vector<NBARecord> records;
        Block();
};

class Storage {
    private:
        std::vector<Block> blocks;
    public:
        Storage();
        void loadFromFile(const std::string& filename);
};

#endif 