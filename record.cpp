#include "Record.h"
#include <string>
using namespace std;

Record::Record() {}

Record::Record(const string &id, const string &home, const string &away, float fg_home, float fg_away)
    : game_id(id), team_home(home), team_away(away), FG_PCT_home(fg_home), FG_PCT_away(fg_away) {}

int Record::size() const {
    return sizeof(game_id) + sizeof(team_home) + sizeof(team_away) + sizeof(FG_PCT_home) + sizeof(FG_PCT_away);
}

string Record::toString() const {
    return game_id + " " + team_home + " " + team_away + " " + to_string(FG_PCT_home) + " " + to_string(FG_PCT_away);
}
