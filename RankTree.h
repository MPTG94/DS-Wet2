//
// Created by Mor on 03/06/2020.
//

#ifndef WET2_RANKTREE_H
#define WET2_RANKTREE_H

#include <cmath>
#include <algorithm>
#include <iostream>

using std::max;
using std::ceil;
using std::log2;
using std::pow;

/**
 * Generic Template Class for a RankTreeNode
 * Made Specifically for use as a balanced AVL tree node meant to store dynamic
 * user data
 */
template<class K, class T>
class RankTreeNode {
private:
    K key;
    T *data;
    int height = 1;
    int rank = 1;
    RankTreeNode<K, T> *left = nullptr;
    RankTreeNode<K, T> *right = nullptr;
    RankTreeNode<K, T> *parent = nullptr;

    RankTreeNode<K, T> *Rebalance();

    int getBalanceFactor();

    void SwapNodesParent(RankTreeNode<K, T> *replacement);

    RankTreeNode<K, T> *LeftRightRotate();

    RankTreeNode<K, T> *RightLeftRotate();

    void updateRotatedRootParent(RankTreeNode<K, T> *previousRoot, RankTreeNode<K, T> *newRoot);

    RankTreeNode<K, T> *LeftRotate();

    RankTreeNode<K, T> *RightRotate();

public:
    RankTreeNode(K key, T *nData = nullptr, RankTreeNode *parent = nullptr, RankTreeNode *left = nullptr, RankTreeNode *right = nullptr);

    K getKey();

    T *getData();

    void removeDataPointer();

    int getHeight();

    int getRank();

    static K FindNodeByRank(RankTreeNode<K, T> *root, int searchRank);

    void PrintTreeInOrderWithRanks();

    int getNodeBalanceFactor();

    int getLeftChildRank();

    int getRightChildRank();

    int getLeftChildHeight();

    int getRightChildHeight();

    RankTreeNode<K, T> *findMin();

    RankTreeNode<K, T> *Insert(K nodeKey, T *nodeData = nullptr, RankTreeNode<K, T> *result = nullptr);

    RankTreeNode<K, T> *Remove(K nodeKey);

    RankTreeNode<K, T> *DeleteNode();

    void updateHeightRecurse();

    void updateNodeHeight();

    void updateNodeRank();

    RankTreeNode<K, T> *DeleteAndReplaceNodeWithLeftSuccessor();

    RankTreeNode<K, T> *DeleteAndReplaceNodeWithRightSuccessor();

    int FillNodesWithArrDataInOrder(T **&array, int size, int index);

    int FillArrayWithNodesInOrder(RankTreeNode<K, T> **&array, int size, int index);

    void updateRebalancedNodeHeights(RankTreeNode<K, T> *origin, RankTreeNode<K, T> *newRoot);

    RankTreeNode<K, T> *Find(K searchKey);

    RankTreeNode<K, T> *getNext();

    RankTreeNode<K, T> *getLeft();

    void setLeft(RankTreeNode<K, T> *ptr);

    RankTreeNode<K, T> *getRight();

    void setRight(RankTreeNode<K, T> *ptr);

    RankTreeNode<K, T> *getParent();

    void setParent(RankTreeNode<K, T> *ptr);

    RankTreeNode<K, T> *findMaxNoRank();

    RankTreeNode<K, T> *findMax();

    void DeleteTreeData();

    static void CreateCompleteBinaryTree(RankTreeNode<K, T> *root, int treeLevel, int currentLevel);

    static void RemoveExtraNodes(RankTreeNode<K, T> *root, int &numberOfNodesToRemove);

    static void FillKeysInOrder(RankTreeNode<K, T> *root, K &key);

    ~RankTreeNode();

    void RankSanity();
};

/**
 * =============================================================================
 * Tree Node Functions Implementation
 * =============================================================================
 */

/**
 * Checks the balance factor of the tree and rebalances it using the AVL tree
 * rotation algorithms, returns the root of the new subtree at the end
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the subtree after rebalancing
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::Rebalance() {
    // Getting the balance factor of the current node
    int balance = getBalanceFactor();

    if (balance < -1) {
        // The tree is right heavy
        int rightSubtreeBalance = getRight()->getBalanceFactor();
        if (rightSubtreeBalance > 0) {
            return RightLeftRotate();
        } else {
            return RightRotate();
        }

    } else if (balance > 1) {
        // The tree is left heavy
        int leftSubtreeBalance = getLeft()->getBalanceFactor();
        if (leftSubtreeBalance >= 0) {
            return LeftRotate();
        } else {
            return LeftRightRotate();
        }
    }

    return this;
}

template<class K, class T>
void RankTreeNode<K, T>::RankSanity() {
    int leftRank = 0;
    int rightRank = 0;

    if (left) {
        left->RankSanity();
        leftRank = left->rank;
    }
    if (right) {
        right->RankSanity();
        rightRank = right->rank;
    }
    if (rank != 1 + leftRank + rightRank) {
        std::cout << "ERROR IN TREE RANK" << std::endl;
    }
}

/**
 * Calculates the balance factor of the current node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The balance factor of the current node
 */
template<class K, class T>
int RankTreeNode<K, T>::getBalanceFactor() {
    updateNodeHeight();
    updateNodeRank();
    int leftHeight = 0;
    int rightHeight = 0;
    if (getRight()) {
        rightHeight = getRight()->height;
    }
    if (getLeft()) {
        leftHeight = getLeft()->height;
    }
    return leftHeight - rightHeight;
}

/**
 * Updates the pointers of the "next in line" node in order to make sure proper linkage
 * of the subtree after deletion
 * @tparam T Pointer to dynamically allocated object of type T
 * @param replacement The successor node
 */
template<class K, class T>
void RankTreeNode<K, T>::SwapNodesParent(RankTreeNode<K, T> *replacement) {
    if (replacement) {
        replacement->parent = this->parent;
        replacement->updateNodeHeight();
    }
    if (getParent()) {
        if (getParent()->getLeft() && getParent()->getLeft()->getKey() == getKey()) {
            this->parent->left = replacement;
        } else {
            this->parent->right = replacement;
        }
        getParent()->updateNodeHeight();
    }
}

/**
 * Updates the height of the node based on it's children
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
void RankTreeNode<K, T>::updateNodeHeight() {
    int leftHeight = getLeftChildHeight();
    int rightHeight = getRightChildHeight();
    height = 1 + max(leftHeight, rightHeight);
}

/**
 * Updates the rank of hte node based on it's children
 * @tparam K Class for node key comparisons
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
void RankTreeNode<K, T>::updateNodeRank() {
    int leftRank = getLeftChildRank();
    int rightRank = getRightChildRank();
    rank = 1 + leftRank + rightRank;
}

/**
 * Returns the rank of the left child
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The rank of the left child
 */
template<class K, class T>
int RankTreeNode<K, T>::getLeftChildRank() {
    if (left) {
        return left->rank;
    }
    return 0;
}

/**
 * Returns the rank of the right child
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The rank of the right child
 */
template<class K, class T>
int RankTreeNode<K, T>::getRightChildRank() {
    if (right) {
        return right->rank;
    }
    return 0;
}

/**
 * Returns the height of the left child
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The height of the left child
 */
template<class K, class T>
int RankTreeNode<K, T>::getLeftChildHeight() {
    if (left) {
        return left->height;
    }
    return 0;
}

/**
 * Returns the height of the right child
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The height of the right child
 */
template<class K, class T>
int RankTreeNode<K, T>::getRightChildHeight() {
    if (right) {
        return right->height;
    }
    return 0;
}

/**
 * Performs a left right rotation based on AVL tree rotation algorithms
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the rotated subtree
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::LeftRightRotate() {
    if (left) {
        left->RightRotate();
    }
    return this->LeftRotate();
}

/**
 * Performs a right left rotation based on AVL tree rotation algorithms
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the rotated subtree
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::RightLeftRotate() {
    if (right) {
        right->LeftRotate();
    }
    return this->RightRotate();
}

/**
 * Performs a left rotation based on AVL tree rotation algorithms
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the rotated subtree
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::LeftRotate() {
    RankTreeNode<K, T> *newRoot = left;
    left = left->right;
    if (left) {
        left->parent = this;
    }
    // Moving newRoot to the place of "this"
    newRoot->right = this;
    newRoot->parent = this->parent;
    if (this->getParent()) {
        // Checking if newRoot is a right child or left child of the original parent
        updateRotatedRootParent(this, newRoot);
    }
    this->parent = newRoot;
    // Updating node heights after switches
    if (newRoot->left) {
        newRoot->left->updateNodeHeight();
        newRoot->left->updateNodeRank();
    }
    if (newRoot->right) {
        newRoot->right->updateNodeHeight();
        newRoot->right->updateNodeRank();
    }
    updateRebalancedNodeHeights(this, newRoot);
//    if (newRoot->getParent()) {
//        newRoot->parent->updateNodeRank();
//    }
    return newRoot;
}

/**
 * Updates the height of nodes that were the core components of a rebalance
 * @tparam T Pointer to dynamically allocated object of type T
 * @param origin The original root of the subtree
 * @param newRoot The new root of the subtree
 */
template<class K, class T>
void RankTreeNode<K, T>::updateRebalancedNodeHeights(RankTreeNode<K, T> *origin, RankTreeNode<K, T> *newRoot) {
    if (origin->right) {
        origin->right->updateNodeHeight();
    }
    if (origin->left) {
        origin->left->updateNodeHeight();
    }
    origin->updateNodeHeight();
    origin->updateNodeRank();
    if (newRoot->right) {
        newRoot->right->updateNodeHeight();
    }
    if (newRoot->left) {
        newRoot->left->updateNodeHeight();
    }
    newRoot->updateNodeHeight();
    newRoot->updateNodeRank();
}

/**
 * Insert a new node to the subtree tree and returns the new root of the subtree
 * after rotations and rebalance
 * @tparam T Pointer to dynamically allocated object of type T
 * @param nodeKey The key of the new node
 * @param nodeData The data object of the new node
 * @param result A pointer to save a reference to the new node in
 * @return The new root of the rebalanced tree after insert
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::Insert(K nodeKey, T *nodeData, RankTreeNode<K, T> *result) {
    // Search for the right place to insert the node
    if (nodeKey > getKey()) {
        // The node will be a right child of this node
        if (!right) {
            // Found the correct spot to insert the node
            right = new RankTreeNode(nodeKey, nodeData, this);
            // Saving the new object to the result pointer for future retrieval
            result = right;
        } else {
            // Need to continue searching for the right spot of the new node
            right->Insert(nodeKey, nodeData, result);
        }
    } else if (nodeKey < getKey()) {
        // The node will be a left child of this node
        if (!left) {
            // Found the correct spot to insert the node
            left = new RankTreeNode(nodeKey, nodeData, this);
            // Saving the new object to the result pointer for future retrieval
            result = left;
        } else {
            // Need to continue searching for the right spot of the new node
            left->Insert(nodeKey, nodeData, result);
        }
    } else {
        // The input key already exists in the tree, returning nullptr without
        // modifying the tree or the input data
        return nullptr;
    }
    // Rebalancing tree after insertion
    return Rebalance();
}

/**
 * Updates the parent of a new rotated subtree root
 * @tparam T Pointer to dynamically allocated object of type T
 * @param previousRoot The old root of the subtree
 * @param newRoot The new root of the subtree
 */
template<class K, class T>
void RankTreeNode<K, T>::updateRotatedRootParent(RankTreeNode<K, T> *previousRoot, RankTreeNode<K, T> *newRoot) {
    // Checking if newRoot is a right child or left child of the original parent
    if (previousRoot->getParent()->getLeft() && previousRoot->getParent()->getLeft()->getKey() == previousRoot->getKey()) {
        previousRoot->parent->left = newRoot;
    } else {
        previousRoot->parent->right = newRoot;
    }
}

/**
 * Performs a right rotation based on AVL tree rotation algorithms
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the rotated subtree
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::RightRotate() {
    RankTreeNode<K, T> *newRoot = right;
    right = right->left;
    if (right) {
        right->parent = this;
    }
    // Moving newRoot to the place of "this"
    newRoot->left = this;
    newRoot->parent = this->parent;
    if (this->getParent()) {
        // Checking if newRoot is a right child or left child of the original parent
        updateRotatedRootParent(this, newRoot);
    }

    this->parent = newRoot;
    // Updating node heights after switches
    if (newRoot->left) {
        newRoot->left->updateNodeHeight();
        newRoot->left->updateNodeRank();
    }
    if (newRoot->right) {
        newRoot->right->updateNodeHeight();
        newRoot->right->updateNodeRank();
    }
    updateRebalancedNodeHeights(this, newRoot);
//    if (newRoot->getParent()) {
//        newRoot->parent->updateNodeRank();
//    }
    return newRoot;
}

/**
 * Replaces a node that is targeted for delete with it's leftmost successor
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the node's subtree after it's removal
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::DeleteAndReplaceNodeWithLeftSuccessor() {
    RankTreeNode<K, T> *next = left->findMax();
    if (left->getRight()) {
        next->parent->right = next->left;
        if (next->getLeft()) {
            next->left->parent = next->parent;
            if (next->left->parent) {
                next->left->parent->updateNodeHeight();
                next->left->parent->updateNodeRank();
            }
        }
        // might need to be reversed
        this->left->parent = next;
        next->left = this->left;
    }
    // Performing pointer and parent swap between the next node and the
    // node we need to delete
    this->SwapNodesParent(next);
    next->right = this->right;
    if (this->getRight()) {
        this->right->parent = next;
    }
    delete this;
    next->updateNodeHeight();
    return next->Rebalance();
}

/**
 * Replaces a node that is targeted for delete with it's right successor
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the node's subtree after it's removal
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::DeleteAndReplaceNodeWithRightSuccessor() {
    RankTreeNode<K, T> *next = this->right;
    // Performing pointer and parent swap between the next node and the
    // node we need to delete
    this->SwapNodesParent(next);
    delete this;
//    if (next->getParent()) {
//        next->getParent()->rank = next->getParent()->rank - 1;
//    }
    return next->Rebalance();
}

template<class K, class T>
RankTreeNode<K, T>::RankTreeNode(K key, T *nData, RankTreeNode *parent, RankTreeNode *left, RankTreeNode *right):
        key(key), data(nData) {
    height = 1;
    rank = 1;
    this->left = left;
    this->right = right;
    this->parent = parent;
};

/**
 * Returns the key of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The key of the tree node
 */
template<class K, class T>
K RankTreeNode<K, T>::getKey() {
    return key;
}

/**
 * Returns the stored data object in the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The data stored in the tree node
 */
template<class K, class T>
T *RankTreeNode<K, T>::getData() {
    if (data) {
        return data;
    }
    return nullptr;
}

/**
 * Returns the height of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The height of the tree node
 */
template<class K, class T>
int RankTreeNode<K, T>::getHeight() {
    return height;
}

/**
 * Completely deletes the tree from the given node downward, including all dynamically
 * allocated data in each node
 * also marks every deleted node as nullptr
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
void RankTreeNode<K, T>::DeleteTreeData() {
    if (left) {
        left->DeleteTreeData();
        delete left;
        left = nullptr;
    }
    if (right) {
        right->DeleteTreeData();
        delete right;
        right = nullptr;
    }
}

template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::getNext() {
    RankTreeNode<K, T> *current = this;
    if (current->getRight() != nullptr) {
        // This node has a right child, which means if we follow the branch
        // once to the right and then all the way to the left, we will find the
        // correct following child
        RankTreeNode<K, T> *rightOfCurrent = current->getRight();
        return rightOfCurrent->findMin();
    } else {
        RankTreeNode<K, T> *parent = current->getParent();
        while (parent != nullptr) {
            RankTreeNode<K, T> *child = parent->getLeft();
            if (child && child->getKey() == current->getKey()) {
                // The node we started with is the left child of the current parent,
                // which means the parent is the next node in the tree.
                return parent;
            }
            current = parent;
            parent = current->getParent();
        }
        return parent;
    }
}

/**
 * Returns the balance factor of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The balance factor of the tree node
 */
template<class K, class T>
int RankTreeNode<K, T>::getNodeBalanceFactor() {
    return getBalanceFactor();
}

/**
 * Deletes the data stored in this node,
 * marks the data as deleted and deletes the node itself
 * (does not delete child nodes)
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
RankTreeNode<K, T>::~RankTreeNode() {
    if (data) {
        delete data;
        data = nullptr;
    }
}

/**
 * Searches for a tree node with the input key and returns a pointer to it
 * @tparam T Pointer to dynamically allocated object of type T
 * @param searchKey The key of the node to search for
 * @return A pointer to the tree node if it is found, nullptr otherwise
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::Find(K searchKey) {
    if (getKey() > searchKey) {
        // The node we are looking for is a left child of this node
        if (left) {
            return left->Find(searchKey);
        } else {
            return nullptr;
        }
    } else if (getKey() < searchKey) {
        // The node we are looking for is a right child of this node
        if (right) {
            return right->Find(searchKey);
        } else {
            return nullptr;
        }
    } else {
        // We found the node we were looking for
        return this;
    }
}

/**
 * Removes a node with input key from the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @param nodeKey The key of the node to remove
 * @return The new root of the subtree after removal of the node
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::Remove(K nodeKey) {
    // Searching for the node to remove
    if (nodeKey > getKey()) {
        // The delete target is a right child of the current node
        if (right) {
            right->Remove(nodeKey);
        } else {
            return this;
        }
    } else if (nodeKey < getKey()) {
        // The delete target is a left child of the current node
        if (left) {
            left->Remove(nodeKey);
        } else {
            return this;
        }
    } else {
        // Found the node we need to delete
        // Using delete handler to remove the node from the tree
        return this->DeleteNode();
    }
    // Rebalancing tree after removal
    return Rebalance();
}

/**
 * Deletes the node from the tree, returns the new root of the node's subtree
 * after it's removal
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The new root of the node's subtree after it's removal
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::DeleteNode() {
    // Search for the next node to act as subtree root
    if (left) {
        return DeleteAndReplaceNodeWithLeftSuccessor();
    } else if (right) {
        return DeleteAndReplaceNodeWithRightSuccessor();
    } else {
        // The node we want to delete has no leafs, so it's subtree will remain empty
        // after deletion.
        this->SwapNodesParent(nullptr);
        delete this;
        return nullptr;
    }
}

/**
 * Finds the minimal node from this point downward in the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The minimal node
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::findMin() {
    if (left) {
        return left->findMin();
    }
    return this;
}

/**
 * Finds the maximal node from this point downward in the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The maximal node
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::findMaxNoRank() {
    if (right) {
        return right->findMaxNoRank();
    }
    return this;
}

template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::findMax() {
    if (right) {
        RankTreeNode<K, T> *temp = right->findMax();
        this->updateNodeRank();
        this->updateNodeHeight();
        return temp;
    }
    this->rank = this->rank - 1;
    this->height = 0;
    return this;
}

/**
 * Returns the left child of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The left child of the node
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::getLeft() {
    return left;
}

/**
 * Sets the left child of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @param ptr The new left child of the node
 */
template<class K, class T>
void RankTreeNode<K, T>::setLeft(RankTreeNode<K, T> *ptr) {
    left = ptr;
}

/**
 * Returns the right child of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The right child of the node
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::getRight() {
    return right;
}

/**
 * Sets the right child of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @param ptr The new right child of the node
 */
template<class K, class T>
void RankTreeNode<K, T>::setRight(RankTreeNode<K, T> *ptr) {
    right = ptr;
}

/**
 * Returns the parent of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @return The parent of the node
 */
template<class K, class T>
RankTreeNode<K, T> *RankTreeNode<K, T>::getParent() {
    return parent;
}

/**
 * Sets the parent of the node
 * @tparam T Pointer to dynamically allocated object of type T
 * @param ptr The new parent of the node
 */
template<class K, class T>
void RankTreeNode<K, T>::setParent(RankTreeNode<K, T> *ptr) {
    parent = ptr;
}

/**
 * Removes the reference to the data of the node
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
void RankTreeNode<K, T>::removeDataPointer() {
    data = nullptr;
}

template<class K, class T>
int RankTreeNode<K, T>::FillNodesWithArrDataInOrder(T **&array, int size, int index) {
    // Insert on left
    if (this->left) {
        index = this->left->FillNodesWithArrDataInOrder(array, size, index);
    }

    // Insert on root
    data = array[key];
    index++;

    // Insert on right
    if (this->right) {
        index = this->right->FillNodesWithArrDataInOrder(array, size, index);
    }
    return index;
}

/**
 * Fills an array with pointers to the RankTreeNodes of the tree
 * Array should be as large as the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @param array The array to fill with tree nodes
 * @param size The size of the array
 * @param index The current insertion position
 * @return The next index to insert values to the array
 */
template<class K, class T>
int RankTreeNode<K, T>::FillArrayWithNodesInOrder(RankTreeNode<K, T> **&array, int size, int index) {
    // Scan left
    if (this->left) {
        index = this->left->FillArrayWithNodesInOrder(array, size, index);
    }

    // Scan root
    array[index++] = this;

    // Scan right
    if (this->right) {
        index = this->right->FillArrayWithNodesInOrder(array, size, index);
    }
    return index;
}

/**
 * Creates a complete binary tree according to the current tree level
 * relative to the full height of the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @param root The root to create children for
 * @param treeLevel The total level the tree should reach
 * @param currentLevel The current level of the tree
 */
template<class K, class T>
void RankTreeNode<K, T>::CreateCompleteBinaryTree(RankTreeNode<K, T> *root, int treeLevel, int currentLevel) {
    // Checking if we created all necessary tree levels
    if (treeLevel <= currentLevel) {
        // We finished creating all new levels of the tree
        return;
    }
    root->left = new RankTreeNode<K, T>(0, nullptr, root);
    root->right = new RankTreeNode<K, T>(0, nullptr, root);
    CreateCompleteBinaryTree(root->left, treeLevel, currentLevel + 1);
    CreateCompleteBinaryTree(root->right, treeLevel, currentLevel + 1);
    // Updating root height
    root->updateNodeHeight();
    root->updateNodeRank();
}

/**
 * Removes unnecessary nodes from a Complete Binary Tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @param root The root of the current subtree
 * @param numberOfNodesToRemove The number of nodes left to remove
 */
template<class K, class T>
void RankTreeNode<K, T>::RemoveExtraNodes(RankTreeNode<K, T> *root, int &numberOfNodesToRemove) {
    // Checking if we need to continue removing nodes
    if (numberOfNodesToRemove == 0 || !root) {
        // We removed all the nodes we needed or we reached the end of the tree
        // scan so we shouldn't perform anymore actions
        return;
    }
    // Removing nodes in reverse in order, only from the bottom level of the tree
    RemoveExtraNodes(root->right, numberOfNodesToRemove);
    if (root->left == nullptr && root->right == nullptr) {
        if (root->parent) {
            // Setting the child of parent as nullptr to avoid segfault
            if (root->parent->left == root) {
                root->parent->left = nullptr;
            } else {
                root->parent->right = nullptr;
            }
        }
        numberOfNodesToRemove--;
        delete root;
        return;
    }
    RemoveExtraNodes(root->left, numberOfNodesToRemove);
    root->updateNodeHeight();
    root->updateNodeRank();
}

/**
 * Fills the nodes of the tree with keys using in order scan
 * @tparam T Pointer to dynamically allocated object of type T
 * @param root The root of the current subtree
 * @param key The current node's key
 */
template<class K, class T>
void RankTreeNode<K, T>::FillKeysInOrder(RankTreeNode<K, T> *root, K &key) {
    // We reached an empty node, no need to assign key
    if (!root) {
        return;
    }
    // Filling keys in the left part of the tree
    FillKeysInOrder(root->left, key);
    // Setting current node's key
    root->key = key++;
    // Filling keys in the right part of the tree
    FillKeysInOrder(root->right, key);
}

template<class K, class T>
int RankTreeNode<K, T>::getRank() {
    return rank;
}

template<class K, class T>
K RankTreeNode<K, T>::FindNodeByRank(RankTreeNode<K, T> *root, int searchRank) {
    if (root->right) {
        if (root->right->rank == searchRank - 1) {
            return root->key;
        } else if (root->right->rank > searchRank - 1) {
            return FindNodeByRank(root->right, searchRank);
        } else {
            return FindNodeByRank(root->left, searchRank - root->right->rank - 1);
        }
    } else if (root->left) {
        if (searchRank == 1) {
            return root->key;
        } else {
            return root->left->key;
        }
    } else {
        return root->key;
    }
}

template<class K, class T>
void RankTreeNode<K, T>::PrintTreeInOrderWithRanks() {
    if (left) {
        left->PrintTreeInOrderWithRanks();
    }
    std::cout << "Key is: " << key << " Rank is: " << rank << std::endl;

    if (right) {
        right->PrintTreeInOrderWithRanks();
    }
}

template<class K, class T>
void RankTreeNode<K, T>::updateHeightRecurse() {
    if (left) {
        left->updateHeightRecurse();
    }
    if (right) {
        right->updateHeightRecurse();
    }
    int leftHeight = getLeftChildHeight();
    int rightHeight = getRightChildHeight();
    height = 1 + max(leftHeight, rightHeight);
}


/**
 * Generic Template Class for an AVL Tree
 * Stores and organizes AVL tree nodes meant to store dynamic user data
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
class RankTree {
    RankTreeNode<K, T> *root;

public:
    RankTree();

    RankTree(int numberOfNodes);

    RankTreeNode<K, T> *GetRoot();

    void MarkRootAsNullptr();

    void MarkRootDataAsNullptr();

    void FillRankTreeNodesWithArrData(T **&array, int size, int index = 0);

    void FillArrWithNodesInOrder(RankTreeNode<K, T> **&array, int size, int index = 0);

    RankTreeNode<K, T> *Find(K key);

    K FindByRank(int searchRank);

    void Insert(K key, T *data = nullptr);

    RankTreeNode<K, T> *InsertGetBack(K key, T *data);

    void PrintTreeWithRanks();

    void Remove(K key);

    bool IsRootNull();

    ~RankTree();

    void RankSanityCheck();
};

/**
 * =============================================================================
 * Tree Functions Implementation
 * =============================================================================
 */

/**
 * Creates an empty AVL Tree to store nodes with type T
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
RankTree<K, T>::RankTree(): root(nullptr) {};


/**
 * Returns a pointer to the root of the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @return A pointer to the root of the tree
 */
template<class K, class T>
RankTreeNode<K, T> *RankTree<K, T>::GetRoot() {
    if (root) {
        return root;
    }
    return nullptr;
}

/**
 * Searches for a node by key in the tree and returns it if it exists
 * @tparam T Pointer to dynamically allocated object of type T
 * @param key The key of the node to find
 * @return A pointer to the node if it's found in the tree, nullptr otherwise
 */
template<class K, class T>
RankTreeNode<K, T> *RankTree<K, T>::Find(K key) {
    if (!root) {
        return nullptr;
    }

    return root->Find(key);
}

/**
 * Inserts a new node with the given key and data to the tree
 * If the key already exists in the tree, nothing will be done
 * (This also means the dynamic data sent to the function will not
 * be freed)
 * @tparam T Pointer to dynamically allocated object of type T
 * @param key The key of the new node
 * @param data The data of the new node
 */
template<class K, class T>
void RankTree<K, T>::Insert(K key, T *data) {
    if (!root) {
        // The tree is empty, inserting the new node as the root
        root = new RankTreeNode<K, T>(key, data);
    } else {
        root = root->Insert(key, data);
    }
}

/**
 * Insert a new node to the subtree tree and returns a pointer to the new node
 * @tparam T Pointer to dynamically allocated object of type T
 * @param key The key of the new node
 * @param data The data object of the new node
 * @param result A pointer to save a reference to the new node in
 * @return A pointer to the newely inserted ndoe
 */
template<class K, class T>
RankTreeNode<K, T> *RankTree<K, T>::InsertGetBack(K key, T *data) {
    if (!root) {
        // The tree is empty, inserting the new node as the root
        root = new RankTreeNode<K, T>(key, data);
        return root;
    } else {
        RankTreeNode<K, T> *result = nullptr;
        root = root->Insert(key, data, result);
        return result;
    }
}

/**
 * Removes a node with the given key from the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @param key The key of the node to remove
 */
template<class K, class T>
void RankTree<K, T>::Remove(K key) {
    if (root) {
        root = root->Remove(key);
    }
}

/**
 * Checks if the tree is empty
 * @tparam T Pointer to dynamically allocated object of type T
 * @return True if the root of the tree is a nullptr, False otherwiese
 */
template<class K, class T>
bool RankTree<K, T>::IsRootNull() {
    return (root == nullptr);
}

/**
 * Marks the root of the tree as nullptr
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
void RankTree<K, T>::MarkRootAsNullptr() {
    root = nullptr;
}

/**
 * Marks the data of the root of the tree as nullptr
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
void RankTree<K, T>::MarkRootDataAsNullptr() {
    root->removeDataPointer();
}

/**
 * Deletes all nodes stored in the tree, including the dynamic data stored in them
 * and then sets the root of the tree to nullptr
 * @tparam T Pointer to dynamically allocated object of type T
 */
template<class K, class T>
RankTree<K, T>::~RankTree() {
    if (root) {
        root->DeleteTreeData();
        delete root;
        root = nullptr;
    }
}

/**
 * Fills the tree nodes with data pointer of type T taken from
 * the input array, the array is ordered so every node's correct data is in
 * the cell whose index is it's key
 * @tparam T Pointer to dynamically allocated object of type T
 * @param array The array to fill with tree nodes
 * @param size The size of the array
 * @param index The current insertion position
 */
template<class K, class T>
void RankTree<K, T>::FillRankTreeNodesWithArrData(T **&array, int size, int index) {
    if (!root) {
        // Tree is empty, no nodes to save data in
        return;
    }
    index = 0;
    root->FillNodesWithArrDataInOrder(array, size, index);
}

/**
 * Fills an array with pointers to the RankTreeNodes of the tree
 * Array should be as large as the tree
 * @tparam T Pointer to dynamically allocated object of type T
 * @param array The array to fill with tree nodes
 * @param size The size of the array
 * @param index The current insertion position
 */
template<class K, class T>
void RankTree<K, T>::FillArrWithNodesInOrder(RankTreeNode<K, T> **&array, int size, int index) {
    if (!root) {
        // Tree is empty, no nodes to get back
        return;
    }
    index = 0;
    root->FillArrayWithNodesInOrder(array, size, index);
}

/**
 * Creates a new almost complete binary tree according to the input parameters
 * @tparam T Pointer to dynamically allocated object of type T
 * @param root The key of the trees' root
 * @param maxHeight The height ot the trees' root
 * @param maxNode the key of the highest node
 */
template<class K, class T>
RankTree<K, T>::RankTree(int numberOfNodes) {
    // Calculating the height of the tree
    int treeLevel = ceil(log2(numberOfNodes));
    if (treeLevel == log2(numberOfNodes)) {
        // The number of nodes is a power of two, need to increase it by one
        // to create a real complete binary tree
        treeLevel++;
    }
    // Creating the root of the complete tree
    root = new RankTreeNode<K, T>(0, nullptr, nullptr);
    RankTreeNode<K, T>::CreateCompleteBinaryTree(root, treeLevel, 1);

    // After creating the complete binary tree, we need to remove extra nodes
    // that are unnecessary
    int numberOfNodesToRemove = (int) (pow(2, treeLevel) - 1) - numberOfNodes;
    if (treeLevel != 0) {
        // The tree has more than one floor, meaning we need to remove a few of
        // the nodes
        RankTreeNode<K, T>::RemoveExtraNodes(root, numberOfNodesToRemove);
    }
    K key = K();
    RankTreeNode<K, T>::FillKeysInOrder(root, key);
}

template<class K, class T>
K RankTree<K, T>::FindByRank(int searchRank) {
    return RankTreeNode<K, T>::FindNodeByRank(root, searchRank);
}

template<class K, class T>
void RankTree<K, T>::PrintTreeWithRanks() {
    if (root) {
        root->PrintTreeInOrderWithRanks();
    }
}

template<class K, class T>
void RankTree<K, T>::RankSanityCheck() {
    if (root) {
        root->RankSanity();
    }
}

#endif //WET2_RANKTREE_H
