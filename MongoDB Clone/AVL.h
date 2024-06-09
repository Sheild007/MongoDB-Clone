#pragma once
#ifndef AVL_H
#define AVL_H

#pragma once
#include<iostream>
#include<queue>
using namespace std;
template <typename T>
struct Node
{
    T data;
    int height;

    Node<T>* left;
    Node<T>* right;
    Node(T data)
    {
        this->data = data;
        this->height = 1;
        this->left = nullptr;
        this->right = nullptr;

    }





};
template <typename T>
class AVL
{
public:
    Node<T>* root;
    int height(Node<T>* r)
    {
        if (r == nullptr)
            return 0;
        return r->height;
    }
    Node<T>* leftRotate(Node<T>* r)
    {
        Node<T>* newRoot = r->right;
        r->right = newRoot->left;
        newRoot->left = r;
        r->height = max(height(r->left), height(r->right)) + 1;
        newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
        return newRoot;
    }
    Node<T>* rightRotate(Node<T>* r)
    {
        Node<T>* newRoot = r->left;
        r->left = newRoot->right;
        newRoot->right = r;
        r->height = max(height(r->left), height(r->right)) + 1;
        newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
        return newRoot;
    }
    int getBalance(Node<T>* r)
    {
        if (r == nullptr)
            return 0;
        return height(r->left) - height(r->right);
    }
    void print_tree(Node<T>* temp, int t)
    {
        if (temp == nullptr)
            return;

        print_tree(temp->right, t + 1);

        for (int i = 0; i < t; i++)
            cout << "\t";
        cout << temp->data << endl;

        print_tree(temp->left, t + 1);
    }
    /*Node<T>* insertRecursive(Node<T>* current, Node<T>* newNode)
    {
        if (current == nullptr)
        {
            return newNode;
        }
        if (newNode->data < current->data)
        {
            current->left = insertRecursive(current->left, newNode);
        }
        else if (newNode->data > current->data)
        {
            current->right = insertRecursive(current->right, newNode);
        }

        current->height = 1 + max(height(current->left), height(current->right));

        int balance = getBalance(current);

        if (balance > 1 && newNode->data < current->left->data)
            return rightRotate(current);
        if (balance < -1 && newNode->data > current->right->data)
            return leftRotate(current);
        if (balance > 1 && newNode->data > current->left->data)
        {
            current->left = leftRotate(current->left);
            return rightRotate(current);
        }
        if (balance < -1 && newNode->data < current->right->data)
        {
            current->right = rightRotate(current->right);
            return leftRotate(current);
        }

        return current;
    }*/
    Node<T>* findRecursive(Node<T>* current, T value)
    {
        //base case
        if (current == nullptr || current->data == value)
            return current;

        //go to left subtree
        if (value < current->data)
            return findRecursive(current->left, value);
        //go to right subtree
        else
            return findRecursive(current->right, value);

    }

    Node<T>* insertRecursive(Node<T>* current, Node<T>* newNode)
    {
        // base case
        if (current == nullptr)
            return newNode;

        //go to left subtree
        if (newNode->data < current->data)
            current->left = insertRecursive(current->left, newNode);

        //go to right subtree
        else if (newNode->data > current->data)
            current->right = insertRecursive(current->right, newNode);

        //  height update
        current->height = 1 + max(height(current->left), height(current->right));

        return Balnce(current);
    }
    Node<T>* minValueNode(Node<T>* node)
    {
        Node<T>* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }
    Node<T>* Balnce(Node<T>* current)
    {
        current->height = 1 + max(height(current->left), height(current->right));

        int balance = getBalance(current);
        if (balance == 2)
        {
            if (height(current->left->left) >= height(current->left->right))
                return rightRotate(current);
            else
            {
                current->left = leftRotate(current->left);
                return rightRotate(current);
            }

        }
        if (balance == -2)
        {
            if (height(current->right->right) >= height(current->right->left))
                return leftRotate(current);
            else
            {
                current->right = rightRotate(current->right);
                return leftRotate(current);
            }
        }
        return current;
    }
    Node<T>* deleteNode(Node<T>* root, T value)
    {
        if (root == NULL)
            return root;
        if (value < root->data)
            root->left = deleteNode(root->left, value);
        else if (value > root->data)
            root->right = deleteNode(root->right, value);
        else
        {
            if (root->left == nullptr)
            {
                Node<T>* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                Node<T>* temp = root->left;
                delete root;
                return temp;
            }
            root->data = minValueNode(root->right)->data;
            root->right = deleteNode(root->right, root->data);

        }
        return Balnce(root);
    }
    void print_inorder_recursive(Node<T>* r)
    {
        if (r == nullptr)
            return;
        print_inorder_recursive(r->left);
        cout << r->data << " ";
        print_inorder_recursive(r->right);

    }


    AVL()
    {
        root = nullptr;
    }
    Node<T>* insert(T data)
    {
        Node<T>* temp = new Node<T>(data);
        if (root == nullptr)
        {
            root = temp;
            return root;
        }
        root = insertRecursive(root, temp);
        return root;
    }
    void Delete(T value)
    {
        root = deleteNode(root, value);

    }
    int max_deptht()
    {
        return height(root);
    }
    T get_root_data()
    {
        return root->data;
    }
    Node<T>* find(T value)
    {
        return findRecursive(root, value);
    }
    Node<T>* get_root()
    {
		return root;
	}

    void print()
    {
        print_tree(root, 0);

    }
    void print_BFS()
    {
        queue<Node<T>*> q;
        q.push(root);
        while (!q.empty())
        {
            Node<T>* temp = q.front();
            q.pop();
            if (temp == nullptr)
                cout << "null ";
            else
            {
                cout << temp->data << " ";
                q.push(temp->left);
                q.push(temp->right);
            }

        }
    }
    void print_heights()
    {
        queue<Node<T>*> q;
        q.push(root);
        while (!q.empty())
        {
            Node<T>* temp = q.front();
            q.pop();
            if (temp == nullptr)
                cout << 0 << " ";
            else
            {
                cout << temp->height << " ";
                q.push(temp->left);
                q.push(temp->right);
            }

        }
    }
    void print_inorder()
    {
        print_inorder_recursive(root);
    }
    ~AVL() {
        if (root != nullptr) {
            deleteEntireTree(root);
        }
    }

    // Function to delete entire tree
    void deleteEntireTree(Node<T>* current) {
        if (current == nullptr) {
            return;
        }
        deleteEntireTree(current->left);
        deleteEntireTree(current->right);
        delete current;
    }

    
};
#endif // !AVL_H