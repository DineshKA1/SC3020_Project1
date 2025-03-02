#ifndef RECORD_H
#define RECORD_H

#include <string>

class Record {
public:
    std::string game_id;
    std::string team_home;
    std::string team_away;
    float FG_PCT_home;
    float FG_PCT_away;

    Record();
    Record(const std::string &id, const std::string &home, const std::string &away, float fg_home, float fg_away);

    int size() const;
    std::string toString() const;
};

#endif // RECORD_H
