#ifndef NBARECORD_H
#define NBARECORD_H

#include <string>
#include <iostream>
#include <cstdio>

struct NBARecord
{
    std::string GAME_DATE_EST;
    int TEAM_ID_home;
    int PTS_home;
    float FG_PCT_home;
    float FT_PCT_home;
    float FG3_PCT_home;
    int AST_home;
    int REB_home;
    int HOME_TEAM_WINS;

    NBARecord(const std::string& line){
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

    NBARecord() : TEAM_ID_home(0), PTS_home(0), FG_PCT_home(0.0f), FT_PCT_home(0.0f),
                 FG3_PCT_home(0.0f), AST_home(0), REB_home(0), HOME_TEAM_WINS(0) {}

};

#endif
