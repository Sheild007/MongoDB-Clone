

#pragma once
#ifndef TRIES_H
#define TRIES_H



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<stdexcept>
#include<queue>

using namespace std;

#define K_WAY 36

extern int getIndex(char ch)
{
    if (ch >= '0' && ch <= '9') {
        return ch - '0' + 26; // Map numbers (0-9) to indices 26-35
    }
    else if (ch >= 'a' && ch <= 'z') {
        return ch - 'a'; // Map alphabets (a-z) to indices 0-25
    }


}
extern char getChar(int index)
{
    if (index >= 0 && index <= 25) {
        return 'a' + index; // Map indices 0-25 to alphabets (a-z)
    }
    else if (index >= 26 && index <= 35) {
        return '0' + index - 26; // Map indices 26-35 to numbers (0-9)
    }

}

class trieNode
{
    trieNode* next[K_WAY]{};
    bool hasEnded;


public:
    trieNode()
    {
        hasEnded = false;
    }
    void insert(char ch, bool state)
    {
        int index = getIndex(ch);
        if (next[index] == nullptr)
            next[index] = new trieNode();

        if (!next[index]->hasEnded)
            next[index]->hasEnded = state;
    }
    trieNode* getNode(char ch)
    {
        return next[getIndex(ch)];
    }
    bool isEnd()
    {
        return hasEnded;
    }
    void update(bool state)
    {
        hasEnded = state;
    }
    ~trieNode()
    {
        for (int i = 0; i < K_WAY; i++)
        {
            delete next[i];
        }
    }
    friend class trie;
};



class trie
{
    trieNode* root;


    void autoComplete(trieNode* node, string word, vector<string>& suggestions)
    {
        if (node == nullptr)
            return;

        if (node->isEnd())
            suggestions.push_back(word);

        for (int i = 0; i < K_WAY; i++)
        {
            if (node->next[i] != nullptr)
            {
                char ch = getChar(i);
                autoComplete(node->next[i], word + ch, suggestions);
            }
        }
    }
    void print(trieNode* node, int level)
    {

        if (node == nullptr)
            return;

        for (int i = 0; i < K_WAY; i++)
        {
            if (node->next[i] != nullptr)
            {


                char ch = getChar(i);
                for (int j = 0; j < level; j++)
                    cout << "    ";

                cout << ch << endl;
                print(node->next[i], level + 1);
            }
        }
    }
    void printWordsUtil(trieNode* node, string word)
    {
        if (node == nullptr)
            return;

        if (node->isEnd()) {
            cout << word << endl;
        }

        for (int i = 0; i < K_WAY; i++)
        {
            if (node->next[i] != nullptr)
            {

                char ch = getChar(i);
                printWordsUtil(node->next[i], word + ch);
            }
        }
    }
 
    void copyNodes(trieNode* dest, trieNode* src) 
    {
        if (src == nullptr)
            return;

        dest->hasEnded = src->hasEnded;

        for (int i = 0; i < K_WAY; ++i) {
            if (src->next[i] != nullptr) {
                dest->next[i] = new trieNode();
                copyNodes(dest->next[i], src->next[i]);
            }
        }
    }

    // Recursively clear nodes starting from 'node'
    void clearNodes(trieNode* node) 
    {
        if (node == nullptr)
            return;

        for (int i = 0; i < K_WAY; ++i) {
            clearNodes(node->next[i]);
        }
        delete node;
    }

public:
    trie()
    {
        root = nullptr;
    }
    trie(ifstream& Rdr) : root(new trieNode())
    {
        string word;
        while (getline(Rdr, word))
            insert(word);

    }
  
    trie(const trie& other) 
    {
      if(other.root==nullptr)
		  root=nullptr;
      else
      {
          root = new trieNode();
          copyNodes(root, other.root);
      }
    }

    // Assignment operator
    trie& operator=(const trie& other) 
    {
        if (other.root == nullptr)
        {
			root=nullptr;
			return *this;
		}
        if (this != &other) 
        { 
            clearNodes(root);

            root = new trieNode(); // Create a new root for the assigned trie

            // Recursively copy nodes
            copyNodes(root, other.root);
        }
        return *this;
    }


    void print()
    {
        print(root, 0);
    }

    void printWords() {
        string word = ""; // Initialize an empty string
        printWordsUtil(root, word);
    }


    void insert(const string& key) {
        if (root == nullptr) {
            root = new trieNode();
        }
        trieNode* node = root;
        for (char ch : key) {
            int index = getIndex(ch);
            if (index < 0 || index >= K_WAY) {
                // Handle invalid characters here, if required
                continue; // Skip invalid characters
            }
            if (node->next[index] == nullptr) {
                node->next[index] = new trieNode();
            }
            node = node->next[index];
        }
        node->hasEnded = true;
    }

    bool search(string key)
    {
        trieNode* node = root;
        if(node==nullptr)
			return false;
        for (int i = 0; i < key.length(); i++)
        {
            char ch = key[i];
            node = node->getNode(ch);
            if (node == nullptr)
                return false;

        }
        return node->isEnd();
    }
    void erase(string key)
    {
        trieNode* node = root;
        vector<trieNode*> path;
        for (int i = 0; i < key.length(); i++)
        {
            char ch = key[i];
            node = node->getNode(ch);
            if (node == nullptr)
                return;
            path.push_back(node);
        }

        node->update(false);

        for (int i = path.size() - 1; i > 0; i--)
        {
            trieNode* currentNode = path[i];
            char ch = key[i];

            if (currentNode->isEnd() || currentNode->getNode(ch) != nullptr)
                break;

            trieNode* parent = path[i - 1];
            parent->next[getIndex(ch)] = nullptr;
            delete currentNode;
        }
    }

    ~trie()
    {
        delete root;
    }

    vector<string> wordSuggestTool(string prefix)
    {
        vector<string> suggestions;

        trieNode* node = root;
        for (int i = 0; i < prefix.length(); i++)
        {
            char ch = prefix[i];
            node = node->getNode(ch);
            if (node == nullptr)
                return suggestions;

        }
        autoComplete(node, prefix, suggestions);


        return suggestions;
    }
};

#endif // TRIES_H





