#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "record.h"

class Block {
public:
    std::vector<Record> records;
    int block_id;

    Block(int id);

    int getBlockSize() const;
    bool canAddRecord(const Record &record) const;
    bool addRecord(const Record &record);
    void printBlock() const;
};

#endif // BLOCK_H
