#ifndef BTREE_H
#define BTREE_H

#pragma once


#include <iostream>
#include <queue>
#include <string>

#include<queue>
using namespace std;


//class Document;


using KEY = pair<int, pair<string, int>>;
struct node
{
    int degree;
    KEY* keys;
    node** children;
    int numKeys;
    bool isLeaf;

    node(int _t = 5, bool leaf = true) : degree(_t), numKeys(0), isLeaf(leaf)
    {
        keys = new KEY[2 * degree - 1];
        children = new node * [2 * degree];

        for (int i = 0; i < 2 * degree; i++) {
            children[i] = nullptr;
        }
    }
    node(const node& other) : degree(other.degree), numKeys(other.numKeys), isLeaf(other.isLeaf) {
        keys = new KEY[2 * degree - 1];
        children = new node * [2 * degree];

        // Copy the keys array
        for (int i = 0; i < numKeys; ++i) {
            keys[i] = other.keys[i];
        }

        // Copy the children array
        for (int i = 0; i <= numKeys; ++i) {
            children[i] = nullptr;
            if (other.children[i] != nullptr) {
                children[i] = new node(*(other.children[i]));
            }
        }
    }


    int findKeyIndex(KEY key)
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

    KEY getPredecessor(node* root)
    {
        node* current = root;
        while (!current->isLeaf)
            current = current->children[current->numKeys];
        return current->keys[current->numKeys - 1];
    }

    KEY getSuccessor(node* root) {
        node* current = root;
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
        node* child = children[index];
        node* sibling = children[index + 1];
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
        node* child = children[index];
        node* sibling = children[index - 1];

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
        node* child = children[index];
        node* sibling = children[index + 1];

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

    node* getLeftSibling(int index)
    {
        if (index == 0)
            return nullptr;
        return children[index - 1];
    }

    node* getRightSibling(int index)
    {
        if (index == numKeys)
            return nullptr;
        return children[index + 1];
    }

    node* getParent(node* root, node* child)
    {
        if (root == nullptr)
            return nullptr;
        for (int i = 0; i <= root->numKeys; i++) {
            if (root->children[i] == child)
                return root;
            node* parent = getParent(root->children[i], child);
            if (parent != nullptr)
                return parent;
        }
        return nullptr;
    }

    ~node()
    {
        delete[] keys;
        delete[] children;
    }


};

class BTree
{
private:


    node* root;
    int degree;

    int height(node* root) {
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
    void print_bfs(node* root) {
        if (root == nullptr)
            return;

        int h = height();
        int currentLevel = 1;
        queue<node*> q;
        q.push(root);
        q.push(nullptr);
        bool levelChange = true;

        while (!q.empty()) {
            node* p = q.front();
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
    void printBTree(node* root, int level = 0) {
        int h = height(root);
        int currentLevel = 1;
        if (root != nullptr) {
            for (int i = 0; i < h - level; i++)
                cout << string(2 * degree - 1, ' ');
            cout << "[";
            for (int i = 0; i < root->numKeys; i++) {
                cout << root->keys[i].first;  // Assuming KEY is pair<int, Collection*>
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
    void splitChild(node* root, int i, node* child) {
        node* newChild = new node(child->degree, child->isLeaf);
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
    void insertHelper(node* root, KEY key) {
        int i = root->numKeys - 1;
        if (root->isLeaf) {
            while (i >= 0 && root->keys[i].first > key.first) {
                root->keys[i + 1] = root->keys[i];
                i--;
            }
            root->keys[i + 1] = key;
            root->numKeys++;
        }
        else {
            while (i >= 0 && root->keys[i].first > key.first)
                i--;
            if (root->children[i + 1]->numKeys == 2 * degree - 1) {
                splitChild(root, i + 1, root->children[i + 1]);
                if (root->keys[i + 1].first < key.first)
                    i++;
            }
            insertHelper(root->children[i + 1], key);
        }
    }

    node* findParent(node* root, node* child) {
        if (root == nullptr)
            return nullptr;
        for (int i = 0; i <= root->numKeys; i++) {
            if (root->children[i] == child)
                return root;
            node* parent = findParent(root->children[i], child);
            if (parent != nullptr)
                return parent;
        }
        return nullptr;
    }

    node* search(node* root, KEY key) {
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

    node* leftchild(node* root, int index) {
        return root->children[index];
    }

    node* rightchild(node* root, int index) {
        return root->children[index + 1];
    }

    int findkeyIndex(node* root, KEY key) {
        for (int i = 0; i < root->numKeys; i++)
            if (root->keys[i].first == key.first)
                return i;
    }

    void removefromNonLeaf(node* root, int index) {
        KEY key = root->keys[index];
        if (root->children[index]->numKeys >= degree) {
            KEY predecessor = root->getPredecessor(root->children[index]);
            root->keys[index] = predecessor;
            remove(root->children[index], predecessor);
        }
        else if (root->children[index + 1]->numKeys >= degree) {
            KEY successor = root->getSuccessor(root->children[index + 1]);
            root->keys[index] = successor;
            remove(root->children[index + 1], successor);
        }
        else {
            root->merge(index);
            remove(root->children[index], key);
        }
    }

    void fill(node* root, int index) {
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

    void remove(node* root, KEY key)
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
    BTree(int _t = 5) {
        root = nullptr;
        degree = _t;
    }

    BTree(const BTree& other) {
        degree = other.degree;
        root = nullptr;
        if (other.root) {
            root = copyNodes(other.root);
        }
    }

    // Assignment Operator
    BTree& operator=(const BTree& other) {
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
    node* copyNodes(node* otherRoot) {
        if (otherRoot == nullptr) {
            return nullptr;
        }

        node* newNode = new node(*otherRoot); // Copy current node

        // Recursively copy child nodes
        for (int i = 0; i <= otherRoot->numKeys; ++i) {
            newNode->children[i] = copyNodes(otherRoot->children[i]);
        }

        return newNode;
    }

    // Function to delete the tree
    void deleteTree(node* root) {
        if (root == nullptr) {
            return;
        }

        // Recursively delete child nodes
        for (int i = 0; i <= root->numKeys; ++i) 
        
        {
            deleteTree(root->children[i]);
        }

        // Delete current node
        delete root;
    }

    // Destructor
    ~BTree() {
        deleteTree(root);
    }






    void insert(KEY key) {
        if (root == nullptr) {
            root = new node(degree, true);
            root->keys[0] = key;
            root->numKeys = 1;
        }
        else {
            int maxKeys = 2 * degree - 1;
            if (root->numKeys == maxKeys) {
                node* newRoot = new node(degree, false);
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

    node* search(KEY key) {
        if (root == nullptr)
            return nullptr;
        return search(root, key);
    }

    void remove(KEY key) {
        if (root == nullptr) {
            cout << "Tree is empty" << endl;
            return;
        }
        else
            remove(root, key);
        if (root->numKeys == 0) {
            node* temp = root;
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


    KEY& getkey(int hash)
    {
        KEY null(-1, pair<string, int>("", -1));
        KEY key(hash, pair<string, int>("", -1));
        node* temp = search(key);
        if (temp != nullptr)
        {
            int index = temp->findKeyIndex(key);

            return temp->keys[index];
        }
        else
            return null;

    }

    queue<node*> getnodes()
    {
        queue<node*> nodes;
        queue<node*> temp;
        if (root != nullptr)
        {
            temp.push(root);
            while (!temp.empty())
            {
                node* current = temp.front();
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
        }
        return nodes;
    }

};

#endif // BTREE_H

