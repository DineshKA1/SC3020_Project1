#include "Block.h"
#include <iostream>
using namespace std;

Block::Block(int id) : block_id(id) {}

int Block::getBlockSize() const {
    int totalSize = 0;
    for (const Record &rec : records) {
        totalSize += rec.size();
    }
    return totalSize;
}

bool Block::canAddRecord(const Record &record) const {
    return getBlockSize() + record.size() <= 4096; // 4096 为块的大小
}

bool Block::addRecord(const Record &record) {
    if (canAddRecord(record)) {
        records.push_back(record);
        return true;
    }
    return false;
}

void Block::printBlock() const {
    cout << "Block ID: " << block_id << endl;
    for (const Record &rec : records) {
        cout << rec.toString() << endl;
    }
}
