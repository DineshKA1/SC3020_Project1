#include "storage.h"
#include <iostream>
#include <string>

int main() {
    std::string fileName = "games.txt";
    Storage storage;
    storage.loadFromFile(fileName);
}