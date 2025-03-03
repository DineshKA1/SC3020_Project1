#include "record.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

string formatDate(const string& date) {
    int month, day, year;
    char delimiter1, delimiter2;
    stringstream ss(date);
    ss >> month >> delimiter1 >> day >> delimiter2 >> year;
    stringstream formattedDate;
    formattedDate << setw(2) << setfill('0') << month << '/'
                 << setw(2) << setfill('0') << day << '/'
                 << year;

    return formattedDate.str();
}

Record::Record() {}

Record::Record(const std::string &date, const int &homeID, const int &_pts_home, float fg_home, float fg_away, float fg3_home, short ast_home, short reb_home, bool homeWins) {
    game_date_set = formatDate(date);
    team_id_home = homeID;
    pts_home = _pts_home;
    FG_PCT_home = fg_home;
    FG_PCT_away = fg_away;
    FG3_PCT_home = fg3_home;
    AST_home = ast_home;
    REB_home = reb_home;
    HOME_TEAM_WINS = homeWins;
}

string Record::toString() const {
    return game_date_set + " " + to_string(team_id_home) + " " + to_string(pts_home) + " " + to_string(FG_PCT_home) + " " + to_string(FG_PCT_away) + " " + to_string(FG3_PCT_home) + " " + to_string(AST_home) + " " + to_string(REB_home) + " " + to_string(HOME_TEAM_WINS);
}
int Record::size() const {
    return game_date_set.size() + sizeof(team_id_home) + sizeof(pts_home) + sizeof(FG_PCT_home) + sizeof(FG_PCT_away) +
           sizeof(FG3_PCT_home) + sizeof(AST_home) + sizeof(REB_home) + sizeof(HOME_TEAM_WINS);
}

string Record::serialize() const{

    stringstream ss;
    ss << game_date_set << ";" 
       << team_id_home << ";" 
       << pts_home << ";" 
       << FG_PCT_home << ";" 
       << FG_PCT_away << ";" 
       << FG3_PCT_home << ";" 
       << AST_home << ";" 
       << REB_home << ";" 
       << HOME_TEAM_WINS;
    return ss.str();

}

Record Record::deserialize(const string &serialized) {

    stringstream ss(serialized);
    string date;
    int homeID, pts;
    float fg_home, fg_away, fg3_home;
    short ast_home, reb_home;
    bool homeWins;

    getline(ss, date, ';');
    ss >> homeID;
    ss.ignore(1); //ignore ';' separator
    ss >> pts;
    ss.ignore(1); 
    ss >> fg_home;
    ss.ignore(1); 
    ss >> fg_away;
    ss.ignore(1); 
    ss >> fg3_home;
    ss.ignore(1); 
    ss >> ast_home;
    ss.ignore(1);
    ss >> reb_home;
    ss.ignore(1);
    ss >> homeWins;

    return Record(date, homeID, pts, fg_home, fg_away, fg3_home, ast_home, reb_home, homeWins);
}

/* debugging for serialization and deserialization
void Record::printRecord() const {
    cout << "Game Date: " << game_date_set << "\n"
         << "Home Team ID: " << team_id_home << "\n"
         << "Points Home: " << pts_home << "\n"
         << "FG % Home: " << FG_PCT_home << "\n"
         << "FG % Away: " << FG_PCT_away << "\n"
         << "3-Point FG % Home: " << FG3_PCT_home << "\n"
         << "AST Home: " << AST_home << "\n"
         << "REB Home: " << REB_home << "\n"
         << "Home Team Wins: " << (HOME_TEAM_WINS ? "Yes" : "No") << "\n";
}
         */