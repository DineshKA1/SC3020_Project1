#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iomanip>
#include "storage.h"

struct BPlusTreeNode
{
    bool isLeaf;
    std::vector<float> keys;
    std::vector<BPlusTreeNode*> children;
    std::vector<Record*> records;
    BPlusTreeNode* parent;

    BPlusTreeNode(bool isLeaf = false) : isLeaf(isLeaf), parent(nullptr) {}
    std::string toString();
};

class BPlusTree{
    public:
        BPlusTree(float minKey = 0.0, float maxKey = 1.0, int branchingFactor = 3) 
        : minKey(minKey), maxKey(maxKey), branchingFactor(branchingFactor), root(nullptr) {}
    
        void insert(Record* record);
        std::vector<Record*> search(float low, float high);
        void printTree();
        void deserialize(std::ifstream& inFile);
        void serialize(std::ofstream& outFile);
    
    private:
        void insertInternal(BPlusTreeNode* node, float key, BPlusTreeNode* newChild);
        BPlusTreeNode* searchNode(BPlusTreeNode* node, float key);
        void splitLeafNode(BPlusTreeNode* parent);
        void splitInternalNode(BPlusTreeNode* parent);

        void serializeNode(std::ofstream& outFile, BPlusTreeNode* node);
        BPlusTreeNode* deserializeNode(std::ifstream& inFile, BPlusTreeNode* node);
    
        BPlusTreeNode* root;
        float minKey, maxKey;
        int branchingFactor;
    };
#endif //BPLUSTREE_H