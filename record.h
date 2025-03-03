#ifndef RECORD_H
#define RECORD_H

#include <string>

class Record {
public:
    std::string game_date_set;
    int team_id_home;
    int pts_home;
    float FG_PCT_home,  FG_PCT_away, FG3_PCT_home;
    short AST_home, REB_home;
    bool HOME_TEAM_WINS;
    Record();
    Record(const std::string &date, const int &homeID, const int &pts_home, float fg_home, float fg_away, float fg3_home, short ast_home, short reb_home, bool homeWins);

    int size() const;
    std::string toString() const;
};

#endif // RECORD_H
