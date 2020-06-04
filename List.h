//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_LIST_H
#define WET2_LIST_H

#include "library2.h"

template<class T>
class ListNode {
private:
    int key;
    T *data;
    ListNode<T> *previous = nullptr;
    ListNode<T> *next = nullptr;

public:
    ListNode(int key, T *data, ListNode<T> *previous = nullptr, ListNode<T> *next = nullptr);

    int getKey();

    T *getData();

    void removeDataPointer();

    ListNode<T> *getPrevious();

    void setPrevious(ListNode<T> *nPrevious);

    ListNode<T> *getNext();

    void setNext(ListNode<T> *nNext);

    ~ListNode();
};

template<class T>
ListNode<T>::ListNode(int key, T *data, ListNode<T> *previous, ListNode<T> *next): key(key), data(data), previous(nullptr), next(nullptr) {

}

template<class T>
int ListNode<T>::getKey() {
    return key;
}

template<class T>
T *ListNode<T>::getData() {
    return data;
}

template<class T>
ListNode<T> *ListNode<T>::getPrevious() {
    return previous;
}

template<class T>
void ListNode<T>::setPrevious(ListNode<T> *nPrevious) {
    previous = nPrevious;
}

template<class T>
ListNode<T> *ListNode<T>::getNext() {
    return next;
}

template<class T>
void ListNode<T>::setNext(ListNode<T> *nNext) {
    next = nNext;
}

template<class T>
ListNode<T>::~ListNode() {
    if (data) {
        delete data;
    }
}

template<class T>
void ListNode<T>::removeDataPointer() {
    data = nullptr;
}

template<class T>
class List {
private:
    ListNode<T> *head = nullptr;
    ListNode<T> *current;

public:
    List();

    StatusType Insert(int key, T *data);

    bool Find(int key);

    ListNode<T> *FindNode(int key);

    StatusType Remove(int key);

    ListNode<T> *ResetIterator();

    ListNode<T> *NextIteration();

    ~List();
};

template<class T>
List<T>::List(): head(nullptr) {

}

template<class T>
StatusType List<T>::Insert(int key, T *data) {
    if (head) {
        // This isn't the first node in the list
        // Need to check if it already exists
        if (!Find(key)) {
            // The node doesn't exist, need to add it.
            ListNode<T> *nHead = new ListNode<T>(key, data);
            head->setPrevious(nHead);
            nHead->setNext(head);
            head = nHead;
            return SUCCESS;
        } else {
            return FAILURE;
        }
    } else {
        // This is the first node in the list
        head = new ListNode<T>(key, data, nullptr, nullptr);
        return SUCCESS;
    }
}

template<class T>
bool List<T>::Find(int key) {
    ListNode<T> *temp = head;
    while (temp) {
        if (temp->getKey() == key) {
            return true;
        }
        temp = temp->getNext();
    }
    return false;
}

template<class T>
ListNode<T> *List<T>::FindNode(int key) {
    ListNode<T> *temp = head;
    while (temp) {
        if (temp->getKey() == key) {
            return temp;
        }
        temp = temp->getNext();
    }
    return nullptr;
}

template<class T>
StatusType List<T>::Remove(int key) {
    if (head) {
        // There are nodes in the list, need to check if the one we want to remove
        // exists
        ListNode<T> *temp = FindNode(key);
        if (!temp) {
            // The node doesn't exist, nothing to remove.
            return FAILURE;
        } else {
            if (temp->getKey() == head->getKey()) {
                // We need to replace the head
                if (head->getNext()) {
                    head = head->getNext();
                    head->setPrevious(nullptr);
                    delete temp;
                    return SUCCESS;
                } else {
                    // The head is the only node in the list
                    head = nullptr;
                    delete temp;
                    return SUCCESS;
                }
            } else {
                // Need to remove a node from the middle/end of the list
                if (temp->getNext()) {
                    // We are removing a node from the middle of the list
                    ListNode<T> *prev = temp->getPrevious();
                    ListNode<T> *next = temp->getNext();
                    prev->setNext(next);
                    next->setPrevious(prev);
                    delete temp;
                    return SUCCESS;
                } else {
                    // We are removing a node from the end of the list
                    ListNode<T> *prev = temp->getPrevious();
                    prev->setNext(nullptr);
                    delete temp;
                    return SUCCESS;
                }
            }
        }
    } else {
        // There are no nodes in this list
        return FAILURE;
    }
}

template<class T>
List<T>::~List() {
    ListNode<T> *temp = head;
    ListNode<T> *next;
    if (temp) {
        next = temp->getNext();
    }
    while (temp) {
//        if (temp->getNext()) {
//            next = temp->getNext();
//        }
        next = temp->getNext();
        delete temp;
        temp = next;
    }
    head = nullptr;
}

template<class T>
ListNode<T> *List<T>::ResetIterator() {
    current = head;
    return current;
}

template<class T>
ListNode<T> *List<T>::NextIteration() {
    current = current->getNext();
    return current;
}

#endif //WET2_LIST_H
