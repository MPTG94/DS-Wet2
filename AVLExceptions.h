//
// Created by Mor on 14/06/2020.
//

#ifndef WET2_AVLEXCEPTIONS_H
#define WET2_AVLEXCEPTIONS_H

#include <iostream>
#include <exception>
using namespace std;

class KeyAlreadyExist:public exception {
public:
    const char* what() {
        return "The key is already exist in the tree\n";
    }
};

class KeyDoesNotExist:public exception {
public:
    const char* what() {
        return "The key does not exist in the tree\n";
    }
};

class IllegalInput:public exception {
public:
    const char* what() {
        return "The input was wrong. Please try harder.\n";
    }
};

#endif //WET2_AVLEXCEPTIONS_H
