#include "storage.h"
#include "bPlusTree.h"
#include "const.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    Storage storageManager;
    storageManager.loadData("games.txt");
    storageManager.printStatistics();
    storageManager.saveToDB("games.db");
    return 0;
}
