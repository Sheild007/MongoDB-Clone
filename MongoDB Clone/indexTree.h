#ifndef INDEXTREE_H
#define INDEXTREE_H


#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
using namespace std;

using Unit = pair<int, vector<pair<string, int>>>;
using Data = pair<int, pair<string, int>>;
struct ITnode
{
    int degree;
    Unit* keys;
    ITnode** children;
    int numKeys;
    bool isLeaf;

    ITnode(int _t = 5, bool leaf = true) : degree(_t), numKeys(0), isLeaf(leaf)
    {
        keys = new Unit[2 * degree - 1];
        children = new ITnode * [2 * degree];

        for (int i = 0; i < 2 * degree; i++) {
            children[i] = nullptr;
        }
    }
    ITnode(const ITnode& other) : degree(other.degree), numKeys(other.numKeys), isLeaf(other.isLeaf) {
        keys = new Unit[2 * degree - 1];
        children = new ITnode * [2 * degree];

        // Copy the keys array
        for (int i = 0; i < numKeys; ++i) {
            keys[i] = other.keys[i];
        }

        // Copy the children array
        for (int i = 0; i <= numKeys; ++i) {
            children[i] = nullptr;
            if (other.children[i] != nullptr) {
                children[i] = new ITnode(*(other.children[i]));
            }
        }
    }


    int findKeyIndex(Unit key)
    {
        int index = 0;
        while (index < numKeys && keys[index].first < key.first)
            index++;
        return index;
    }

    void traverse()
    {
        int i;
        for (i = 0; i < numKeys; i++) {
            if (!isLeaf)
                children[i]->traverse();
            cout << " " << keys[i].first;
        }

        if (!isLeaf)
            children[i]->traverse();
    }

    Unit getPredecessor(ITnode* root)
    {
        ITnode* current = root;
        while (!current->isLeaf)
            current = current->children[current->numKeys];
        return current->keys[current->numKeys - 1];
    }

    Unit getSuccessor(ITnode* root) {
        ITnode* current = root;
        while (!current->isLeaf)
            current = current->children[0];
        return current->keys[0];
    }

    void removeFromLeaf(int index)
    {
        for (int i = index + 1; i < numKeys; i++)
            keys[i - 1] = keys[i];
        numKeys--;
    }

    void merge(int index)
    {
        ITnode* child = children[index];
        ITnode* sibling = children[index + 1];
        child->keys[degree - 1] = keys[index];

        for (int i = 0; i < sibling->numKeys; i++)
            child->keys[i + degree] = sibling->keys[i];

        if (!child->isLeaf)
            for (int i = 0; i <= sibling->numKeys; i++)
                child->children[i + degree] = sibling->children[i];

        for (int i = index + 1; i < numKeys; i++)
            keys[i - 1] = keys[i];

        for (int i = index + 2; i <= numKeys; i++)
            children[i - 1] = children[i];

        child->numKeys += sibling->numKeys + 1;
        numKeys--;

        delete sibling;
    }

    void borrowFromPrev(int index)
    {
        ITnode* child = children[index];
        ITnode* sibling = children[index - 1];

        for (int i = child->numKeys - 1; i >= 0; i--) {
            child->keys[i + 1] = child->keys[i];
            if (!child->isLeaf)
                child->children[i + 1] = child->children[i];
        }

        child->keys[0] = keys[index - 1];
        if (!child->isLeaf)
            child->children[0] = sibling->children[sibling->numKeys];

        keys[index - 1] = sibling->keys[sibling->numKeys - 1];

        child->numKeys++;
        sibling->numKeys--;
    }

    void borrowFromNext(int index)
    {
        ITnode* child = children[index];
        ITnode* sibling = children[index + 1];

        child->keys[child->numKeys] = keys[index];
        if (!child->isLeaf)
            child->children[child->numKeys + 1] = sibling->children[0];

        keys[index] = sibling->keys[0];

        for (int i = 1; i < sibling->numKeys; i++)
            sibling->keys[i - 1] = sibling->keys[i];

        if (!sibling->isLeaf) {
            for (int i = 1; i <= sibling->numKeys; i++)
                sibling->children[i - 1] = sibling->children[i];
        }

        child->numKeys++;
        sibling->numKeys--;
    }

    ITnode* getLeftSibling(int index)
    {
        if (index == 0)
            return nullptr;
        return children[index - 1];
    }

    ITnode* getRightSibling(int index)
    {
        if (index == numKeys)
            return nullptr;
        return children[index + 1];
    }

    ITnode* getParent(ITnode* root, ITnode* child)
    {
        if (root == nullptr)
            return nullptr;
        for (int i = 0; i <= root->numKeys; i++) {
            if (root->children[i] == child)
                return root;
            ITnode* parent = getParent(root->children[i], child);
            if (parent != nullptr)
                return parent;
        }
        return nullptr;
    }

    ~ITnode()
    {
        delete[] keys;
        delete[] children;
    }


};

class IndexTree
{
private:


    ITnode* root;
    int degree;

    int height(ITnode* root) {
        if (root == nullptr)
            return 0;

        int h = 0;
        for (int i = 0; i < root->numKeys; i++) {
            int temp = height(root->children[i]);
            if (temp > h)
                h = temp;
        }
        return h + 1;
    }
    void print_bfs(ITnode* root) {
        if (root == nullptr)
            return;

        int h = height();
        int currentLevel = 1;
        queue<ITnode*> q;
        q.push(root);
        q.push(nullptr);
        bool levelChange = true;

        while (!q.empty()) {
            ITnode* p = q.front();
            q.pop();

            if (p == nullptr) {
                cout << endl;
                if (!q.empty()) {
                    q.push(nullptr);
                    currentLevel++;
                    levelChange = true;
                }
            }
            else {
                if (levelChange) {
                    for (int i = 0; i < h - currentLevel; i++)
                        cout << string(2 * degree - 1, ' ');

                    levelChange = false;
                }

                cout << "[";
                for (int i = 0; i < p->numKeys; i++) {
                    cout << p->keys[i].first;
                    if (i < p->numKeys - 1) {
                        cout << " ";
                    }
                }
                cout << "]";

                if (!p->isLeaf) {
                    for (int i = 0; i <= p->numKeys; i++) {
                        q.push(p->children[i]);
                    }
                }
            }
        }
    }
    void printBTree(ITnode* root, int level = 0) {
        int h = height(root);
        int currentLevel = 1;
        if (root != nullptr) {
            for (int i = 0; i < h - level; i++)
                cout << string(2 * degree - 1, ' ');
            cout << "[";
            for (int i = 0; i < root->numKeys; i++) {
                cout << root->keys[i].first;  // Assuming Unit is pair<int, Collection*>
                if (i < root->numKeys - 1) {
                    cout << " ";
                }
            }
            cout << "]";

            if (!root->isLeaf) {
                for (int i = 0; i <= root->numKeys; i++) {
                    printBTree(root->children[i], level + 1);
                }
            }
        }
    }
    void splitChild(ITnode* root, int i, ITnode* child) {
        ITnode* newChild = new ITnode(child->degree, child->isLeaf);
        newChild->numKeys = degree - 1;
        for (int j = 0; j < degree - 1; j++)
            newChild->keys[j] = child->keys[j + degree];
        if (!child->isLeaf)
            for (int j = 0; j < degree; j++)
                newChild->children[j] = child->children[j + degree];
        child->numKeys = degree - 1;
        for (int j = root->numKeys; j >= i + 1; j--)
            root->children[j + 1] = root->children[j];
        root->children[i + 1] = newChild;
        for (int j = root->numKeys - 1; j >= i; j--)
            root->keys[j + 1] = root->keys[j];
        root->keys[i] = child->keys[degree - 1];
        root->numKeys++;
    }
    void insertHelper(ITnode* root, Data key)
    {
        int i = root->numKeys - 1;
        // if root is leaf
        if (root->isLeaf)
        {
            while (i >= 0 && root->keys[i].first > key.first)
            {
                root->keys[i + 1] = root->keys[i];
                i--;
            }
            if (i >= 0 && root->keys[i].first == key.first)
            {
                root->keys[i].second.push_back(key.second);
                return;
            }
            root->keys[i + 1].first = key.first;
            root->keys[i + 1].second.clear();
            root->keys[i + 1].second.push_back(key.second);
            root->numKeys++;
        }
        // if root is not leaf
        else
        {
            while (i >= 0 && root->keys[i].first > key.first)
                i--;
            if (i >= 0 && root->keys[i].first == key.first)
            {
                root->keys[i].second.push_back(key.second);
                return;

            }
            if (root->children[i + 1]->numKeys == 2 * degree - 1)

            {
                splitChild(root, i + 1, root->children[i + 1]);
                if (root->keys[i + 1].first < key.first)
                    i++;

            }
            insertHelper(root->children[i + 1], key);
        }


    }


    ITnode* findParent(ITnode* root, ITnode* child) {
        if (root == nullptr)
            return nullptr;
        for (int i = 0; i <= root->numKeys; i++) {
            if (root->children[i] == child)
                return root;
            ITnode* parent = findParent(root->children[i], child);
            if (parent != nullptr)
                return parent;
        }
        return nullptr;
    }

    ITnode* search(ITnode* root, Unit key) {
        if (root == nullptr)
            return nullptr;
        int i = 0;
        while (i < root->numKeys && key.first > root->keys[i].first)
            i++;
        if (root->keys[i].first == key.first)
            return root;
        if (root->isLeaf)
            return nullptr;
        return search(root->children[i], key);
    }

    ITnode* leftchild(ITnode* root, int index) {
        return root->children[index];
    }

    ITnode* rightchild(ITnode* root, int index) {
        return root->children[index + 1];
    }

    int findkeyIndex(ITnode* root, Unit key) {
        for (int i = 0; i < root->numKeys; i++)
            if (root->keys[i].first == key.first)
                return i;
    }

    void removefromNonLeaf(ITnode* root, int index) {
        Unit key = root->keys[index];
        if (root->children[index]->numKeys >= degree) {
            Unit predecessor = root->getPredecessor(root->children[index]);
            root->keys[index] = predecessor;
            remove(root->children[index], predecessor);
        }
        else if (root->children[index + 1]->numKeys >= degree) {
            Unit successor = root->getSuccessor(root->children[index + 1]);
            root->keys[index] = successor;
            remove(root->children[index + 1], successor);
        }
        else {
            root->merge(index);
            remove(root->children[index], key);
        }
    }

    void fill(ITnode* root, int index) {
        if (index != 0 && root->children[index - 1]->numKeys >= degree)
            root->borrowFromPrev(index);
        else if (index != root->numKeys && root->children[index + 1]->numKeys >= degree)
            root->borrowFromNext(index);
        else {
            if (index != root->numKeys)
                root->merge(index);
            else
                root->merge(index - 1);
        }
    }

    void remove(ITnode* root, Unit key)
    {
        int index = root->findKeyIndex(key);
        if (index < root->numKeys && root->keys[index].first == key.first) {
            if (root->isLeaf)
                root->removeFromLeaf(index);
            else
                removefromNonLeaf(root, index);
        }
        else {
            if (root->isLeaf) {
                cout << "The key " << key.first << " does not exist in the tree" << endl;
                return;
            }
            bool flag = (index == root->numKeys);
            if (root->children[index]->numKeys < degree)
                fill(root, index);
            if (flag && index > root->numKeys)
                remove(root->children[index - 1], key);
            else
                remove(root->children[index], key);
        }
    }

public:
    IndexTree(int _t = 5) {
        root = nullptr;
        degree = _t;
    }

    IndexTree(const IndexTree& other) {
        degree = other.degree;
        root = nullptr;
        if (other.root) {
            root = copyNodes(other.root);
        }
    }

    // Assignment Operator
    IndexTree& operator=(const IndexTree& other) {
        if (this != &other) {
            degree = other.degree;
            deleteTree(root);
            root = nullptr;
            if (other.root) {
                root = copyNodes(other.root);
            }
        }
        return *this;
    }

    // Function to perform a deep copy of nodes
    ITnode* copyNodes(ITnode* otherRoot) {
        if (otherRoot == nullptr) {
            return nullptr;
        }

        ITnode* newNode = new ITnode(*otherRoot); // Copy current ITnode

        // Recursively copy child nodes
        for (int i = 0; i <= otherRoot->numKeys; ++i) {
            newNode->children[i] = copyNodes(otherRoot->children[i]);
        }

        return newNode;
    }

    // Function to delete the tree
    void deleteTree(ITnode* root) {
        if (root == nullptr) {
            return;
        }

        // Recursively delete child nodes
        for (int i = 0; i <= root->numKeys; ++i) {
            deleteTree(root->children[i]);
        }

        // Delete current ITnode
        delete root;
    }

    // Destructor
    ~IndexTree() {
        deleteTree(root);
    }


    void insert(Data key)
    {
        if (root == nullptr)
        {
            Unit new_key;
            new_key.first = key.first;
            new_key.second.push_back(key.second);
            root = new ITnode(degree, true);
            root->keys[0] = new_key;
            root->numKeys = 1;
        }
        else
        {
            int maxKeys = 2 * degree - 1;
            if (root->numKeys == maxKeys)
            {
                ITnode* newRoot = new ITnode(degree, false);
                newRoot->children[0] = root;
                splitChild(newRoot, 0, root);
                int i = 0;
                if (newRoot->keys[0].first < key.first)
                    i++;
                insertHelper(newRoot->children[i], key);
                root = newRoot;
            }
            else
                insertHelper(root, key);
        }
    }

    ITnode* search(Unit key) {
        if (root == nullptr)
            return nullptr;
        return search(root, key);
    }

    void remove(Unit key) {
        if (root == nullptr) {
            cout << "Tree is empty" << endl;
            return;
        }
        else
            remove(root, key);
        if (root->numKeys == 0) {
            ITnode* temp = root;
            if (root->isLeaf)
                root = nullptr;
            else
                root = root->children[0];
            delete temp;
        }
    }

    void print_tree() {
        print_bfs(root);
    }

    int height() {
        return height(root);
    }

    queue<ITnode*> getnodes()
    {
        queue<ITnode*> nodes;
        queue<ITnode*> temp;
        temp.push(root);
        while (!temp.empty())
        {
            ITnode* current = temp.front();
            temp.pop();
            nodes.push(current);
            if (!current->isLeaf)
            {
                for (int i = 0; i <= current->numKeys; i++)
                {
                    temp.push(current->children[i]);
                }
            }
        }
        return nodes;
    }

    vector<pair<string, int>> getValues(int key)
    {
        ITnode* current = search(Unit(key, vector<pair<string, int>>()));
        if (current == nullptr)
            return vector<pair<string, int>>();
        else
        {
            for (int i = 0; i < current->numKeys; i++)
            {
                if (current->keys[i].first == key)
                    return current->keys[i].second;
            }
        }

    }

};
#endif // !INDEXTREE_H