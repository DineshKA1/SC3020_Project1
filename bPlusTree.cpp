#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iomanip>
#include "storage.h"
#include "bPlusTree.h"
#include "const.h"


void BPlusTree::insert(Record* record) {
    float key = record->FG_PCT_home;
    if (DEBUG_MODE) 
        std::cout << "Inserting Record: " << record->toString() << ", with key = " << key << std::endl;

    if (root == nullptr) {
        root = new BPlusTreeNode(true);
    }

    BPlusTreeNode* node = root;
    while (!node->isLeaf) {
        int index = std::lower_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();
        node = node->children[index];
    }

    node->keys.push_back(key);
    node->records.push_back(new Record(*record));
    std::vector<std::pair<float, Record*>> keyRecordPairs;
    for (size_t i = 0; i < node->keys.size(); ++i) {
        keyRecordPairs.push_back({node->keys[i], node->records[i]});
    }
    std::sort(keyRecordPairs.begin(), keyRecordPairs.end());

    node->keys.clear();
    node->records.clear();
    for (auto& pair : keyRecordPairs) {
        node->keys.push_back(pair.first);
        node->records.push_back(pair.second);
    }

    if (node->keys.size() > branchingFactor) {
        splitLeafNode(node);
    }
    if (DEBUG_MODE) 
        std::cout << "End of Inserting Record: " << record->toString() << ", with key = " << key << std::endl;
}


void BPlusTree::insertInternal(BPlusTreeNode* node, float key, BPlusTreeNode* newChild) {
    if(DEBUG_MODE) {
        std::cout << "Inserting internal node with key: " << key << std::endl;
    }
    int index = std::lower_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();
    node->keys.insert(node->keys.begin() + index, key);
    node->children.insert(node->children.begin() + index + 1, newChild);

    if (node->keys.size() > branchingFactor) {
        splitInternalNode(node); 
    }
    if(DEBUG_MODE) {
        std::cout << "End of inserting internal node with key: " << key << std::endl;
    }
}


void BPlusTree::splitLeafNode(BPlusTreeNode* node) {
    if(DEBUG_MODE) {
        std::cout << "Splitting leaf node with key:";
        for(auto key: node->keys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;
    }
    BPlusTreeNode* newLeaf = new BPlusTreeNode(true);
    newLeaf->parent = node->parent;

    int mid = node->keys.size() / 2;
    newLeaf->keys.assign(node->keys.begin() + mid, node->keys.end());
    newLeaf->records.assign(node->records.begin() + mid, node->records.end());
    node->keys.resize(mid);
    node->records.resize(mid);

    if(!node->children.empty())  {
        newLeaf->children.push_back(node->children[0]);
        node->children[0] = newLeaf;
    } else {
        node->children.push_back(newLeaf);
    }

    if (node->parent == nullptr) {
        root = new BPlusTreeNode(false); 
        root->keys.push_back(newLeaf->keys[0]);
        root->children.push_back(node);
        root->children.push_back(newLeaf);
        node->parent = root;
        newLeaf->parent = root;
    } else {
        insertInternal(node->parent, newLeaf->keys[0], newLeaf);
    }
    if(DEBUG_MODE) {
        std::cout << "End of splitting leaf node" << std::endl;
    }
}

void BPlusTree::splitInternalNode(BPlusTreeNode* node) {
    if(DEBUG_MODE) {
        std::cout << "Splitting internal node with key:";
        for(auto key: node->keys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;
    }
    BPlusTreeNode* newInternal = new BPlusTreeNode(false);
    newInternal->parent = node->parent;

    int mid = node->keys.size() / 2;
    newInternal->keys.assign(node->keys.begin() + mid + 1, node->keys.end()); 
    newInternal->children.assign(node->children.begin() + mid + 1, node->children.end()); 

    float midKey = node->keys[mid];
    node->keys.resize(mid);
    node->children.resize(mid + 1);

    for (BPlusTreeNode* child : newInternal->children) {
        child->parent = newInternal;
    }

    if (node->parent == nullptr) {
        root = new BPlusTreeNode(false);  
        root->keys.push_back(midKey);    
        root->children.push_back(node);
        root->children.push_back(newInternal);
        node->parent = root;
        newInternal->parent = root;
    } else {
        insertInternal(node->parent, midKey, newInternal);
    }
    if(DEBUG_MODE) {
        std::cout << "End of splitting internal node" << std::endl;
    }
}

void BPlusTree::printTree() {
    if (root == nullptr) {
        std::cout << "Empty tree." << std::endl;
        return;
    }
    std::cout << "Printing tree..." << std::endl;
    std::vector<BPlusTreeNode*> currentLevel;
    std::vector<BPlusTreeNode*> nextLevel;
    currentLevel.push_back(root);
    int level = 0;
    int totRecord = 0;
    while (!currentLevel.empty()) {
        std::cout << "Level " << level << ": ";
        for (BPlusTreeNode* node : currentLevel) {
            std::cout << '[';
            if (node->isLeaf) {
                for (Record* record : node->records) {
                    std::cout << record->toString() << " ";
                    totRecord++;
                }
            } else {
                std::cout << "DEBUG_PRINT_TREE:";
                for (BPlusTreeNode* child : node->children) {
                    std::cout << child << " ";
                }
                for (float key : node->keys) {
                    std::cout << key << " ";
                }
                for (BPlusTreeNode* child : node->children) {
                    nextLevel.push_back(child);
                }
            }
            std::cout << ']';
            
        }
        std::cout << std::endl;
        currentLevel = nextLevel;
        nextLevel.clear();
        level++;
    }
    std::cout << "Total records: " << totRecord << std::endl;
    std::cout << "End of printing tree." << std::endl;
}

std::vector<Record*> BPlusTree::search(float low, float high) {
    std::vector<Record*> result;
    if (root == nullptr) {
        return result;
    }

    BPlusTreeNode* node = root;
    while (!node->isLeaf) {
        int index = std::lower_bound(node->keys.begin(), node->keys.end(), low) - node->keys.begin();
        node = node->children[index];
    }
    if(DEBUG_MODE) {
        std::cout << "First leaf node with key:";
        for(auto key: node->keys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;
    }

    while (node != nullptr) {
        for (size_t i = 0; i < node->keys.size(); ++i) {
            float key = node->keys[i];
            if (key >= low && key <= high) {
                result.push_back(node->records[i]);
            } else if (key > high) {
                return result; 
            }
        }
        if (!node->children.empty()) {
            node = node->children[0];
        } else {
            node = nullptr;
        }
    }

    return result;
}

std::string BPlusTreeNode::toString() {
    std::string result;
    if (isLeaf) {
        for (Record* record : records) {
            result += record->toString() + " ";
        }
    } else {
        for (float key : keys) {
            result += std::to_string(key) + " ";
        }
    }
    return result;
}

void BPlusTree::serializeRecord(std::ofstream& outFile, Record* record) const {
    std::string serializedRecord = record->serialize();
    size_t record_size = serializedRecord.size();
    outFile.write(reinterpret_cast<const char*>(&record_size), sizeof(record_size));
    outFile.write(serializedRecord.c_str(), record_size);
}

Record* BPlusTree::deserializeRecord(std::ifstream& inFile) const {
    size_t record_size;
    inFile.read(reinterpret_cast<char*>(&record_size), sizeof(record_size));

    std::vector<char> buffer(record_size + 1);
    inFile.read(buffer.data(), record_size);
    buffer[record_size] = '\0'; 
    std::string serializedRecord(buffer.data());
    Record* record = new Record();
    *record = Record::deserialize(serializedRecord); 
    return record;
}

void BPlusTree::serialize(const std::string& filename) const {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Unable to open file for saving B+ tree.");
    }
    serializeNode(outFile, root);
    outFile.close();
}

void BPlusTree::serializeNode(std::ofstream& outFile, BPlusTreeNode* node) const {
    bool isLeaf = node->isLeaf;
    outFile.write(reinterpret_cast<const char*>(&isLeaf), sizeof(isLeaf));

    int keyCount = node->keys.size();
    outFile.write(reinterpret_cast<const char*>(&keyCount), sizeof(keyCount));
    for (float key : node->keys) {
        outFile.write(reinterpret_cast<const char*>(&key), sizeof(key));
    }

    if (isLeaf) {
        int recordCount = node->records.size();
        outFile.write(reinterpret_cast<const char*>(&recordCount), sizeof(recordCount));
        for (Record* record : node->records) {
            serializeRecord(outFile, record);
        }
    } else {
        int childCount = node->children.size();
        outFile.write(reinterpret_cast<const char*>(&childCount), sizeof(childCount));
        for (BPlusTreeNode* child : node->children) {
            serializeNode(outFile, child);  
        }
    }
}

void BPlusTree::deserialize(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Unable to open file for loading B+ tree.");
    }

    std::vector<BPlusTreeNode*> leafNodes;  
    root = deserializeNode(inFile, nullptr, leafNodes);
    for (size_t i = 0; i < leafNodes.size() - 1; ++i) {
        leafNodes[i]->children.push_back(leafNodes[i + 1]); 
    }
    inFile.close();
}


BPlusTreeNode* BPlusTree::deserializeNode(std::ifstream& inFile, BPlusTreeNode* parent, std::vector<BPlusTreeNode*>& leafNodes) {
    bool isLeaf;
    static int count = 0;
    count++;
    inFile.read(reinterpret_cast<char*>(&isLeaf), sizeof(isLeaf));
    BPlusTreeNode* node = new BPlusTreeNode(isLeaf);
    node->parent = parent;
    int keyCount;
    inFile.read(reinterpret_cast<char*>(&keyCount), sizeof(keyCount));
    node->keys.resize(keyCount);
    for (int i = 0; i < keyCount; ++i) {
        inFile.read(reinterpret_cast<char*>(&node->keys[i]), sizeof(node->keys[i]));
    }

    if (isLeaf) {
        int recordCount;
        inFile.read(reinterpret_cast<char*>(&recordCount), sizeof(recordCount));
        node->records.resize(recordCount);
        for (int i = 0; i < recordCount; ++i) {
            node->records[i] = deserializeRecord(inFile); 
        }

        leafNodes.push_back(node);
    } else {
        int childCount;
        inFile.read(reinterpret_cast<char*>(&childCount), sizeof(childCount));
        node->children.resize(childCount);
        for (int i = 0; i < childCount; ++i) {
            node->children[i] = deserializeNode(inFile, node, leafNodes); 
        }
    }
    return node;
}

void BPlusTree::printStatics() {
    std::vector<BPlusTreeNode*> currentLevel;
    std::vector<BPlusTreeNode*> nextLevel;
    currentLevel.push_back(root);
    int level = 0;
    int totRecord = 0;
    int count = 0;
    while (!currentLevel.empty()) {
        count += currentLevel.size();
        for (BPlusTreeNode* node : currentLevel) {
            if (node->isLeaf) {
                totRecord += node->records.size();
            } else {
                for (BPlusTreeNode* child : node->children) {
                    nextLevel.push_back(child);
                }
            }
        }
        currentLevel = nextLevel;
        nextLevel.clear();
        level++;
    }

    std::cout << "the parameter n of the B+ tree is: " << branchingFactor << std::endl;
    std::cout << "the number of nodes of the B+ tree: " << count << std::endl;
    std::cout << "the number of levels in the B+ tree is: " << level << std::endl;
    std::cout << "the content of the root node: [";
    for (float key : root->keys) {
        if(key == root->keys.back()) {
            std::cout << key;
        } else {
            std::cout << key << ",";
        }
    }
    std::cout << ']' << std::endl;
}
