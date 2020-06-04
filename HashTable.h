//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include "List.h"
#include <cmath>
#include <iostream>
#include <ostream>

using std::sqrt;
using std::modf;
using std::floor;

const double goldenFactor = (sqrt(5) - 1) / 2;

template<class T>
class HashTable {
private:
    int numberOfItems = 0;
    int size = 2;
    List<T> **hashArray;

    int HashFunction(int key);

    bool ExtendArray();

    bool ShrinkArray();

public:
    HashTable();

    StatusType Insert(int key, T *data);

    StatusType Find(int key);

    int getSize();

    ListNode<T> *FindNode(int key);

    StatusType Remove(int key);

    void PrintTable();

    ~HashTable();

};

template<class T>
HashTable<T>::HashTable() {
    hashArray = new List<T> *[size];
    for (int i = 0; i < size; ++i) {
        hashArray[i] = new List<T>();
    }
}

template<class T>
StatusType HashTable<T>::Insert(int key, T *data) {
    int index = HashFunction(key);
    if (hashArray[index]->Find(key)) {
        // The key already exists in the hash table
        return FAILURE;
    }
    hashArray[index]->Insert(key, data);
    numberOfItems++;
    double ratio = (double) numberOfItems / size;
    if (ratio > 0.5) {
        //std::cout << "EXTENDING ratio is: "<< ratio << "number of items is: " << numberOfItems << "size is: " << size << std::endl;
        if (!ExtendArray()) {
            return FAILURE;
        }
    }
    return SUCCESS;
}

template<class T>
StatusType HashTable<T>::Find(int key) {
    int index = HashFunction(key);
    if (hashArray[index]->Find(key)) {
        return SUCCESS;
    }
    return FAILURE;
}

template<class T>
ListNode<T> *HashTable<T>::FindNode(int key) {
    int index = HashFunction(key);
    return hashArray[index]->FindNode(key);
}

template<class T>
StatusType HashTable<T>::Remove(int key) {
    int index = HashFunction(key);
    if (!hashArray[index]->Find(key)) {
        // The key doesn't exist in the hash table
        return FAILURE;
    }
    hashArray[index]->Remove(key);
    numberOfItems--;
    double ratio = (double) numberOfItems / size;
    if (ratio < 0.25) {
        //std::cout << "SHRINKING ratio is: "<< ratio << "number of items is: " << numberOfItems << "size is: " << size << std::endl;
        if (!ShrinkArray()) {
            return FAILURE;
        }
    }

    return SUCCESS;
}

template<class T>
int HashTable<T>::HashFunction(int key) {
    double temp = key * goldenFactor;
    double num;
    double frac = modf(temp, &num);
    int index = floor(size * frac);
    return index;
}

template<class T>
bool HashTable<T>::ExtendArray() {
    size = size * 2;
    List<T> **nTable = new List<T> *[size];
    for (int i = 0; i < size; ++i) {
        nTable[i] = new List<T>();
    }

    for (int i = 0; i < size / 2; ++i) {
        List<T> *oldListAtIndex = hashArray[i];
        ListNode<T> *oldNode = oldListAtIndex->ResetIterator();
        while (oldNode) {
            int index = HashFunction(oldNode->getKey());
            nTable[index]->Insert(oldNode->getKey(), oldNode->getData());
            oldNode->removeDataPointer();
            oldNode = oldListAtIndex->NextIteration();
        }
        delete oldListAtIndex;
        hashArray[i] = nullptr;
    }
    delete[] hashArray;
    hashArray = nTable;
    return true;
}

template<class T>
bool HashTable<T>::ShrinkArray() {
    size = size / 2;
    List<T> **nTable = new List<T> *[size];
    for (int i = 0; i < size; ++i) {
        nTable[i] = new List<T>();
    }

    for (int i = 0; i < size * 2; ++i) {
        List<T> *oldListAtIndex = hashArray[i];
        ListNode<T> *oldNode = oldListAtIndex->ResetIterator();
        while (oldNode) {
            int index = HashFunction(oldNode->getKey());
            nTable[index]->Insert(oldNode->getKey(), oldNode->getData());
            oldNode->removeDataPointer();
            oldNode = oldListAtIndex->NextIteration();
        }
        delete oldListAtIndex;
        hashArray[i] = nullptr;
    }
    delete[] hashArray;
    hashArray = nTable;
    return true;
}

template<class T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < size; ++i) {
        delete hashArray[i];
    }
    delete[] hashArray;
}

template<class T>
int HashTable<T>::getSize() {
    return size;
}

template<class T>
void HashTable<T>::PrintTable() {
    for (int i = 0; i < size; ++i) {
        List<T> *listIndex = hashArray[i];
        ListNode<T> *node = listIndex->ResetIterator();
        std::cout << "Values at index: " << i << std::endl;
        while (node) {
            std::cout << node->getKey() << ", ";
            node = listIndex->NextIteration();
        }
        std::cout << std::endl;
    }

}


#endif //WET2_HASHTABLE_H
