#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "record.h"

class Block {
public:
    std::vector<Record> records;
    int block_id;

    Block(); //if empty block is first created before assigning values
    Block(int id);

    int getBlockSize() const;
    bool canAddRecord(const Record &record) const;
    bool addRecord(const Record &record);
    void printBlock() const;

    std::string serialize() const;
    static Block deserialize(const std::string &serialized);

};

#endif // BLOCK_H
