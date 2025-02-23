#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iomanip>
#include "NBARecord.h"

struct BPlusTreeNode
{
    bool isLeaf;
    std::vector<float> keys;
    std::vector<BPlusTreeNode*> children;
    std::vector<NBARecord*> records;
    BPlusTreeNode* parent;

    BPlusTreeNode(bool isLeaf = false) : isLeaf(isLeaf), parent(nullptr) {}
};

class BPlusTree{
    public:
        BPlusTree(float minKey = 0.0, float maxKey = 1.0, int branchingFactor = 3) 
        : minKey(minKey), maxKey(maxKey), branchingFactor(branchingFactor), root(nullptr) {}
    
        void insert(NBARecord* record);
        std::vector<NBARecord*> search(float low, float high);
        void printTree();
    
    private:
        void insertInternal(BPlusTreeNode* node, float key, NBARecord* record);
        BPlusTreeNode* searchNode(BPlusTreeNode* node, float key);
        void splitLeafNode(BPlusTreeNode* parent, int index);
        void splitInternalNode(BPlusTreeNode* parent, int index);
    
        BPlusTreeNode* root;
        float minKey, maxKey;
        int branchingFactor;
    };