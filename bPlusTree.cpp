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
        root = new BPlusTreeNode(true); // Create root as a leaf node
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


void BPlusTree::serialize(const std::string& filename){
    std::ofstream file(filename, std::ios::binary);
    if (!file){
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }
    serializeNode(file,root);
    file.close();
}

void BPlusTree::serializeNode(std::ofstream& file, BPlusTreeNode* node) {
    if (!node) return;
    
    file.write(reinterpret_cast<char*>(&node->isLeaf), sizeof(bool));
    
    size_t keySize = node->keys.size();
    file.write(reinterpret_cast<char*>(&keySize), sizeof(size_t));
    file.write(reinterpret_cast<char*>(node->keys.data()), keySize * sizeof(float));

    if (node->isLeaf) {
        size_t recordSize = node->records.size();
        file.write(reinterpret_cast<char*>(&recordSize), sizeof(size_t));
        for (auto* record : node->records) {
            file.write(reinterpret_cast<char*>(record), sizeof(Record));
        }
    } else {
        size_t childSize = node->children.size();
        file.write(reinterpret_cast<char*>(&childSize), sizeof(size_t));
        for (auto* child : node->children) {
            serializeNode(file, child);
        }
    }
}



void BPlusTree::deserialize(const std::string& filename){
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
        }
    root = deserializeNode(file, nullptr);
    file.close();
}

BPlusTreeNode* deserializeNode(std::ifstream& file, BPlusTreeNode* parent){
    if (file.eof()) return nullptr;

    bool isLeaf;
    file.read(reinterpret_cast<char*>(&isLeaf), sizeof(bool));
    if (file.fail()) return nullptr;

    BPlusTreeNode* node = new BPlusTreeNode(isLeaf);
    node->parent = parent;

    size_t keySize;
    file.read(reinterpret_cast<char*>(&keySize), sizeof(size_t));
    if (file.fail()) return nullptr;

    node->keys.resize(keySize);
    file.read(reinterpret_cast<char*>(node->keys.data()), keySize * sizeof(float));
    
    if (node->isLeaf) {

        size_t recordSize;
        file.read(reinterpret_cast<char*>(&recordSize), sizeof(size_t));
        if (file.fail()) return nullptr;
        
        for (size_t i=0; i < recordSize; i++){

            Record* record = new Record();
            file.read(reinterpret_cast<char*>(record), sizeof(Record));
            if (file.fail()) {
                delete record;
                return nullptr;
            }
            node->records.push_back(record);
        }
    }
    else {

        size_t childSize;
        file.read(reinterpret_cast<char*>(&childSize), sizeof(size_t));
        if (file.fail()) return nullptr;

        for (size_t i=0; i < childSize; i++) {
            BPlusTreeNode* child = deserializeNode(file, node);
            if (child){
                node->children.push_back(child);
            }
        }

    }
    return node;
}




// void BPlusTree::serialize(std::ofstream& outFile) {
//     if (!outFile.is_open()) {
//         throw std::runtime_error("File not open for writing");
//     }
    
//     // 递归序列化从根节点开始
//     serializeNode(outFile, root);
// }

// void BPlusTree::serializeNode(std::ofstream& outFile, BPlusTreeNode* node) {
//     // 写入节点类型（叶子节点或内部节点）
//     bool isLeaf = node->isLeaf;
//     outFile.write(reinterpret_cast<const char*>(&isLeaf), sizeof(isLeaf));

//     // 写入关键字数量和关键字列表
//     int keyCount = node->keys.size();
//     outFile.write(reinterpret_cast<const char*>(&keyCount), sizeof(keyCount));
//     for (float key : node->keys) {
//         outFile.write(reinterpret_cast<const char*>(&key), sizeof(key));
//     }

//     if (isLeaf) {
//         // 叶子节点：写入记录
//         int recordCount = node->records.size();
//         outFile.write(reinterpret_cast<const char*>(&recordCount), sizeof(recordCount));
//         for (Record* record : node->records) {
//             record->serialize(outFile); // 假设 Record 类有 serialize 函数
//         }

//         // 写入 children[0]（即 next）是否存在
//         bool hasNext = !node->children.empty() && node->children[0] != nullptr;
//         outFile.write(reinterpret_cast<const char*>(&hasNext), sizeof(hasNext));
//     } else {
//         // 内部节点：写入子节点
//         int childCount = node->children.size();
//         outFile.write(reinterpret_cast<const char*>(&childCount), sizeof(childCount));
//         for (BPlusTreeNode* child : node->children) {
//             serializeNode(outFile, child); // 递归写入子节点
//         }
//     }
// }


// void BPlusTree::deserialize(std::ifstream& inFile) {
//     if (!inFile.is_open()) {
//         throw std::runtime_error("File not open for reading");
//     }

//     // 递归反序列化从根节点开始
//     root = deserializeNode(inFile, nullptr);
// }
// BPlusTreeNode* BPlusTree::deserializeNode(std::ifstream& inFile, BPlusTreeNode* parent) {
//     // 读取节点类型
//     bool isLeaf;
//     inFile.read(reinterpret_cast<char*>(&isLeaf), sizeof(isLeaf));
//     std::cout << "isLeaf: " << isLeaf << std::endl;

//     BPlusTreeNode* node = new BPlusTreeNode(isLeaf);
//     node->parent = parent;

//     // 读取关键字数量和关键字列表
//     int keyCount;
//     inFile.read(reinterpret_cast<char*>(&keyCount), sizeof(keyCount));
//     node->keys.resize(keyCount);
//     for (int i = 0; i < keyCount; ++i) {
//         inFile.read(reinterpret_cast<char*>(&node->keys[i]), sizeof(node->keys[i]));
//     }

//     if (isLeaf) {
//         // 读取记录
//         int recordCount;
//         inFile.read(reinterpret_cast<char*>(&recordCount), sizeof(recordCount));
//         node->records.resize(recordCount);
//         for (int i = 0; i < recordCount; ++i) {
//             node->records[i] = new Record();
//             node->records[i]->deserialize(inFile); // 假设 Record 类有 deserialize 函数
//         }

//         // 读取 children[0]（即 next）是否存在
//         bool hasNext;
//         inFile.read(reinterpret_cast<char*>(&hasNext), sizeof(hasNext));
//         // if (hasNext) {
//         //     node->children.push_back(deserializeNode(inFile, node)); // 递归读取下一个叶子节点
//         // } else {
//         //     node->children.push_back(nullptr);
//         // }
//     } else {
//         // 读取子节点
//         int childCount;
//         inFile.read(reinterpret_cast<char*>(&childCount), sizeof(childCount));
//         node->children.resize(childCount);
//         for (int i = 0; i < childCount; ++i) {
//             node->children[i] = deserializeNode(inFile, node); // 递归读取子节点
//         }
//     }

//     std::cout << "Node: " << node->toString() << std::endl;
//     return node;
// }

