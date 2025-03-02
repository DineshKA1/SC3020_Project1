#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iomanip>
#include "storage.h"
#include "bPlusTree.h"
#include "const.h"


void BPlusTree::insert(Record* record) {
    float key = record->FG_PCT_home;
    if(DEBUG_MODE) 
        std::cout<<"Inserting Record:"<<record->toString() << ", with key = " << key << std::endl;

    if(root == nullptr){
        root = new BPlusTreeNode(true); //create root as leaf node
    }

    BPlusTreeNode* node = root;
    while(!node->isLeaf){
        int index = std::lower_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();
        node = node->children[index];
    }

    node->keys.push_back(key);
    node->records.push_back(new Record(*record));

    std::sort(node->keys.begin(), node->keys.end());

    //split if the leaf node overflows
    if(node->keys.size() > branchingFactor){
        splitLeafNode(node);
    }

    if(DEBUG_MODE) 
        std::cout<<"End of Inserting Record:"<<record->toString() << ", with key = " << key << std::endl;
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
    while (!currentLevel.empty()) {
        std::cout << "Level " << level << ": ";
        for (BPlusTreeNode* node : currentLevel) {
            std::cout << '[';
            if (node->isLeaf) {
                for (Record* record : node->records) {
                    std::cout << record->toString() << " ";
                }
            } else {
                for (float key : node->keys) {
                    std::cout << key << " ";
                }
            }
            std::cout << ']';
            for (BPlusTreeNode* child : node->children) {
                nextLevel.push_back(child);
            }
        }
        std::cout << std::endl;
        currentLevel = nextLevel;
        nextLevel.clear();
        level++;
    }
}