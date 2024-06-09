#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H



#pragma once

#include <iostream>
#include <string>
#include <utility>

using namespace std;
#define INF INT_MAX

template<class K, class T>
class unordered_Map
{
private:
    struct Node
    {
        K key;
        T value;
        bool occupied;
        Node()
        {
            occupied = false;


        }

    };

    Node* arr;
    int _size;
    int capacity;
    const double loadFactorThreshold = 0.75;

    int hashFunction(K key)
    {
		return std::hash<K>{}(key) % capacity;
	}
    void reHash()
    {
        int oldCapacity = capacity;
        capacity = capacity * 2;
        Node* newArr = new Node[capacity];

        for (int i = 0; i < oldCapacity; i++)
        {
            if (arr[i].occupied)
            {
                int index = hashFunction(arr[i].key);
                while (newArr[index].occupied)
                {
                    index = collide(index, 1);
                }
                newArr[index].key = arr[i].key;
                newArr[index].value = arr[i].value;
                newArr[index].occupied = true;
            }
        }
        delete[] arr;
        arr = newArr;
    }
    virtual int collide(int i, int j)
    {
        return (i + j) % capacity;
    }
    int getIndexOfKey(K key)
    {
        int index = hashFunction(key);
        int j = 0;

        while (arr[index].occupied && arr[index].key != key)
            index = collide(index, ++j);

        return index;

    }

public:
    unordered_Map() : _size(0), capacity(1000)
    {
        arr = new Node[capacity];
    }
    unordered_Map(int capacity) : _size(0), capacity(capacity)
    {
        arr = new Node[capacity];
    }
    unordered_Map(const unordered_Map& other) : _size(other._size), capacity(other.capacity)
    {
        arr = new Node[capacity];
        for (int i = 0; i < capacity; i++)
        {
            arr[i] = other.arr[i];
        }
    }

    T& operator[](K key)
    {
        int index = getIndexOfKey(key);
        if (index == -1)
        {
            throw "Key not found";
        }

        if (!arr[index].occupied)
        {
            arr[index].key = key;
            arr[index].occupied = true;
            _size++;
        }


        return arr[index].value;
    }
    const T& operator[](K key) const {
        int index = getIndexOfKey(key);
        if (index == -1) {
            throw "Key not found";
        }

        if (!arr[index].occupied) {
            arr[index].key = key;
            arr[index].occupied = true;
            ++_size;
        }

        return arr[index].value;
    }

    T* find(K key) {
        int index = hashFunction(key);
        int j = 0;

        while (arr[index].occupied && arr[index].key != key) {
            index = collide(index, ++j);
        }

        if (arr[index].occupied) {
            return &arr[index].value;
        }
        else {
            return nullptr;
        }
    }

    void insert(std::pair<K, T> entry)
    {
        double currentLoad = static_cast<double>(_size) / capacity;
        if (currentLoad >= loadFactorThreshold)
            reHash();

        (*this)[entry.first] = entry.second;
    }

    bool erase(K key)
    {
        int index = getIndexOfKey(key);
        if (index == -1)
        {
            throw "Key not found";
        }
        else
        {
            if (arr[index].occupied && arr[index].key == key)
            {
                arr[index].occupied = false;
                _size--;
                return true;
            }
            return false;
        }
    }

    int size()
    {
        return _size;
    }

    void print()
    {
        for (int i = 0; i < capacity; i++)
        {
            if (arr[i].occupied)
            {
                std::cout << arr[i].key << " " << arr[i].value << std::endl;
            }
        }
    }

    ~unordered_Map()
    {
        delete[] arr;
    }


    class iterator {
    private:
        Node* ptr;
        int index;
        int capacity;
        int size;
        std::pair<K, T>* pairPtr; // Declaration of pairPtr

    public:
        // Constructor
        iterator(Node* ptr, int index, int capacity, int size)
            : ptr(ptr), index(index), capacity(capacity), size(size), pairPtr(nullptr) {}

        // Overloading ++ operator
        iterator& operator++()
        {
			if (index == capacity)
				return *this;

			index++;
			while (index < capacity && !ptr[index].occupied)
				index++;

			return *this;
		}
        iterator operator++(int)
        {
			iterator temp = *this;
			++* this;
			return temp;
		}
        iterator& operator--()
        {
            if (index > 0) {
                index--;
                while (index >= 0 && !ptr[index].occupied) {
                    index--;
                }
            }
            return *this;
        }

        iterator operator--(int)
        {
			iterator temp = *this;
			--* this;
			return temp;
		}
        // Overloading * operator
        std::pair<K, T>& operator*()
        {
            pairPtr = new std::pair<K, T>(ptr[index].key, ptr[index].value);
            return *pairPtr;

        }
        pair<K, T>* operator->()
        {
			pairPtr = new std::pair<K, T>(ptr[index].key, ptr[index].value);
			return pairPtr;
		}
        // Overloading == operator
        bool operator==(const iterator& rhs) const
        {
			return index == rhs.index;
		}
		// Overloading != operator
        bool operator!=(const iterator& rhs) const
        {
			return index != rhs.index;
		}
		// Destructor
        ~iterator()
        {
			delete pairPtr;
		}
        bool operator < (const iterator& rhs) const
        {
			return index < rhs.index;
		}
        bool operator > (const iterator& rhs) const
        {
            return index > rhs.index;
        }
        bool operator <= (const iterator& rhs) const
        {
			return index <= rhs.index;
		}
        bool operator >= (const iterator& rhs) const
        {
            return index >= rhs.index;
        }


    };

    iterator begin()
    {
		int index = 0;
		while (index < capacity && !arr[index].occupied)
			index++;

		return iterator(arr, index, capacity, _size);
	}
    iterator end()
    {
        return iterator(arr, capacity, capacity, _size);

    }
};

#endif // !UNORDERED_MAP_H

