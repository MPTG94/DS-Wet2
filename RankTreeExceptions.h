//
// Created by Mor on 14/06/2020.
//

#ifndef WET2_RANKTREEEXCEPTIONS_H
#define WET2_RANKTREEEXCEPTIONS_H

#include <iostream>
#include <exception>

using std::exception;

class KeyAlreadyExist : public exception {
public:
    const char *what() {
        int ErrorCode = 1;
        if (ErrorCode == 1) {
            // Print debugging information here
        }
        return "The key already exists in the tree. ErrorCode: 1\n";
    }
};

class KeyDoesNotExist : public exception {
public:
    const char *what() {
        int ErrorCode = 0;
        if (ErrorCode == 0) {
            // Print debugging information here
        }
        return "The key doesn't exist yet in the tree.\n";
    }
};

#endif //WET2_RANKTREEEXCEPTIONS_H
